// Error Detection Unit Testing Program

// Author: Matthew Knight
// File Name: err_test.c
// Date: 2017-11-27

// This program tests for correct operation of the error detection layer. It
// will not test for error detection as there is the possibility that errors
// could be missed, but instead checks that the calculations are correct.

#include <stdio.h>
#include <assert.h>
#include "global_def.h"
#include "phy.h"
#include "err.h"

#define N 1

int main(void)
{
    // Initialize
    init_phy();
    init_err();
    
    // basic test
    uint16_t buf1[DG_WORDS-1], buf2[DG_WORDS];

    int i;
    for (i = 0; i < DG_WORDS - 1; i++)
	buf1[i] = i;

    write_fifo(TX_ERR, buf1);

    calc_checksum();
    read_fifo(TX_PHY, buf2);
    
    // Now to go up a layer
    write_fifo(RX_PHY, buf2);
    check_datagram();
    
    // Success if packet not dropped
    assert(get_fifo_len(RX_ERR) != 0);

    for (i = 0; i < DG_WORDS; i++)
	assert(buf1[i] == buf2[i]);

    printf("ERR Test - Successful\n");
    return 0;
}
