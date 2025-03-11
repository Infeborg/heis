#pragma once
#include <stdlib.h>
#include <stdbool.h>

// Oppdaterer køen om noe trykkes
void update_queue(void);

// True om noe skal gjøres, false om ikke
bool find_order(void);

// Gjennomfører ordre
void execute_order(void); 

// Sjekker hvilken cabplasser som skal brukes
void check_cab_floor(void);

// Setter funksjonell verdi på dir
int fix_dir(int button, int floor, int current_floor, int destination_floor);

// Når stoppknappen er trykket skal køen tømmes.
void clear_queue(void); 

// Iterer gjennom køen. Dersom heisen beveger seg oppover, (Altså state moving + fortegn)
// Skal den lete gjennom køen om det er noen elementer i køen
// som er over + som skal oppover. Dersom det er det, skal den stoppe på veien.
int find_order_over(int current_floor, int destination_floor);

// Iterer gjennom køen. Dersom heisen beveger seg nedover.
// Skal den lete gjennom køen om det er noen elementer i køen som er under + skal nedover.
int find_order_under(int current_floor, int destination_floor);