#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "uart.h"

volatile bool interrupt = false;

ISR(PCINT0_vect) {
  interrupt = true;
}

void
uart_puts(const char *str) {
  while (*str) {
    TxByte(*str++);
  }
}

/**
 * GPIO ports:
 * PB0 (pin 5) - UART Tx;
 * PB3 (pin 2) - Alarm Interruption Pin;
 */
int main(void) {
  // Setup UART. Set port to HIGH to signify UART default condition.
  DDRB |= _BV(DDB0);
  PORTB |= _BV(DDB0);

  // Setup external interrupt pin.
  DDRB &= ~_BV(DDB3);
  PCMSK |= _BV(PCINT3);
  GIMSK |= _BV(PCIE);

  sei();

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
  while (1) {
    if (interrupt) {
      uart_puts("INTERRUPT");
      interrupt = false;
    } else {
      uart_puts("NO INTERRUPT");
      _delay_ms(1000);
    }
  }
#pragma clang diagnostic pop
}