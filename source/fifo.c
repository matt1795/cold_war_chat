// f object interface implementation

// Author: Matthew Knight
// File Name: fifo.c
// Date: 2017-10-28

// These functions provide an interface for fifo objects. Programmers are
// expected to create their own fifo structures based off of the base f. Take
// care in using these, it is absolutely possible to write/read data to/from the
// wrong f. It is recommended that you use size defines for defining and
// initializing structures. Buffers must have room allocated for one extra
// element.

#include "fifo.h"

// Initialize the f statically
void init_fifo(struct fifo *f, uint16_t *buf, int len, int elem_size)
{ 
    f->buf = buf;
    f->len = len;
    f->elem_size = elem_size;
    f->r = f->buf;
    f->w = f->buf;
}

// Get current length of the fifo
int get_fifo_len(struct fifo *f)
{
    int diff = f->w - f->r;

    // Check if f has wrapped around
    if (diff < 0)
	diff += (f->len + 1)*f->elem_size;

    return diff/f->elem_size;
}

// Read from fifo, returns -1 if empty, else it transfers element to location
int read_fifo(struct fifo *f, uint16_t *loc)
{
    // Return -1 if empty
    if (f->r == f->w)
	return -1;

    // Transfer Element
    int i;
    for (i = 0; i < f->elem_size; i++)
	*loc++ = *f->r++;

    // check if read pointer is outside buffer
    if (f->r >= f->buf + ((f->len + 1) * f->elem_size))
	f->r = f->buf;

    return 0;
}

// Write to fifo, returns -1 if full.
int write_fifo(struct fifo *f, uint16_t *loc)
{
    // Return -1 if full
    if (get_fifo_len(f) == f->len)
	return -1;

    // Transfer Element
    int i;
    for (i = 0; i < f->elem_size; i++)
	*f->w++ = *loc++;

    // Check if write pointer is outside buffer
    if (f->w >= f->buf + ((f->len + 1) * f->elem_size))
	f->w = f->buf;

    return 0;
}

// Flushes the buffer
void flush_fifo(struct fifo *f)
{
    f->w = f->r;
}
