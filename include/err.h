// Error Detection Layer Interface

// Author: Matthew Knight
// File Name: err.c
// Date: 2017-11-14

// This layer adds an error detection service to the stack. Initially this will
// employ a checksum and later will be improved to a software CRC. It will check
// the checksum of incoming messages and calculate and append the checksum to
// the datagram.

#ifndef ERR_H_
#define ERR_H_

#include "fifo.h"

// interface fifos
struct fifo *RX_ERR, *TX_ERR;

// Initialize Error detection layer
void init_err(void);

// check incoming datagram
void check_datagram(void);

// add checksum to outgoing checksum
void calc_checksum(void);

#endif
