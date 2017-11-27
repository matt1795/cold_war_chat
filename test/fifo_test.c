// FIFO Module test program

// Author: Matthew Knight
// File Name: fifo_test.c
// Date: 2017-11-14

// This program uses the standard library's assert macro to test our simple fifo
// implementation

#include <stdio.h>
#include <assert.h>
#include "global_def.h"
#include "fifo.h"

int main(void)
{
    // Program variables
    int i;
    uint16_t read[DG_WORDS], write[DG_WORDS];
    
    // load write datagram with arbitrary values
    for (i = 0; i < DG_WORDS; i++)
	write[i] = i;
    
    // Initialize
    uint16_t buf[DG_WORDS*(BUFLEN + 1)];
    struct fifo test_fifo;
    init_fifo(&test_fifo, buf, BUFLEN, DG_WORDS);
    
    // Check if fifo is empty
    assert(get_fifo_len(&test_fifo) == 0); 

    // Error on reading empty fifo
    assert(read_fifo(&test_fifo, read) != 0);
    
    // FIll up the fifo, successful every time
    for (i = 0; i < BUFLEN; i++)
	assert(write_fifo(&test_fifo, write) == 0);

    // Flush it
    flush_fifo(&test_fifo);
    
    // Make sure it's empty
    assert(get_fifo_len(&test_fifo) == 0);

    // FIll up the fifo again, successful every time
    for (i = 0; i < BUFLEN; i++)
	assert(write_fifo(&test_fifo, write) == 0);

    // test that it's full
    assert(get_fifo_len(&test_fifo) == BUFLEN);
    
    // error on writing to full fifo
    assert(write_fifo(&test_fifo, write) != 0);

    // Check that data is correct
    assert(read_fifo(&test_fifo, read) == 0);
    for (i = 0; i < DG_WORDS; i++)
	assert(read[i] == write[i]);
    
    // Check that fifo length went down by 1
    assert(get_fifo_len(&test_fifo) == BUFLEN - 1);

    // Test if flush works
    flush_fifo(&test_fifo);
    assert(get_fifo_len(&test_fifo) == 0);
    
    
    printf("FIFO Test - Success\n");
}
