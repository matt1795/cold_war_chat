// Transport Layer Implementation

// Author: Matthew Knight
// File Name: transport.c
// Date: 2017-11-14

// The transport layer of this stack provides two services, connection
// establishment and reliable data transfer

#ifndef TRANSPORT_H
#define TRANSPORT_H

#include "fifo.h"

// Interface Pointers
extern struct fifo *RX_TRAN, *TX_TRAN;

// Layer Initialization
void init_transport();

// Will figure out internal layer topology at a later point

#endif
