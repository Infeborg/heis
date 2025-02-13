#pragma once
#include <queue.h>
#include <door.h>
#include <elevio.h>

typedef struct {
    bool stop;
    char target;

} controller; 

void move_up();
void move_down();
void get_target();
