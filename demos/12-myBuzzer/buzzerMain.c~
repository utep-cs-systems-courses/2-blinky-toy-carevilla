#include <msp430.h>
#include "libTimer.h"
#include "buzzer.h"

#define ST4 BIT3

#define SWITCHES4 ST4

int main(void) {
  configureClocks();
  buzzer_init();
  buzzer_set_period(1000); /* start buzzing!!! 2MHz/1000 = 2KHz */           

  P2REN |= SWITCHES4;
  P2IE |= SWITCHES4;
  P2OUT |= SWITCHES4;
  P2DIR &= ~SWITCHES4;

  buzzer_init();
  or_sr(0x18);  //CPU off, GIE on
}
