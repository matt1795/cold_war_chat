# Cold War Chat Room

Authors: Matthew Knight, Andrew Obermeyer

This is a project to develop an encrypted wireless chat client. It is intended
to only have two users at a time, providing point to point communication between
russian spies.

The project is for our class project for Real Time Embedded Systems at BCIT.

Target DSP: TMS320F28027

Under the hood, we have TX and RX modules transmitting on the 433MHz frequency
band. These modules are bare bones quality, susceptible to interference, and
provide a serial port like interface. We will only be using one channel so we
need to implement half duplex communication. Initially only fixed frames will be
used, (eg. 1packet back and forth). And later (if we somehow have time) we may
implement variable frame sizing to best utilize bandwidth.

## Stack

<center>
    
    |:-----------:|
    | Application |
    | Encryption |
    | Transport |
    | Error Detection |
    | Physical |

</center>

## Stages of Development:

These stages can be worked on in parallel

### Wireless Link

1. Design Software Interface
2. [Characterize TX/RX](docs/tx_rx.md)
3. [Hardware Design](docs/hardware_design.md)
4. [Packet Framing](docs/packet_framing.md)
5. Error Detection
6. Reliable Data Transfer
7. Connection Establishment
8. Interface Functions
9. Variable packet size

### Encryption

1. Design Software Interface
2. Pseudorandom Generator
3. RSA Implementation

### Chat Client

1. Design Display - We need red text for the motherland
2. Application Layer Connection
    - Establish Link Layer Connection
    - Trade Public Keys
    - Trade User information
3. Application Close Connection
