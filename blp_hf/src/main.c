#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <getopt.h>
#include <fcntl.h>
#include <poll.h>
#include <ctype.h>
#include <time.h>

#include "app.h"
#include "terminal.h"
#include "serial.h"


//state machine states


// --- Utility functions

int main(int argc, char *argv[]) {

    if(terminal_commandline_parse(argc, argv) == 0){
        return 0;
    }
    
    app_init();
    
    char devicename[CFGSTR_SIZE];
    terminal_copy_serdev(devicename);
    printf("Terminal application has Started!\n"
        "Device: %s\n"
        "Speed: %d\n",
        devicename, terminal_get_serspeed()
    );
    printf("Press any key to continue...\n");
    char tmp;
    read(STDIN_FILENO, &tmp, 1);

    terminal_clear();
    terminal_print_starting();
    while(1) {
        serial_handle_io(app_get_current_state_ptr(), app_get_score_ptr());
        app_handle_state_change();
    }
    
    return 0;
}