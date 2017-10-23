# Hardware Design

Transmitter power is varied by changing voltage supply. We shall keep it to 3.3V
in order to keep things simple. If we do have range issues then we will look
into bosting power. The intput of the transmitter will simply be driven by a
general GPIO.

As seen in our preliminary measurements into the TX and RX modules, there is a
noticeable drop from the voltage supplied to the reciever and its output voltage
(~2V). The output will have to be wired into an internal comparater found on the
target DSP. The  output of this comparator needs to be able to activate a
hardware interrupt.
