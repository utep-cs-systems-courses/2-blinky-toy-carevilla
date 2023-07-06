//Alternate LEDs from Off, Green, and Red
#include <msp430.h>
#include "libTimer.h"
#include "led.h"

int main(void) {
  P1DIR |= LEDS;
  P1OUT &= ~LED_GREEN;
  P1OUT |= LED_RED;

  configureClocks();		/* setup master oscillator, CPU & peripheral clocks */
  enableWDTInterrupts();	/* enable periodic interrupt */
  
  or_sr(0x18);		/* CPU off, GIE on */
}

char led_on = 0;
int oncePerSecond() {              /* called once per second */
  led_on = !led_on;
  if (led_on)                  /* toggle green LED on & off once per second */
    P1OUT |= LED_GREEN;
  else
    P1OUT &= ~LED_GREEN;
}

int secondCount = 0;
void
__interrupt_vec(WDT_VECTOR) WDT()	/* 250 interrupts/sec */
{
  secondCount++;
  if (secondCount == 250) {
    oncePerSecond();
    secondCount = 0;
  }
  P1OUT |= LED_GREEN;
} 

