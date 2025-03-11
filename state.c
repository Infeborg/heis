#include "state.h"
#include "queue.h"
#include "driver/elevio.h"

state current_state = idle;

// Kanskje bedre om alt dette er i main, og heller implementere koden som funksjoner, så skriver funksjonene under:

/*
DIRN_DOWN = -1,
DIRN_STOP = 0,
DIRN_UP = 1
*/

// Går til en etasje om den ikke er på en
void state_init() {
    printf("initfunc\n");
    while (elevio_floorSensor() == -1) {
        elevio_motorDirection(DIRN_UP);
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
        switch (get_state()) {
        case idle: // Venter på noe å gjøre
            printf("idle\n");    
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
            elevio_motorDirection(DIRN_STOP);
            set_state(idle);
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
        set_state(stopped);
    }
}
