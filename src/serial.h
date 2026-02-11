#ifndef SERIAL_H
#define SERIAL_H

#include "state.h"

int serial_open();
void serial_handle_io(State* current_state, char* score_str);

#endif