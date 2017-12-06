// Physical Layer Implementation

// Author: Matthew Knight
// File Name: phy.c
// Date: 2017-11-06

// These functions implement the physical layer through hardware and software
// interrupts. Internal memory components of the physical layer are static
// variables, the only interface available will be given to the data-link layer.

// The physical layer consists of two GPIO pins on the MCU, (one must be
// interruptible), and the timer1 module. 

#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Swi.h>

#include "DSP2802x_Device.h"
#include "global_def.h"
#include "phy.h"

//  SWI handles
extern const Swi_Handle swi_handle_rx_packet, swi_handle_phy_timer;

// Internal defines
#define PREAMBLE 0xAA
#define POSTAMBLE 0x55

// Preamble Analysis
static uint32_t T, pulse;
static uint32_t T_samp[3], pulse_samp[3];
static uint16_t edge_count;

// State of the transciever
static enum State
{
    WAIT,
    WAIT_DELAY,
    RX_PRE,
    RX_DATA,
    RX_POST,
    TX_PRE,
    TX_DATA,
    TX_POST
} state;

// Buffers
static uint16_t stage_buf[DG_WORDS];
static uint16_t rx_buf[DG_WORDS*(BUFLEN+1)];
static uint16_t tx_buf[DG_WORDS*(BUFLEN+1)];

// fifo interfaces
struct fifo *RX_PHY, *TX_PHY;
static struct fifo rx_fifo, tx_fifo;


// Initialize physical layer - only run once
void init_phy(void)
{
    // initialize fifo objects
    init_fifo(&rx_fifo, rx_buf, BUFLEN, DG_WORDS);
    init_fifo(&tx_fifo, tx_buf, BUFLEN, DG_WORDS);

    // Point assign interface pointers
    RX_PHY = &rx_fifo;
    TX_PHY = &tx_fifo;


}

// RX GPIO HWI
void hwi_rx_packet(void)
{
    Swi_post(swi_handle_rx_packet);
}

// Timer1 HWI
void hwi_phy_timer(void)
{

    Swi_post(swi_handle_phy_timer);
}

// RX Software Interrupt
void swi_rx_packet(void)
{
    switch (state) {
	case WAIT:
	    // Semaphore_Post(sem_channel);
	    state = RX_PRE;
	    // initialize and reset timer
	    edge_count = 0;
	    break;
	case RX_PRE:
	    if (edge_count % 2) {
	        T_samp[(edge_count - 1)/2] = 1;
	        // reset timer
	    }
	    else {
	        pulse_samp[edge_count/2] = 1;
	    }

	    if (++edge_count > 5 ){
	        int i;
	        T = 0;
	        pulse = 0;

	        for (i = 0; i < 3; i++) {
	            T += T_samp[i]/6;
	            pulse += pulse_samp[i]/6;
	        }
		
	        // configure timer, disable GPIO int
	        state = RX_DATA;
	    }
	    break;
    }
    
    // toggle gpio 
}

// Timer Software Interrupt
void swi_phy_timer(void)
{
    switch (state) {
	/*
    case RX_PRE:	// Preamble timeout
	    // disable timer int
	    // congifure gpio int
	    state = WAIT;
	    break;
	case RX_DATA:

	    *(stage_buf + bit/MEM_LEN) = 0x8000 >> bit % MEM_LEN;
	    if (++bit > DATAGRAM_BITS) 

		state = RX_POST;
	    

	    break;
	case RX_POST:

	    *(post_buf + bit/BYTE_LEN) = 0x80 >> bit % BYTE_LEN;
	    if (++bit > DATAGRAM_BITS + BYTE_LEN) { 
	        if (post_buf == POSTAMBLE) {
	            // store RX BUFFER to RX FIFO
	        }
	        else {
	            // Postamble error, Data is no good
	            state = WAIT;
	            // deactivate timer
	            // enable gpio
	        }
	    }

	    state = WAIT;
	    break;
	    */
    case WAIT_DELAY:
        CpuTimer0Regs.TCR.bit.TIE = 0;
        Semaphore_post(tx_delay_sem);
        state = WAIT;
        break;
	case TX_PRE:
	    GpioDataRegs.GPADAT.bit.GPIO4 = ((0x80 >> edge_count) & PREAMBLE) && 1;
	    if (edge_count++ >= 7) {
	        state = TX_DATA;
	        edge_count = 0;
	    }
	    break;
	case TX_DATA:
	    GpioDataRegs.GPADAT.bit.GPIO4 = ((0x8000 >> edge_count % 16) & stage_buf[edge_count >> 4]) && 1;
	    if (edge_count++ >= (DG_WORDS-1)*16 + 15) {
	        state = TX_POST;
	        edge_count = 0;
	    }
	    break;
	case TX_POST:
	    GpioDataRegs.GPADAT.bit.GPIO4 = ((0x80 >> edge_count) & POSTAMBLE) && 1;
	    if (edge_count++ >= 8) {
	        GpioDataRegs.GPACLEAR.bit.GPIO4 = 1;
	        CpuTimer0Regs.PRD.all = mSec5;
	        CpuTimer0Regs.TCR.bit.TRB = 1;
	        state = WAIT_DELAY;
	        Semaphore_post(channel);
	    }
	    break;
    }
}

// Transmit setup task
void task_transmit_setup(void)
{
    while (TRUE) {
        // Wait for TX_PHY to have something then wait for the channel to be ready
        Semaphore_pend(tx_phy_sem, BIOS_WAIT_FOREVER);
        Semaphore_pend(tx_delay_sem, BIOS_WAIT_FOREVER);
        Semaphore_pend(channel, BIOS_WAIT_FOREVER);

        // disable GPIO interrupt


        state = TX_PRE;
        // transfer datagram
        read_fifo(TX_PHY, stage_buf);

        // Configure Timer interrupt
        CpuTimer0Regs.PRD.all = mSec0_1;
        CpuTimer0Regs.TCR.bit.TRB = 1;
        CpuTimer0Regs.TCR.bit.TIE = 1;

        edge_count = 0;
    }
}

