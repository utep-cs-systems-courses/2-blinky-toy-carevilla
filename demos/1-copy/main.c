//Alternate LEDs from Off, Green, and Red
#include <msp430.h>
#include "libTimer.h"
#include "led.h"

int main(void) {
  /*
  P1DIR |= LEDS;
  P1OUT &= ~LED_GREEN;  
  P1OUT |= LED_RED;
  */
  
  // P1DIR &= LEDS;          /* clears both bits */
  
  //P1DIR |= LEDS;          /* sets both bits */
  
  //P1DIR |= LED_RED;       /* This turns on the green */
  
  //P1DIR |= LED_GREEN;     /* This turns on the red */

  P1DIR &= 0x00;
  P1OUT &= 0x00;
  P1REN &= 0x00;
  //P1DIR |= LED_GREEN >> 6;
  //P1OUT |= LED_GREEN >> 6;
  //P1DIR |= LED_RED << 6;
  P1OUT |= LED_RED << 6;
  P1DIR |= LED_RED << 6;
  //P1OUT &= ~LED_GREEN;
  //P1OUT |= 1;
  //P1DIR |= 1;
  //P1DIR |= LED_GREEN; 
  //P1OUT |= 1;
  //P1OUT &= ~1;
  //P1OUT |= LED_GREEN;
  //P1OUT |= BIT0;
  //P1DIR |= BIT6;
  //*(char *)0x22 |= 1;
  //or_sr(0x18);		/* CPU off, GIE on */
}
