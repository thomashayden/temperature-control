#ifndef TEMPERATURE_CONTROL_RXTX_H
#define TEMPERATURE_CONTROL_RXTX_H

#include <stdio.h>
#include <sys/time.h>
#include <time.h>
//#include <writingPi.h>

#define PIN_RECEIVE 1
#define PIN_TRANSMIT 2
#define LOW 0
#define HIGH 1

#define NOISE_CHECK_TIMEOUT_MS 2500
#define WAIT_FOR_BUTTON_TIMEOUT_MS 10000
#define LISTEN_TIMEOUT_MS 1000

/**
 * Setup GPIO pins and other one time operations.
 */
void setup();

int read_rx();

void write_tx(const int value);

/**
 * Listen for a code sent from the remote and determine how to replicate it.
 */
void find_remote_code();

#endif //TEMPERATURE_CONTROL_RXTX_H
