#ifndef TERMINAL_H
#define TERMINAL_H

#include <stdbool.h>
#include <stddef.h>

#define CFGSTR_SIZE 128

enum { DEVICE_OPT = 0, SPEED_OPT };

void terminal_clear();
void terminal_print_starting();
void terminal_print_playing();
int terminal_commandline_parse(int argc, char *argv[]);
void terminal_get_name(char* name, size_t s);
void terminal_change(bool is_canonic);

void terminal_copy_serdev(char* dest);
int terminal_get_serspeed();

#endif