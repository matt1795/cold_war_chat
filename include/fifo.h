// FIFO object declaration and interface functions

// Author: Matthew Knight
// File Name: fifo.h
// Date: 2017-10-28

// These functions provide an interface for fifo objects. Programmers are
// expected to create their own fifo structures based off of the base FIFO. Take
// care in using these, it is absolutely possible to write/read data to/from the
// wrong FIFO. These are specifically used for 16-bit memory

#ifndef FIFO_H
#define FIFO_H

#include <stdint.h>

// Base FIFO structure used for interfacing
struct fifo
{
    uint16_t *w, *r;
    int len;
    int elem_size;
    uint16_t* buf;
};

// Initialize the FIFO
void init_fifo(struct fifo *f, uint16_t *buf, int len, int elem_size);

// Get current FIFO length
int get_fifo_len(struct fifo *f);

// Read from fifo, returns -1 if empty, else it transfers element to location
int read_fifo(struct fifo *f, uint16_t *loc);

// Write to fifo, returns -1 if full.
int write_fifo(struct fifo *f, uint16_t *loc);

// Flushes the buffer
void flush_fifo(struct fifo *f);

#endif
