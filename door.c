#include "door.h"

void wait_for_close() {
    usleep(3000000);

    while (elevio_obstruction()) {
        usleep(3000000);
    }
    printf("LUKKER DORA!\n");
}

// Skrur på lys
void open_door() {
    elevio_doorOpenLamp(1);
}

// Skrur av åpen-dør-lyset
void close_door() {
    elevio_doorOpenLamp(0);
}

// Trenger kanskje ikke en egen en for door, hehe. Kan flytte det ut etterhvert