// Physical Layer Interface

// Author: Matthew Knight
// File Name: phy.c
// Date: 2017-11-06

// These functions implement the physical layer through hardware and software
// interrupts. Internal memory components of the physical layer are static
// variables, the only interface available will be given to the data-link layer.

// The physical layer consists of two GPIO pins on the MCU, (one must be
// interruptible), and the timer1 module. 

#ifndef PHY_H
#define PHY_H

#include <ti/sysbios/knl/Semaphore.h>
#include "fifo.h"


// Semaphores
extern const Semaphore_Handle channel, tx_phy_sem, rx_phy_sem, tx_delay_sem;


extern uint16_t post_buf;
extern uint16_t stage_buf[DG_WORDS];

// FIFO pointers
extern struct fifo *RX_PHY, *TX_PHY;

// Initialize physical layer
void init_phy(void);

// RX GPIO HWI
void hwi_rx_packet(void);

// Timer1 HWI
void hwi_timer(void);

// RX Software Interrupt
void swi_rx_packet(void);

// Timer Software Interrupt
void swi_phy_timer(void);

// transmit setup task
void task_transmit_setup(void);

#endif
