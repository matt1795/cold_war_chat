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

int gpio_count = 0, timer_count = 0, count = 0;

//  SWI handles
extern const Swi_Handle swi_handle_rx_packet, swi_handle_phy_timer;

// Internal defines
#define PREAMBLE 0xAA
#define POSTAMBLE 0x55

// Preamble Analysis
static uint32_t pulse_width;
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
uint16_t stage_buf[DG_WORDS];
static uint16_t rx_buf[DG_WORDS*(BUFLEN+1)];
static uint16_t tx_buf[DG_WORDS*(BUFLEN+1)];
uint16_t post_buf;

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
    // gpio_count++;
    switch (state) {
        case WAIT_DELAY:
            // Disable Timer interrupt
            CpuTimer0Regs.TCR.bit.TIE = 0;
        case WAIT:
            // Lock down the channel, and free tx from delay
            Semaphore_pend(channel, BIOS_WAIT_FOREVER);
            Semaphore_post(tx_delay_sem);

            // Update new Timer period and reset it
            CpuTimer0Regs.PRD.all = mSec0_1*2;
            CpuTimer0Regs.TCR.bit.TRB = 1;

            // Reverse Edge trigger and update state
            XIntruptRegs.XINT1CR.bit.POLARITY = 0;
            state = RX_PRE;

            // clear variables;
            pulse_width = 0;
            edge_count = 0;
            post_buf = 0;
            int i;
            for(i = 0; i < DG_WORDS; i++)
                stage_buf[i] = 0;
            break;
        case RX_PRE:
            // Detecting edge
            if (edge_count < 4) {
                pulse_width += CpuTimer0Regs.TIM.all;
                edge_count++;
            }
            // Setting up for Datagram Capture
            else {
                // Change up the interrupts
                XIntruptRegs.XINT1CR.bit.ENABLE = 0;
                CpuTimer0Regs.TCR.bit.TIF = 0;
                CpuTimer0Regs.TCR.bit.TIE = 1;

                // Half the Average pulse width
                pulse_width >>= 3;
                edge_count = 0;
            }
            break;
    }
}

// Timer Software Interrupt
void swi_phy_timer(void)
{
    //timer_count++;
    switch (state) {
        case RX_PRE:
            // set up timer for 10kbit/s and measure middle of the pulse
            CpuTimer0Regs.PRD.all = mSec0_1;
            CpuTimer0Regs.TIM.all = pulse_width;
            state = RX_DATA;
            break;
        case RX_DATA:
            stage_buf[edge_count >> 4] |= (0x8000 >> (edge_count % 16)) && GpioDataRegs.GPADAT.bit.GPIO5;

            if (edge_count++ >= (DG_WORDS-1)*16 + 15) {
                state = RX_POST;
                edge_count = 0;
            }

            break;
        case RX_POST:

            post_buf |= (((0x80 >> edge_count)) && GpioDataRegs.GPADAT.bit.GPIO5);

            if (edge_count >= 8) {
                //if (post_buf == POSTAMBLE)
                    write_fifo(RX_PHY, stage_buf);

                // Postamble error, Data is no good
                state = WAIT;

                // deactivate timer and enable GPIO int
                CpuTimer0Regs.TCR.bit.TIE = 0;
                XIntruptRegs.XINT1CR.bit.ENABLE = 1;
            }

            break;
        case WAIT_DELAY:
            // Once TX delay is done, disable timer interrupt, unlock semaphore, and put the layer into the wait state
            CpuTimer0Regs.TCR.bit.TIE = 0;
            Semaphore_post(tx_delay_sem);
            state = WAIT;
            break;
        case TX_PRE:
            // Write Preamble to GPIO pin
            GpioDataRegs.GPADAT.bit.GPIO4 = ((0x80 >> edge_count) & PREAMBLE) && 1;
            if (edge_count++ >= 7) {
                state = TX_DATA;
                edge_count = 0;
            }
            break;
        case TX_DATA:
            // Write data serially to GPIO pin from Stage Buffer
            GpioDataRegs.GPADAT.bit.GPIO4 = ((0x8000 >> edge_count % 16) & stage_buf[edge_count >> 4]) && 1;
            if (edge_count++ >= (DG_WORDS-1)*16 + 15) {
                state = TX_POST;
                edge_count = 0;
            }
            break;
        case TX_POST:
            // Write Postamble to GPIO pin
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
        XIntruptRegs.XINT1CR.bit.ENABLE = 0;

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

