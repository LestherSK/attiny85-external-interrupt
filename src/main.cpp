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

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
  while (1) {
    if (interrupt) {
      uart_puts("INTERRUPT has occurred");
      interrupt = false;

      // Let's wait 5000 ms before going sleep again. Just wanted to have enough time to measure current in
      // non-sleep mode :).
      _delay_ms(5000);
    } else {
      uart_puts("NO INTERRUPT");
    }

    uart_puts("Sleeping...");

    PORTB &= ~_BV(DDB0);

    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    cli();
    sleep_enable();
    sei();
    sleep_cpu();
    sleep_disable();
    sei();

    PORTB |= _BV(DDB0);

    uart_puts("Waking!");
  }
#pragma clang diagnostic pop
}