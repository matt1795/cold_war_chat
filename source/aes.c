// AES Encryption/Decryption Layer Implementation

// Authors: Matthew Knight, Andrew Obermeyer
// File Name: aes.c
// Date: 2017-11-30

// This layer encrypts and decrypts incoming and outgoing messages using
// AES-128. This layer is limited to 16-byte messagesd

#include "global_def.h"
#include "aes.h"
#include "transport.h"

// Buffers
static uint16_t rx_buf[MSG_WORDS*(BUFLEN + 1)];
static uint16_t tx_buf[MSG_WORDS*(BUFLEN + 1)];
static uint16_t key_buf[KEY_WORDS];

// FIFO Interfaces
static struct fifo rx_fifo, tx_fifo;

// initialize the layer
void init_aes(void)
{
    // Initialize FIFO Objects
    init_fifo(&rx_fifo, rx_buf, BUFLEN, MSG_WORDS);
    init_fifo(&tx_fifo, tx_buf, BUFLEN, MSG_WORDS);

    // Initialize Interface Pointers
    RX_AES = &rx_fifo;
    TX_AES = &tx_fifo;
    KEY = key_buf;
}

// Encrypt message task
void encrypt(void)
{
    // Read unencrypted message from AES transmit buffer
    
    // DOOOOO IT

    // Write encrypted message to transport transmit buffer
}

// Decrypt message task
void decrypt(void)
{
    // Read encrypted message from transport recieve buffer
    
    // DOOOOO IT

    // Write decrypted message to AES recieve buffer
}

