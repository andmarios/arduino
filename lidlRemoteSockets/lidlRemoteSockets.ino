/*
 *  This code uses a 433MHz transmitter to control a wireless socket bought from LIDL.
 *  Currently I only include the transmitter sequences for turning on and off the socket that
 *  corresponds to button D of the remote.
 *  To get the other sequences, you can record them with gqrx, open them in audacity and decode
 *  manually. You only have to care for the duration of each level shift (low or high) and it
 *  is either 500us or 1ms. The sequence starts with a 7ms low and ends with a 3ms high.
 *
 *  A. Marios, 2014
 */

#include <avr/io.h>
#include <util/delay.h>
#define TRANSMITTER_PIN 2

// Sequences:
// D on : 1 0 0 1 1 0 1 0 - 1 0 1 0 0 1 0 1 - 1 0 0 1 0 1 1 0 - 1 0 1 0 0 1 0 1 - 1 0 0 1 0 1 1 0 - 0 1 1 0 1 0 1 0
// D off: 1 0 0 1 1 0 1 0 - 0 1 1 0 1 0 0 1 - 0 1 0 1 1 0 0 1 - 0 1 0 1 0 1 1 0 - 1 0 1 0 1 0 0 1 - 0 1 1 0 1 0 1 0

uint16_t DONseq[] = {7000, \
                     1000, 500, 500, 1000, 1000, 500, 1000, 500, \
                     1000, 500, 1000, 500, 500, 1000, 500, 1000, \
                     1000, 500, 500, 1000, 500, 1000, 1000, 500, \
                     1000, 500, 1000, 500, 500, 1000, 500, 1000, \
                     1000, 500, 500, 1000, 500, 1000, 1000, 500, \
                     500, 1000, 1000, 500, 1000, 500, 1000, 500, \
                     3000};
uint16_t DOFFseq[] = {7000, \
                     1000, 500, 500, 1000, 1000, 500, 1000, 500, \
                     500, 1000, 1000, 500, 1000, 500, 500, 1000, \
                     500, 1000, 500, 1000, 1000, 500, 500, 1000, \
                     500, 1000, 500, 1000, 500, 1000, 1000, 500, \
                     1000, 500, 1000, 500, 1000, 500, 500, 1000, \
                     500, 1000, 1000, 500, 1000, 500, 1000, 500, \
                     3000};

int seqLength = 50;

void setup()
{
  pinMode(TRANSMITTER_PIN, OUTPUT);
}

void loop()
{
  playSignal(DONseq);
  delay(15000);
  playSignal(DOFFseq);
  delay(15000);
}

void playSignal(uint16_t seq[])
{
  int signal = 0;

  for (int i = 0; i < 10; i++) { // repeat sequence 10 times to be sure
    signal = 0;
    for (int j = 0; j < seqLength; j++) {
      digitalWrite(TRANSMITTER_PIN, signal);
      signal = 1 - signal; // Swap signal level
      _delay_us(seq[j]);
    }
  }

  digitalWrite(TRANSMITTER_PIN, LOW);
}
