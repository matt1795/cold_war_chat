// AES Encryption/Decryption Layer Interface

// Authors: Matthew Knight, Andrew Obermeyer
// File Name: aes.h
// Date: 2017-11-30

// This layer encrypts and decrypts incoming and outgoing messages using
// AES-128. This layer is limited to 16-byte messagesd

#ifndef AES_H
#define AES_H

#include "fifo.h"

#define KEY_WORDS 8

// Private Key
extern uint16_t *KEY;

// Message FIFO's
extern struct fifo *TX_AES, *RX_AES;

// initialize the layer
void init_aes(void);

// Encrypt message task
void encrypt(void);

// Decrypt message task
void decrypt(void);

#endif
