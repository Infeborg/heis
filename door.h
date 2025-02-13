#pragma once

typedef struct {
    bool open; // invert to close
    bool obstruction;
} door;

void open_door();
void wait_for_close();
