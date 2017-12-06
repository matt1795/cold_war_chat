// Transport Layer Implementation

// Author: Matthew Knight
// File Name: transport.c
// Date: 2017-11-14

// The transport layer of this stack provides two services, connection
// establishment and reliable data transfer. Connection Establishment will

#include "global_def.h"
#include "transport.h"
#include "err.h"

// Buffers
static uint16_t rx_buf[MSG_WORDS*BUFLEN];
static uint16_t tx_buf[MSG_WORDS*BUFLEN];

// fifo objects
static struct fifo rx_fifo, tx_fifo;

// Layer Initialization
void init_transport()
{
    // Initialize fifos
    init_fifo(&rx_fifo, rx_buf, BUFLEN, MSG_WORDS);
    init_fifo(&tx_fifo, tx_buf, BUFLEN, MSG_WORDS);

    // Initialize interface pointers
    RX_TRAN = &rx_fifo;
    TX_TRAN = &tx_fifo;
}
