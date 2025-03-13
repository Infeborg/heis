#include "driver/elevio.h"
#include <stdio.h>
#include <stdbool.h>
#include "door.h"
#include "queue.h"
#include "state.h"

// Vil lage et array som har lagt inn bestilte etasjer som er sant etter knapptype
bool queue[N_BUTTONS][N_FLOORS] = {0};

// Oppdaterer køen om noe trykkes
void update_queue() {
    for (int floor = 0; floor < N_FLOORS; floor++) {
        for (int button = 0; button < N_BUTTONS; button++) {
            bool button_pressed = elevio_callButton(floor, button);
            if (button_pressed) {
                queue[button][floor] = true;
                elevio_buttonLamp(floor, button, 1);
            }
        }
    }
}

// True om noe skal gjøres, false om ikke
bool find_order() {
   
    // check order
    for (int button = 0; button < N_BUTTONS; button++) { // N_BUTTONS -1 
        for (int floor = N_FLOORS - 1; floor >= 0 ; floor--) {
            if (queue[button][floor]) { // hvis noe skal gjøres, gjør det
                return true; // Order found
            }
        }
    }
    printf("No orders in queue\n"); // DEBUG
    return false; // No order found
}



// Når stoppknappen er trykket skal køen tømmes.
void clear_queue()
{
    for (int floor = 0; floor < N_FLOORS; floor++)
    {
        for (int button = 0; button < N_BUTTONS; button++)
        {
            queue[button][floor] = 0;
            elevio_buttonLamp(floor, 0, 0);
            elevio_buttonLamp(floor, 1, 0);
            elevio_buttonLamp(floor, 2, 0);
        }
    }
}



void floor_light_on() {
    if(elevio_floorSensor() != -1) {
        elevio_floorIndicator(elevio_floorSensor());
    }
}


    
        int find_closest_order(int current_floor) {
            int closest_floor = -1;
            int min_distance = N_FLOORS;  // Maksimal avstand
        
            for(int floor = 0; floor < N_FLOORS; floor++) {
                for (int button = 0; button < N_BUTTONS; button++) {
                    if (queue[button][floor]) {
                        int distance = abs(floor - current_floor);
                        if (distance < min_distance) {
                            min_distance = distance;
                            closest_floor = floor;
                        }
                    }
                }
            }
            return closest_floor;  // Returnerer -1 hvis ingen bestillinger finnes
        }
        
        void execute_order() {
            while (find_order()) {  // Så lenge det er bestillinger
                int current_floor = elevio_floorSensor();
                int destination_floor = find_closest_order(current_floor);  // Finn nærmeste etasje
        
                if (destination_floor == -1) {
                    return; // Ingen bestillinger igjen
                }
        
                int dir = (destination_floor > current_floor)? DIRN_UP : DIRN_DOWN;
                elevio_motorDirection(dir);
                
                while (elevio_floorSensor() != destination_floor) {
                    // Vent til vi ankommer riktig etasje
                    usleep(100);
                }
        
                elevio_motorDirection(DIRN_STOP);
        
                // Fjern alle bestillinger i denne etasjen
                for (int button = 0; button < N_BUTTONS; button++) {
                    queue[button][destination_floor] = false;
                    elevio_buttonLamp(destination_floor, button, 0);
                }
        
                // Åpne døren og vent
                open_door();
                wait_for_close();
                close_door();
            }
        }

