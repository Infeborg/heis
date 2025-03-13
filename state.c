#include "state.h"
#include "queue.h"
#include "door.h"
#include "driver/elevio.h"
#include <stdio.h>

state current_state = idle;


void* queue_updater(void* arg) {
    while (1) {
        update_queue();
        check_stopped();
        printf("thread\n");
        usleep(100000);
    }
    return NULL;
}


// Går til en etasje om den ikke er på en
void state_init() {
    printf("initfunc\n");
    close_door();
    while (elevio_floorSensor() == -1) {
        elevio_motorDirection(DIRN_DOWN);
        clear_queue();
    }
    elevio_motorDirection(DIRN_STOP);
}

void state_machine() {
    // For å sette igang heisen.
    while (1)
    {
        // Ting som må sjekkes
        printf("Elevator State as number: %d\n", current_state);
        update_queue();
        check_stopped();
        floor_light_on();

        switch (get_state()) {
        case idle: // Venter på noe å gjøre
            printf("idle\n");
            set_state(moving);
            if (find_order()) {
                set_state(moving);
            }
            break;
        case moving: // Gjennomfører ordre
            printf("moving\n");      
            execute_order();
        // Her har den motatt en bestilling, og beveger seg deretter. Man kan se hvilken vei heisen beveger seg, etter
        // hvilke fortegn det er på differansen mellom den definerte tilstanden og bestillingen.
            break;
        case stopped: // Noen har trykket på stoppknappen-> Køen tømmes
            printf("stopped\n");  
            clear_queue();
            // elevio_motorDirection(DIRN_STOP);
        
            // set_state(idle);
            break;


        case door_open:
            printf("door open!!!\n");  
            open_door();
            wait_for_close();
            close_door();
            set_state(idle);
            break;
        }
        usleep(100000);
    }
}


state get_state() {
    return current_state;
}

void set_state(state new_state) {
    current_state = new_state;
}


void check_stopped() {
    if (elevio_stopButton() == 1) {
        elevio_motorDirection(DIRN_STOP);
        elevio_stopLamp(1);
        set_state(stopped);
    }
    else {
        elevio_stopLamp(0);
    }
    if (!find_order()) {
        state_init();

        set_state(idle);
        
    }
}
