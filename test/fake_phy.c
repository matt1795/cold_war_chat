// Fake Physical Layer for Unit Testing

// Author: Matthew Knight
// File Name: fake_phy.c
// Date: 2017-11-17

// This layer acts as the physical layer so that the stack can be debugged and
// developed on a system with an operating system. It really only intializes the
// FIFO's which the main function can play around with.

#include "phy.h"

// Buffers
static uint16_t rx_buf[DG_WORDS*(BUFLEN + 1)]; 
static uint16_t tx_buf[DG_WORDS*(BUFLEN + 1)]; 

// fifo interfaces
static struct fifo rx_fifo, tx_fifo;

// Initialize physical layer - only run once
int init_phy(void)
{
    // initialize fifo objects
    init_fifo(&rx_fifo, rx_buf, BUFLEN, DG_WORDS);
    init_fifo(&tx_fifo, tx_buf, BUFLEN, DG_WORDS);

    // Point assign interface pointers
    RX_PHY = &rx_fifo;
    TX_PHY = &tx_fifo;
}
