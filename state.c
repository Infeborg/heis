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
    elevio_motorDirection(DIRN_UP);
    while (1) {
        if (elevio_floorSensor() != -1) {
            elevio_motorDirection(DIRN_STOP);
            break;
        }
    }
}

void state_machine() {

    state_init();
    // For å sette igang heisen.
    while (1)
    {
        
        update_queue();
        check_stopped();
        // inn i switchcasen må vi ha den nåværende staten, typ sånn "Typ en funksjon "hent nåværende state"
        switch (get_state()) {
        case idle: // finner ordre
            find_order();
            set_state(moving);
            // do_order();

            break;
        // I denne tilstanden står den i ro i definert tilstand, venter på bestilling.
        case moving: // Gjennomfører ordre
            execute_order();
            set_state(door_open);
        // Her har den motatt en bestilling, og beveger seg deretter. Man kan se hvilken vei heisen beveger seg, etter
        // hvilke fortegn det er på differansen mellom den definerte tilstanden og bestillingen.
            break;
        case stopped: // Noen har trykket på stoppknappen-> Køen tømmes
            clear_queue();
            set_state(idle);
            break;
        case door_open:
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
    if (elevio_stopButton()) {
        set_state(stopped);
    }
}
