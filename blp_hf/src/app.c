#include "app.h"
#include "serial.h"
#include "terminal.h"
#include "network.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    Network net;
    char* server_url;
    int fd_serial;
    State current, previous;
    char score[16];
} App;

static App app;

void app_init(){
    network_init(&app.net);
    app.fd_serial = serial_open();
    if (app.fd_serial < 0) {
        perror("Failed to open serial"); exit(EXIT_FAILURE);
    }
    terminal_change(false);
}

static void log_score(const char* name, const char* score) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", t);

    FILE *log_file = fopen("log.txt", "a");
    if (log_file) {
        fprintf(log_file, "Name: %s, Score: %s, at %s\n", name, score, timestamp);
        fclose(log_file);
    } else {
        perror("Error opening log file");
    }
}

static void handle_game_end() {
    terminal_change(true);

    printf("Game has ended, your score is: %s\n", app.score);
    printf("Enter name: ");

    char name[36] = {0};
    terminal_get_name(name, 36);

    log_score(name, app.score);
    network_send_score(&app.net, name, app.score);

    terminal_change(false);
}

void app_handle_state_change(){
    while(app.current != app.previous){
        switch(app.current) {
            case STARTING:
                terminal_clear();
                fflush(stdout);
                terminal_print_starting();
                break;
            case PLAYING:
                terminal_print_playing();
                break;
            case ENDING:
                handle_game_end();
                break;
            default:
                break;
        }
        app.previous = app.current;
        if(app.current == ENDING){
            app.current = STARTING;
        }
    }
}

State* app_get_current_state_ptr(){
    return &app.current;
}

char* app_get_score_ptr(){
    return app.score;
}