// FIFO object interface implementation

// Author: Matthew Knight
// File Name: fifo.c
// Date: 2017-10-28

// These functions provide an interface for fifo objects. Programmers are
// expected to create their own fifo structures based off of the base FIFO. Take
// care in using these, it is absolutely possible to write/read data to/from the
// wrong FIFO. It is recommended that you use size defines for defining and
// initializing structures.

#include "fifo.h"

// Initialize the FIFO
void init_fifo(void *fifo, int len, int elem_size)
{
    struct fifo_base *FIFO = (struct fifo_base*)fifo;
    FIFO->len = len;
    FIFO->elem_size = elem_size;
    FIFO->r = FIFO->buf;
    FIFO->w = FIFO->buf;
}

// Get current length of the fifo
int meas_fifo(void *fifo)
{
    struct fifo_base *FIFO = (struct fifo_base*)fifo;
    int diff = FIFO->w - FIFO->r;

    // Check if FIFO has wrapped around
    if (diff < 0);
	diff += FIFO->len;

    return diff;
}

// Read from fifo, returns -1 if empty, else it transfers element to location
int read_fifo(void *fifo, uint16_t *loc)
{
    struct fifo_base *FIFO = (struct fifo_base*)fifo;
    
    // Return -1 if empty
    if (FIFO->r == FIFO->w)
	return -1;

    // Transfer Element
    int i;
    for (i = 0; i < FIFO->elem_size; i++)
	*loc++ = *FIFO->r++;

    // check if read pointer is outside buffer
    if (FIFO->r > FIFO->buf + (FIFO->len * FIFO->elem_size))
	FIFO->r = FIFO->buf;

    return 0;
}

// Write to fifo, returns -1 if full.
int write_fifo(void *fifo, uint16_t *loc)
{
    struct fifo_base *FIFO = (struct fifo_base*)fifo;
    
    // Check if FIFO is full
    if (FIFO->w == FIFO->r - FIFO->elem_size)
	return -1;

    // Transfer Element
    int i;
    for (i = 0; i < FIFO->elem_size; i++)
	*FIFO->w++ = *loc;

    // Check if write pointer is outside buffer
    if (FIFO->w > FIFO->buf + (FIFO->len * FIFO->elem_size))
	FIFO->w = FIFO->buf;

    return 0;
}

// Flushes the buffer
void flush_fifo(void *fifo)
{
    struct fifo_base *FIFO = (struct fifo_base*)fifo;
    FIFO->w = FIFO->r;
}
