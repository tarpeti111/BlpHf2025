#include <termios.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdint.h>
#include <poll.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "terminal.h"
#include "state.h"

#define TAG_LENGTH 5
#define TAG_GAME_PLAYING "<PLA>"
#define TAG_GAME_END "<END>"

char tag_detector[TAG_LENGTH] = {0};

char serdev[CFGSTR_SIZE];
char serspeed;

struct pollfd fds[2];

static speed_t get_baudrate(int baud) {
    switch (baud) {
        case 9600: return B9600;
        case 19200: return B19200;
        case 38400: return B38400;
        case 57600: return B57600;
        case 115200: return B115200;
        case 230400: return B230400;
        case 460800: return B460800;
        case 500000: return B500000;
        case 576000: return B576000;
        case 921600: return B921600;
        case 1000000: return B1000000;
        case 1152000: return B1152000;
        case 1500000: return B1500000;
        case 2000000: return B2000000;
        case 2500000: return B2500000;
        case 3000000: return B3000000;
        case 3500000: return B3500000;
        case 4000000: return B4000000;
        default: return -1;
    }
}

int serial_open() {
    serspeed = terminal_get_serspeed();
    
    terminal_copy_serdev(serdev);

    int fd = open(serdev, O_RDWR);
    if (fd < 0) return -1;

    struct termios seropts;
    tcgetattr(fd, &seropts);

    //Turn off canonical mode, echo and posting
    seropts.c_cflag = CS8 | CREAD | CLOCAL;
    seropts.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    seropts.c_iflag &= ~(IXON | IXOFF | ICRNL);
    seropts.c_oflag &= ~OPOST;
    seropts.c_cc[VMIN] = 1;
    seropts.c_cc[VTIME] = 5;

    speed_t spd = get_baudrate(serspeed);
    cfsetospeed(&seropts, spd);
    cfsetispeed(&seropts, spd);
    tcsetattr(fd, TCSANOW, &seropts);

    fds[0].fd = fd;
    fds[0].events = POLLIN;

    fds[1].fd = STDIN_FILENO;
    fds[1].events = POLLIN;

    return fd;
}

void serial_handle_io(State* current_state, char* score_str){
    if (poll(fds, 2, -1) < 0) { perror("poll"); return; }

    int fd = fds[0].fd;

    // --- Serial input ---
    if(fds[0].revents & POLLIN) {
        char buf[256];
        int n = read(fd, buf, sizeof(buf));
        if(n <= 0) return;
        
        // Always search for a tag in serial input with a sliding window ("<...>")
        for(int i = 0; i < n; i++) {
            memmove(tag_detector, tag_detector + 1, TAG_LENGTH-1);
            tag_detector[TAG_LENGTH-1] = buf[i];
            
            if (!memcmp(tag_detector, TAG_GAME_PLAYING, TAG_LENGTH)) *current_state = PLAYING;
            if (!memcmp(tag_detector, TAG_GAME_END, TAG_LENGTH)) {
                *current_state = ENDING;
                score_str[0] = 0;
            }
            
            if(*current_state == ENDING && isdigit(buf[i])) {
                int len = strlen(score_str);
                if (len < sizeof(score_str)-1) score_str[len] = buf[i], score_str[len+1] = 0;
            }
        }

        if(*current_state == STARTING) {
            fwrite(buf, 1, n, stdout);
            fflush(stdout);
        }
    }
    
    // --- User input ---
    if(fds[1].revents & POLLIN) {
        char c;
        if(read(STDIN_FILENO, &c, 1) > 0) write(fd, &c, 1);
    }
}

