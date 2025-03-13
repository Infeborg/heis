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
                printf("Order found at button %d, floor %d\n", button, floor); // DEBUG
                return true; // Order found
            }
        }
    }
    printf("No orders in queue\n"); // DEBUG
    return false; // No order found
}

// Gjennomfører ordre
void execute_order() {
    
    int current_floor = elevio_floorSensor(); // sjekker etasjen man er på
    for (int button = 0; button < N_BUTTONS; button++) { 
        for (int floor = N_FLOORS - 1; floor >= 0 ; floor--) {
            if (queue[button][floor]) { // hvis noe skal gjøres, gjør det
                int destination_floor = floor;
                int dir = fix_dir(button, floor, current_floor, destination_floor);
                elevio_motorDirection(dir);
                // printf("%d", destination_floor);
                
                while (elevio_floorSensor() != destination_floor) { // is moving
                    if (dir > 0) {
                        int stop_floor = find_order_over(current_floor, destination_floor);
                        if (elevio_floorSensor() == stop_floor) { // stops at floor
                            elevio_motorDirection(DIRN_STOP);
                            //set_state(door_open);
                            queue[button][stop_floor] = false;
                            elevio_buttonLamp(stop_floor, button, 0);
                            //set_state(door_open);
                            open_door();
                            wait_for_close();
                            close_door();
                        }
                    }
                    if(dir < 0) {
                        int stop_floor = find_order_under(current_floor, destination_floor);
                        if (elevio_floorSensor() == stop_floor) { // stops at floor
                            elevio_motorDirection(DIRN_STOP);
                            //set_state(door_open);
                            queue[button][stop_floor] = false;
                            elevio_buttonLamp(stop_floor, button, 0);
                            open_door();
                            wait_for_close();
                            close_door();
                            //set_state(door_open);
                        }
                    }
                }
            }
        }
    }    
}

// Setter funksjonell verdi på dir
int fix_dir(int button, int floor, int current_floor, int destination_floor) {
    int dir = destination_floor - current_floor;
    if (dir < 0) { // Setter dir til en funksjonell verdi
        dir = -1;
    }
    else if (dir > 0) {
        dir = 1;
    }
    return dir;
}

// Når stoppknappen er trykket skal køen tømmes.
void clear_queue()
{
    for (int floor = 0; floor < N_FLOORS; floor++)
    {
        for (int button = 0; button < N_BUTTONS; button++)
        {
            queue[button][floor] = 0;
            elevio_buttonLamp(floor, button, 0);
        }
    }
}

int find_order_over(int current_floor, int destination_floor) {
    for (int i = current_floor + 1; i <= destination_floor; i++) {
        for (int button = 0; button < N_BUTTONS; button++) {
            if (queue[button][i]) {
                return i;
            }
        }
    }
    return destination_floor;
}

int find_order_under(int current_floor, int destination_floor) {
    for (int i = current_floor - 1; i >= destination_floor; i--) {
        for (int button = 0; button < N_BUTTONS; button++) {
            if (queue[button][i]) {
                return i;
            }
        }
    }
    return destination_floor;
}


void floor_light_on() {
    if(elevio_floorSensor() != -1) {
        elevio_floorIndicator(elevio_floorSensor());
    }
}