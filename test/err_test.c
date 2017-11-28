// Error Detection Unit Testing Program

// Author: Matthew Knight
// File Name: err_test.c
// Date: 2017-11-27

// This program tests for correct operation of the error detection layer. It
// will not test for error detection as there is the possibility that errors
// could be missed, but instead checks that the calculations are correct.

#include <stdio.h>
#include <time.h>
#include <assert.h>
#include "global_def.h"
#include "phy.h"
#include "err.h"

// Number of iterations to run checksum testds
#define N 1000

int main(void)
{
    // Initialize
    init_phy();
    init_err();
    srand(time(NULL)) ;

    // basic test
    uint16_t tx_buf[DG_WORDS-1], rx_buf[DG_WORDS-1], phy_stage[DG_WORDS];
    

    // No Error added to packet
    int i;
    for (i = 0; i < N; i++) {
	int j;
	for (j = 0; j < DG_WORDS - 1; j++)
	    tx_buf[j] = j;

	write_fifo(TX_ERR, tx_buf);

	calc_checksum();
	read_fifo(TX_PHY, phy_stage);
	
	// Now to go up a layer
	write_fifo(RX_PHY, phy_stage);
	check_datagram();
	
	// Success if packet not dropped
	assert(get_fifo_len(RX_ERR) != 0);
	read_fifo(RX_ERR, rx_buf);

	for (j = 0; j < DG_WORDS - 1; j++)
	    assert(rx_buf[j] == tx_buf[j]);
    }

    printf("ERR Test - Success\n");
    return 0;
}
