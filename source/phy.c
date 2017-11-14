// Physical Layer Implementation

// Author: Matthew Knight
// File Name: phy.c
// Date: 2017-11-06

// These functions implement the physical layer through hardware and software
// interrupts. Internal memory components of the physical layer are static
// variables, the only interface available will be given to the data-link layer.

// The physical layer consists of two GPIO pins on the MCU, (one must be
// interruptible), and the timer1 module. 

#include "phy.h"

// Internal defines
#define PREAMBLE 0xAA
#define POSTAMBLE 0x55

// State of the transciever
static enum State
{
    WAIT,
    RX_PRE,
    RX_DATA,
    RX_POST
} state;

// Static variables
static int edge_count;

// Buffers
static uint16_t stage_buf[DATAGRAM_WORDS];
static uint16_t rx_buf[DATAGRAM_WORDS*BUFLEN]; 
static uint16_t tx_buf[DATAGRAM_WORDS*BUFLEN]; 

// fifo interfaces
static struct fifo rx_fifo, tx_fifo;

// Initialize physical layer - only run once
int init_phy(void)
{
    // initialize fifo objects
    init_fifo(&rx_fifo, &rx_buf, BUFLEN, DATAGRAM_WORDS);
    init_fifo(&tx_fifo, &tx_buf, BUFLEN, DATAGRAM_WORDS);

    // Point assign interface pointers
    RX_FIFO = &rx_fifo;
    TX_FIFO = &tx_fifo;
}

// RX GPIO HWI
void hwi_rx_gpio(void)
{
    //Swi_Post(&swi_rx_gpio);
}

// Timer1 HWI
void hwi_timer(void)
{
    //Swi_Post(&swi_timer);
}

// RX Software Interrupt
void swi_rx_gpio(void)
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
		T_samp[(edge_count - 1)/2];
		// reset timer
	    }
	    else {
		pulse_samp[edge_count/2];
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
void swi_timer(void)
{
    switch (state) {
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
	    break;
    }
}
