/*
 *  This code uses a 433MHz transmitter to control a wireless socket bought from LIDL.
 *  Currently I only include the transmitter sequences for turning on and off the socket that
 *  corresponds to button D of the remote.
 *  I have recorded the other sequences but haven't test them. If you want to test them, you have
 *  to convert them like DONseq and DOFFseq. Zeroes translate to 500, ones to 1000. You start with
 *  7000 and end with 3000.
 *
 *  To get the sequences from the remote, you can record them with gqrx, open them in audacity and
 *  decode manually. You only have to care for the duration of each level shift (low or high) and
 *  it is either 500us or 1ms. The sequence starts with a 7ms low and ends with a 3ms high.
 *  Another way to get them would be with a logic analyzer and a 433.92MHz receiver.
 *  The most cheap way would be an Arduino, a 433.92MHz receiver and some code.
 *
 *  A. Marios, 2014
 */

#include <avr/io.h>
#include <util/delay.h>
#define TRANSMITTER_PIN 2

// Sequences:
// A on : 1 0 0 1 1 0 1 0 - 1 0 1 0 1 0 0 1 - 1 0 0 1 1 0 1 0 - 1 0 0 1 0 1 1 0 - 0 1 1 0 0 1 1 0 - 1 0 1 0 0 1 0 1
// A off: 1 0 0 1 1 0 1 0 - 0 1 0 1 0 1 1 0 - 1 0 1 0 1 0 0 1 - 0 1 0 1 1 0 0 1 - 1 0 0 1 0 1 0 1 - 1 0 1 0 0 1 0 1
// B on : 1 0 0 1 1 0 1 0 - 0 1 1 0 1 0 0 1 - 0 1 0 1 1 0 0 1 - 0 1 0 1 0 1 1 0 - 1 0 1 0 1 0 0 1 - 0 1 1 0 0 1 1 0
// B off: 1 0 0 1 1 0 1 0 - 1 0 1 0 0 1 0 1 - 1 0 0 1 0 1 1 0 - 1 0 1 0 0 1 0 1 - 1 0 0 1 0 1 1 0 - 0 1 1 0 0 1 1 0
// C on : 1 0 0 1 1 0 1 0 - 1 0 1 0 1 0 1 0 - 1 0 1 0 1 0 1 0 - 0 1 1 0 1 0 0 1 - 0 1 0 1 1 0 0 1 - 1 0 1 0 1 0 0 1
// C off: 1 0 0 1 1 0 1 0 - 0 1 0 1 0 1 0 1 - 0 1 1 0 1 0 1 0 - 0 1 1 0 0 1 1 0 - 1 0 0 1 1 0 0 1 - 1 0 1 0 1 0 0 1
// D on : 1 0 0 1 1 0 1 0 - 1 0 1 0 0 1 0 1 - 1 0 0 1 0 1 1 0 - 1 0 1 0 0 1 0 1 - 1 0 0 1 0 1 1 0 - 0 1 1 0 1 0 1 0
// D off: 1 0 0 1 1 0 1 0 - 0 1 1 0 1 0 0 1 - 0 1 0 1 1 0 0 1 - 0 1 0 1 0 1 1 0 - 1 0 1 0 1 0 0 1 - 0 1 1 0 1 0 1 0
// * on : 1 0 0 1 1 0 1 0 - 0 1 1 0 1 0 1 0 - 1 0 1 0 0 1 0 1 - 0 1 0 1 0 1 0 1 - 0 1 1 0 1 0 1 0 - 0 1 0 1 1 0 0 1
// * off: 1 0 0 1 1 0 1 0 - 0 1 0 1 1 0 1 0 - 0 1 0 1 1 0 1 0 - 1 0 0 1 1 0 0 1 - 0 1 1 0 1 0 0 1 - 0 1 0 1 1 0 0 1

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
