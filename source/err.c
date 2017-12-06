// Error Detection Layer Implementation

// Author: Matthew Knight
// File Name: err.c
// Date: 2017-11-14

// This layer adds an error detection service to the stack. Initially this will
// employ a checksum and later will be improved to a software CRC. It will check
// the checksum of incoming messages and calculate and append the checksum to
// the datagram.


#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/BIOS.h>

#include "DSP2802x_Device.h"
#include "global_def.h"
#include "err.h"
#include "phy.h"

// buffers
static uint16_t rx_stage[DG_WORDS];
static uint16_t tx_stage[DG_WORDS];
static uint16_t rx_buf[(DG_WORDS - 1)*(BUFLEN + 1)];
static uint16_t tx_buf[(DG_WORDS - 1)*(BUFLEN + 1)];

// fifos
static struct fifo rx_fifo, tx_fifo;
struct fifo *RX_ERR, *TX_ERR;

// Initialize Error detection layer
void init_err(void)
{
    // Initialize fifo objects
    init_fifo(&rx_fifo, rx_buf, BUFLEN, DG_WORDS - 1);
    init_fifo(&tx_fifo, tx_buf, BUFLEN, DG_WORDS - 1);

    // Initialize interface pointers
    RX_ERR = &rx_fifo;
    TX_ERR = &tx_fifo;
}

// check incoming datagram
void check_datagram(void)
{
    while (TRUE) {
        // wait for datagrams from phy
        Semaphore_pend(rx_phy_sem, BIOS_WAIT_FOREVER);

        // Load datagram into staging buffer
        read_fifo(RX_PHY, rx_stage);
    
        // Calculate the checksum
        int i;
        uint16_t check = 0;
        
        for (i = 0; i < DG_WORDS; i++)
            check += rx_stage[i];

        // if zero then it is correct, continue moving
        if (!check) {
            while (write_fifo(RX_ERR, rx_stage))
                Task_yield();
            Semaphore_post(rx_err_sem);

        }
    }
}

// add checksum to outgoing checksum
void calc_checksum(void)
{
    while (TRUE) {
        // wait for datagrams from above
        Semaphore_pend(tx_err_sem, BIOS_WAIT_FOREVER);

        // Load datagram into staging buffer
        read_fifo(TX_ERR, tx_stage);

        // Calculate checksum
        int i;
        uint16_t check = 0;

        for (i = 0; i < DG_WORDS - 1; i++)
        check += tx_stage[i];

        // Two's complement
        check = ~check;
        check++;
        tx_stage[DG_WORDS-1] = check;

        // send to phy later
        while(write_fifo(TX_PHY, tx_stage))
            Task_yield();
        Semaphore_post(tx_phy_sem);
    }
}
