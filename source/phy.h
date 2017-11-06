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

#include "stdint.h"
#include "global_def.h"

// Semaphore controling whether RX or TX has channel
int sem_channel;

// FIFO pointers
uint16_t *rx_buf;
uint16_t *tx_buf;

// Initialize physical layer
int init_phy(void);

// RX GPIO HWI
void hwi_rx_gpio(void);

// Timer1 HWI
void hwi_timer(void);

// RX Software Interrupt
void swi_rx_gpio(void);

// Timer Software Interrupt
void swi_timer(void);

#endif
