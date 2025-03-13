#pragma once

typedef enum state
{
    idle,
    moving,
    stopped,
    door_open
} state;

void* queue_updater(void* arg);
void state_init(void);
void state_machine(void);
state get_state(void);
void set_state(state new_state);
void check_stopped(void);