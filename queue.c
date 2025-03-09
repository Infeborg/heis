#include "driver/elevio.h"
#include <stdio.h>
#include <stdbool.h>
#include "door.h"

// Vil lage et array som har lagt inn bestilte etasjer som er sant etter knapptype
bool queue[N_BUTTONS][N_FLOORS] = {0};

// Må brukes slik at if (en button er pressed) => update
void update_queue() {
    // oppdaterer køa
    
    for (int floor = 0; floor < N_FLOORS; floor++)
    {
        for (int button = 0; button < N_BUTTONS; button++)
        {
            if (elevio_callButton(floor, button)) {
                queue[button][floor] = elevio_callButton(floor, button);
            }
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
        }
    }
}


// Skal skje: kun false => ingenting skjer
// Fjerner elementet i køen som er utført
int find_order() {
    // Oppdaterer up & down
    check_cab_floor();
   
    // do order
    int current_floor = elevio_floorSensor(); // sjekker etasjen man er på
    for (int button = 0; button < N_BUTTONS-1; button++) {
        for (int floor = 0; floor < N_FLOORS; floor++) {
            if (queue[button][floor]) { // hvis noe skal gjøres, gjør det
                destination_floor = floor;
                dir = fix_dir(button, floor, current_floor, destination_floor);
                elevio_motorDirection(dir);
                return 
            }
        }
    }
}

void execute_order() {
    while (elevio_floorSensor() != destination_floor) { // is moving
        if (dir > 0) {
            int stop_floor = find_order_over(current_floor, destination_floor);
            if (elevio_floorSensor() == stop_floor) { // stops at floor
                elevio_motorDirection(DIRN_STOP);
                open_door();
                wait_for_close();
                close_door();
                queue[button][stop_floor] = false;
            }
        }
        if(dir < 0) {
            int stop_floor = find_order_under(current_floor, destination_floor);
            if (elevio_floorSensor() == stop_floor) { // stops at floor
                elevio_motorDirection(DIRN_STOP);
                open_door();
                wait_for_close();
                close_door();
                queue[button][stop_floor] = false;
            }
        }
    }
}

// Setter funksjonell verdi på dir
int fix_dir(int button, int floor, int current_floor, int destination_floor) {
    dir = destination_floor - current_floor;

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
        }
    }
}

// Find the next stop floor above the current floor, if any.
int find_order_over(int current_floor, int destination_floor) {
    for (int i = current_floor + 1; i <= destination_floor; i++) {
        if (queue[0][i]) { // Check if there's an "up" order at this floor
            return i; // Stop at the first valid stop floor
        }
    }
    return destination_floor; // If no intermediate stops, return final destination
}

// Find the next stop floor below the current floor, if any.
int find_order_under(int current_floor, int destination_floor) {
    for (int i = current_floor - 1; i >= destination_floor; i--) {
        if (queue[1][i]) { // Check if there's a "down" order at this floor
            return i; // Stop at the first valid stop floor
        }
    }
    return destination_floor; // If no intermediate stops, return final destination
}
