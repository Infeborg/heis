#pragma once

typedef struct {
    int size;
    char floor; 
    bool up; // invert for down 
} queue;

void enqueue(); // appends to array
void dequeue(); // pops from array
void clear_queue();
queue get_queue(); // gets element and dequeues 
char get_first();