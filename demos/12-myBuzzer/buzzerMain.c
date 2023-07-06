#include <msp430.h>
#include "libTimer.h"
#include "buzzer.h"

#define LED_RED BIT6
#define LED_GREEN BIT0
#define LEDS (LED_GREEN | LED_RED )
#define SW1 BIT3

#define TSW1 BIT0 //--confirmed SW1 == BIT0
#define TSW2 BIT1 //--confirmed SW2 == BIT1
#define TSW3 BIT2 //--confirmed SW3 == BIT2
#define TSW4 BIT3 //--confirmed SW4 == BIT3

#define SWITCHES SW1
#define TSWITCHES (TSW1 | TSW2 | TSW3 | TSW4)

void main(void) {
  configureClocks();
  buzzer_init();
  
  WDTCTL = WDTPW | WDTHOLD;     /* stop watchdog timer */

  /*  Set both LEDS to output bits */
  /*  LEDS initially off */
  P1DIR |= LEDS;
  P1OUT &= ~LEDS;

  P1REN |= SWITCHES;		/* enables resistors for switches */
  P1IE  |= SWITCHES;		/* enable interrupts from switches */
  P1OUT |= SWITCHES;		/* pull-ups for switches */
  P1IES |= SWITCHES;            /* Hi/Lo edge */
  P1DIR &= ~SWITCHES;		/* set switches' bits for input */
  P1IFG &= ~SWITCHES;           /* clear just in case */

  //--- configure the top board
  P2DIR &= ~TSWITCHES;
  P2IFG &= ~TSWITCHES;
  P2REN |=  TSWITCHES;
  P2IE  |=  TSWITCHES;
  P2OUT |=  TSWITCHES;
  P2IES |=  TSWITCHES;
  
  or_sr(0x18);
}


void siren_leds(){
  //--- turns on and off the GREEN and RED LED
  int n = 10;
  while(n) {
    P1OUT ^= LEDS;
    __delay_cycles(10000000);
    --n;
  }
  P1OUT &= ~LEDS;
  
}
 
void sleep(double);

void resetPattern(int x){
  
  for (int i=0; i<10; ++ i ) {
    if ( x/(i+1) < 10 ) break;
    buzzer_set_period(x/(i+1));
    sleep(1000);
  }

  for ( int i=0; i<10; ++i ) {
    buzzer_set_period(x/(10-i));
    sleep(1000);
  }
}

void sleep(double n){
  while(n>0){
    n--;
  }
}

void
switch_interrupt_handler()
{
  char p1val = P1IN;		/* switch is in P1 */

  /* update switch interrupt sense to detect changes from current buttons */
  //P1IES |= (p1val & SWITCHES);	/* if switch up, sense down */
  //P1IES &= (p1val | ~SWITCHES);	/* if switch down, sense up */

/* up=red, down=green */
  if ( p1val & SW1 ) {
  } else {
    P1OUT |= LED_RED;
    siren_leds();  
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

 void __interrupt_vec(PORT2_VECTOR) Port_2() {
   if ( P2IFG & TSW1 ) {
     P2IFG &= ~TSW1;
     P1OUT |= LEDS;
     siren_leds();
   } else if ( P2IFG & TSW2 ) {
     P2IFG &= ~TSW2;
     buzzer_set_period(1000);
   } else if ( P2IFG & TSW3 ) {
     P2IFG &= ~TSW3;
     resetPattern(3000);
     buzzer_set_period(0);
   } else if ( P2IFG & TSW4 ) {
     P2IFG &= ~TSW4;
     buzzer_set_period(0);
   }
   // buzzer_set_period(1000); /* start buzzing!!! 2MHz/1000 = 2KHz */           
 }
 
