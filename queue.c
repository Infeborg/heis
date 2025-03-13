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
<<<<<<< HEAD
=======

    // Oppdaterer up & down
    check_cab_floor();
>>>>>>> bbb985bda0373464071a8d6126ddcb019eb2ee5c
   
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
<<<<<<< HEAD
                // printf("%d", destination_floor);
                
=======

                /*
                int safety_counter = 0;
                while (elevio_floorSensor() != destination_floor && safety_counter < 1000000) {
                    safety_counter++;
                }
                if (safety_counter >= 1000000) {
                    printf("ERROR: Infinite loop detected in execute_order!\n");
                }
                */
>>>>>>> bbb985bda0373464071a8d6126ddcb019eb2ee5c
                while (elevio_floorSensor() != destination_floor) { // is moving
                    if (dir > 0) {
                        int stop_floor = find_order_over(current_floor, destination_floor);
                        if (elevio_floorSensor() == stop_floor) { // stops at floor
                            elevio_motorDirection(DIRN_STOP);
<<<<<<< HEAD
                            //set_state(door_open);
                            queue[button][stop_floor] = false;
                            elevio_buttonLamp(stop_floor, button, 0);
                            //set_state(door_open);
                            open_door();
                            wait_for_close();
                            close_door();
=======
                            set_state(door_open);
                            queue[button][stop_floor] = false;
>>>>>>> bbb985bda0373464071a8d6126ddcb019eb2ee5c
                        }
                    }
                    if(dir < 0) {
                        int stop_floor = find_order_under(current_floor, destination_floor);
                        if (elevio_floorSensor() == stop_floor) { // stops at floor
                            elevio_motorDirection(DIRN_STOP);
<<<<<<< HEAD
                            //set_state(door_open);
                            queue[button][stop_floor] = false;
                            elevio_buttonLamp(stop_floor, button, 0);
                            open_door();
                            wait_for_close();
                            close_door();
                            //set_state(door_open);
=======
                            set_state(door_open);
                            queue[button][stop_floor] = false;
>>>>>>> bbb985bda0373464071a8d6126ddcb019eb2ee5c
                        }
                    }
                }
            }
<<<<<<< HEAD
=======
        }
    }    
}

// Henter verdier fra cabraden
void check_cab_floor() {
    for (int floor = 0; floor < N_FLOORS; floor++) {
        if (queue[2][floor]) {
            int current_floor = elevio_floorSensor();
            int dir = current_floor - floor;
            if (dir < 0) {
                queue[0][floor] = true;
                queue[2][floor] = false;
            }
            else {
                queue[1][floor] = true;
                queue[2][floor] = false;
            }
>>>>>>> bbb985bda0373464071a8d6126ddcb019eb2ee5c
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
    if (current_floor < 0 || current_floor >= N_FLOORS || destination_floor < 0 || destination_floor >= N_FLOORS) {
        return destination_floor; // If at the bottom floor, return 0
    }
    for (int i = current_floor + 1; i <= destination_floor; i++) {
        for (int button = 0; button < N_BUTTONS; button++) {
            if (queue[button][i]) {
                return i;
            }
        }
    }
<<<<<<< HEAD
    return destination_floor;
=======
    for (int i = current_floor - 1; i >= destination_floor && i >= 0; i--) {
        printf("Checking queue[1][%d] = %d\n", i, queue[1][i]); // Debugging
        if (queue[1][i]) { 
            return i; 
        }
    }
    return destination_floor; // If no intermediate stops, return final destination
>>>>>>> bbb985bda0373464071a8d6126ddcb019eb2ee5c
}

int find_order_under(int current_floor, int destination_floor) {
    if (current_floor < 0 || current_floor >= N_FLOORS || destination_floor < 0 || destination_floor >= N_FLOORS) {
        return destination_floor; // If at the bottom floor, return 0
    }
    for (int i = current_floor - 1; i >= destination_floor; i--) {
        for (int button = 0; button < N_BUTTONS; button++) {
            if (queue[button][i]) {
                return i;
            }
        }
    }
<<<<<<< HEAD
    return destination_floor;
=======
    for (int i = current_floor - 1; i >= destination_floor && i >= 0; i--) {
        printf("Checking queue[1][%d] = %d\n", i, queue[1][i]); // Debugging
        if (queue[1][i]) { 
            return i; 
        }
    }
    return destination_floor; // If no intermediate stops, return final destination
>>>>>>> bbb985bda0373464071a8d6126ddcb019eb2ee5c
}


void floor_light_on() {
    if(elevio_floorSensor() != -1) {
        elevio_floorIndicator(elevio_floorSensor());
    }
}