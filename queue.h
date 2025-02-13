#pragma once

#include <stdlib.h>


typedef struct {
    int size;
    char floor; 
    bool up; // invert for down 
} queue_element;

queue_element queue[];

void enqueue(); // appends to array
void dequeue(); // pops from array
void clear_queue();
void get_queue(); // gets element and dequeues 
char get_first();
void check_floors();