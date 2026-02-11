#include "terminal.h"
#include <getopt.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct termios og_terminal;

char *const token[] = { [DEVICE_OPT] = "dev", [SPEED_OPT] = "speed", NULL };

char g_serfile[CFGSTR_SIZE] = "";
int32_t g_serspeed = 0;

void terminal_print_playing(){
    printf(
        "\n=====GAME HAS STARTED====\n"
        "Press 'b' to move left,\n"
        "Press 'j' to move right,\n"
        "-------------------------\n"
    );
}

void terminal_clear(){
    system("clear");
}

void terminal_print_starting(){
    printf("======GAME STARTING======\n"
        "Press 's' to start,\n"
        "Press '+' to increase speed,\n"
        "Press '-' to decrease speed\n"
        "-------------------------\n"
    );
}

void terminal_get_name(char* name, size_t s){
    while (1) {
        if (fgets(name, s, stdin) == NULL) continue;
        name[strcspn(name, "\n")] = 0;
        if (strlen(name) >= 3 && strlen(name) <= 32) break;

        // Clean input buffer as needed
        if(strlen(name) > 32){
            int c;
            while ((c = getchar()) != '\n' && c != EOF) { }
        }
        printf("Name must be 3–32 characters.\nTry again: ");
    }
}

static void print_help() {
    printf("Help for BLP example\n");
    printf("Command line parameters:\n");
    printf("   -h                           : Print help\n");
    printf("   -s dev=devf,speed=baud_rate  : Set serial port device and speed\n");
}

int terminal_commandline_parse(int argc, char *argv[]){
    int opt;
    char *subopts;
    char *value;
    int errfnd = 0;

    while ((opt = getopt(argc, argv, "hs:?")) != -1) {
        switch(opt) {
            case 'h': print_help(); return 0;
            case 's':
                subopts = optarg;
                while (*subopts != '\0' && !errfnd) {
                    switch(getsubopt(&subopts, token, &value)) {
                        case DEVICE_OPT:
                        if(strlen(value) < CFGSTR_SIZE) strcpy(g_serfile, value);
                        else { printf("Serial file name too long\n"); exit(EXIT_FAILURE); }
                        break;
                        case SPEED_OPT: g_serspeed = atoi(value); break;
                        default: break;
                    }
                }
                break;
            default: print_help(); return 0;
        }
    }
    return 1;
}

void terminal_change(const bool is_canonic){
    struct termios tty;

    if (tcgetattr(STDIN_FILENO, &tty) == -1){
        return;
    }

    if (!is_canonic) {
        struct termios raw = tty;

        raw.c_lflag &= ~(ICANON | ECHO);
        raw.c_cc[VMIN]  = 1;
        raw.c_cc[VTIME] = 0;

        if (tcsetattr(STDIN_FILENO, TCSANOW, &raw) == -1){
            return;
        }
    } else {
        tty.c_lflag |= (ICANON | ECHO);
        if (tcsetattr(STDIN_FILENO, TCSANOW, &tty) == -1)
            return;
    }
}

void terminal_copy_serdev(char* dest){
    strcpy(dest, g_serfile);
}

int terminal_get_serspeed(){
    return g_serspeed;
}