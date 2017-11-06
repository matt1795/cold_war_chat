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

// Staging buffer

// Initialize physical layer
int init_phy(void)
{

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

	case RX_PRE:

    }
    
    // toggle gpio 
}

// Timer Software Interrupt
void swi_timer(void)
{
    switch (state) {

    }
}
