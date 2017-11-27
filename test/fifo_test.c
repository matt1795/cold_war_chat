// FIFO Module test program

// Author: Matthew Knight
// File Name: fifo_test.c
// Date: 2017-11-14

// This program uses the standard library's assert macro to test our simple fifo
// implementation

#include <stdio.h>
#include <assert.h>
#include "fifo.h"
#include "global_def.h"

int main(void)
{
    // Program variables
    int i;
    uint16_t read, write = 1;
    
    // Initialize
    uint16_t buf[DG_WORDS*BUFLEN];
    struct fifo test_fifo;
    init_fifo(&test_fifo, buf, BUFLEN, DG_WORDS);
    
    // Check if fifo is empty
    assert(get_fifo_len(&test_fifo) == 0); 

    // Error on reading empty fifo
    assert(read_fifo(&test_fifo, &read) != 0);
    
    // FIll up the fifo, successful every time
    for (i = 0; i < BUFLEN; i++)
	assert(write_fifo(&test_fifo, &write) == 0);

    // test that it's full
    assert(get_fifo_len(&test_fifo) == BUFLEN);
    
    // error on writing to full fifo
    assert(write_fifo(&test_fifo, &write));

    // Check that data is correct
    read_fifo(&test_fifo, &read);
    assert(read == write);
    
    // Check that fifo length went down by 1
    assert(get_fifo_len(&test_fifo) == BUFLEN - 1);

    // Test if flush works
    flush_fifo(&test_fifo);
    assert(get_fifo_len == 0);
    
    
    printf("FIFO Test - Success\n");
}
