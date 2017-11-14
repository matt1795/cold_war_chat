// Error Detection Layer Implementation

// Author: Matthew Knight
// File Name: err.c
// Date: 2017-11-14

// This layer adds an error detection service to the stack. Initially this will
// employ a checksum and later will be improved to a software CRC. It will check
// the checksum of incoming messages and calculate and append the checksum to
// the datagram.

#include "err.h"

// buffers
static uint16_t rx_stage[DG_WORDS];
static uint16_t tx_stage[DG_WORDS];
static uint16_t rx_buf[(DG_WORDS - 1)*BUFLEN];
static uint16_t tx_buf[(DG_WORDS - 1)*BUFLEN];

// fifos
static struct fifo rx_fifo, tx_fifo;

// Initialize Error detection layer
void init_err(void)
{
    // Initialize fifo objects
    init_fifo(&rx_fifo, rx_buf, BUFLEN, DG_WORDS - 1);
    init_fifo(&tx_fifo, rx_buf, BUFLEN, DG_WORDS - 1);

    // Initialize interface pointers
    RX_ERR = &rx_fifo;
    TX_ERR = &tx_fifo;
}

// check incoming datagram
void check_datagram(void)
{
    // wait for datagrams from phy
    
    // Load datagram into staging buffer
    read_fifo(RX_PHY, rx_stage);

    // Calculate the checksum
    int i;
    uint16_t check = 0;
    
    for (i = 0; i = DG_WORDS - 1; i++)
	check += rx_stage[i];

    // if zero then it is correct, continue moving
    if (!check)
	write_fifo(RX_ERR, rx_stage);
        
}

// add checksum to outgoing checksum
void calc_checksum(void)
{
    // wait for datagrams from above

    // Load datagram into staging buffer
    read_fifo(TX_ERR, tx_stage);

    // Calculate checksum
    int i;
    uint16_t check = 0;

    for (i = 0; i < DG_WORDS - 2; i++)
	check += tx_buf[i];

    // Two's complement
    check ~= check;
    check++;
    tx_buf[DG_WORDS-1] = check;

    // send to phy later
    write_FIFO(TX_PHY, tx_buf);
}
