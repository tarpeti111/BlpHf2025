#ifndef APP_H
#define APP_H

#include "network.h"
#include "state.h"

void app_init();
char* app_get_score_ptr();
State* app_get_current_state_ptr();
void app_handle_state_change();

#endif