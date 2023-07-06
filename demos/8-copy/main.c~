#include <msp430.h>
#include "libTimer.h"

#define LED_RED BIT0               // P1.0
#define LED_GREEN BIT6             // P1.6
#define LEDS (BIT0 | BIT6)
#define SW1 BIT3		/* switch1 is p1.3 */
#define SW2 BIT2
#define SW3 BIT1
#define SW4 BIT0
#define SWITCHES SW1		/* only 1 switch on this board */

void main(void) 
{  
  configureClocks();

  P1DIR &= 0;
  
  P1DIR |= LEDS;
  P1OUT &= ~LEDS;		/* leds initially off */
  
  P1REN |= SWITCHES;		/* enables resistors for switches */
  P1IE |= SWITCHES;		/* enable interrupts from switches */
  P1OUT |= SWITCHES;		/* pull-ups for switches */
  P1DIR &= ~SWITCHES;           /* set switches' bits for input */


  // turn on switch on green board 
  P2REN |= SWITCHES;
  P2IE |= SWITCHES;
  P2OUT |= SWITCHES;
  P2DIR &= ~SWITCHES;

  
  P2OUT |=~LED_GREEN;
  P1OUT |=~LED_GREEN;
  or_sr(0x18);  // CPU off, GIE on
}

// Added switch is released...
void up_event(){
  P1OUT &= ~LED_GREEN;
  P1OUT |= LED_RED;
}

// Added switch is pressed ...
char state = 'G';
void down_event(){
  switch (state) {
  case 'G':
    state = 'R';
    P1OUT |= LED_RED;
    P1OUT &= ~LED_GREEN;
    break;
  case 'R':
    state = 'G';
      P1OUT &= ~LED_RED;
    P1OUT |= LED_GREEN;
  }
}

char greenState = 'G';
void down_Green_board(){
  switch (greenState){
  case 'G':
    greenState = 'R';
    P2OUT |= LED_RED;
    P2OUT &= ~LED_GREEN;
    break;
  case 'R':
    greenState = 'G';
    P2OUT &= ~LED_RED;
    P2OUT |= LED_GREEN;
  }
}

void toggle_led(){
  P1OUT ^= LED_RED;
  P1OUT ^= LED_GREEN;
}

void
switch_interrupt_handler()
{
  char p2val = P2IN;            //switch is in P2 on green
  char p1val = P1IN;		/* switch is in P1 */

/* update switch interrupt sense to detect changes from current buttons */
  P1IES |= (p1val & SWITCHES);	/* if switch up, sense down */
  P1IES &= (p1val | ~SWITCHES);	/* if switch down, sense up */

/* up=red, down=green */
  if ( p1val & SW1 ) {
    //toggle_led();
    //up_event();
  } else {
    down_event();
  }

  if ( p2val ){
    down_Green_board;
  } else {
    // continue;
  }
}


/* Switch on P1 (S2) */
void
__interrupt_vec(PORT1_VECTOR) Port_1(){
  if (P1IFG & SWITCHES) {	      /* did a button cause this interrupt? */
    P1IFG &= ~SWITCHES;		      /* clear pending sw interrupts */
    switch_interrupt_handler();	/* single handler for all switches */
  }
}
