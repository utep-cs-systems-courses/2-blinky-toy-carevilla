#include <msp430.h>
#include "libTimer.h"
#include "buzzer.h"

#define SW1 BIT3                /* switch1 is p1.3 */
#define SW2 BIT2
#define SW3 BIT1
#define SW4 BIT0

#define SWITCHES SW1         /* only 1 switch on this board */
#define SWITCHES1 SW3

#define SWITCHES2 SW2       // S3 on green board !! //

void toggle() {
  buzzer_set_period(0);	/* start buzzing!!! 2MHz/1000 = 2kHz*/
}

void
switch_interrupt_handler() {
  char p1val = P1IN;		/* switch is in P1 */
  char p2val = P2IN;
  // char p3val = P3IN;

/* update switch interrupt sense to detect changes from current buttons */
  P1IES |= (p1val & SWITCHES);	/* if switch up, sense down */
  P1IES &= (p1val | ~SWITCHES);	/* if switch down, sense up */

/* up=red, down=green */
  if ( p1val & SW1 ) {
    toggle();
  } 
}

int
 main() {
    configureClocks();
    enableWDTInterrupts();
    
    P1REN |= SWITCHES;		/* enables resistors for switches */
    P1IE |= SWITCHES;		/* enable interrupts from switches */
    P1OUT |= SWITCHES;		/* pull-ups for switches */
    P1DIR &= ~SWITCHES;		/* set switches' bits for input */

    P2REN |= SWITCHES2;
    P2IE |= SWITCHES2;
    P2OUT |= SWITCHES2;
    P2DIR &= ~SWITCHES2;

    //   P3REN |= SWITCHES3;
    // P3IE |=SWITCHES3;
    // P3OUT |= SWITCHES3;
    //P3DIR &= ~SWITCHES3;
 
    buzzer_init();
    //buzzer_set_period(1000);	/* start buzzing!!! 2MHz/1000 = 2kHz*/
    //buzzer_set_period(500);
    //buzzer_set_period(0);
    or_sr(0x18);          // CPU off, GIE on
}

void state_1_Actions(){
  

  
}




/* Switch on P1 (S2) */
void
__interrupt_vec(PORT1_VECTOR) Port_1(){
  if (P1IFG & SWITCHES) {	      /* did a button cause this interrupt? */
    P1IFG &= ~SWITCHES;		      /* clear pending sw interrupts */
    switch_interrupt_handler();	/* single handler for all switches */
  }
}
//global state vars that control buzzer frequency
int seqLimit = 8;  //duty cycle = 1/seqLimit
int seqMax = 10;
int seqCount = 0;  //cycles 0...seqLimit
int secCount = 0;  //state var representing repeating time 0...1s
int scaleUpD = 1; 

void
__interrupt_vec(WDT_VECTOR) WDT()	/* 250 interrupts/sec */
{
  // handle scale 
  seqCount += scaleUpD;
  if (seqCount > seqLimit ) { // on for 1 interrupt period
    seqCount = 0;
    buzzer_set_period(0);
    scaleUpD *= -1;
  } else {
    buzzer_set_period(3076+scaleUpD*15*seqCount);
  }
  
  // measure a second
  secCount ++;
  if (secCount >= 250) {  // once each second
    secCount = 0;
    seqLimit ++;	     // reduce duty cycle
    if (seqLimit >= 8)     // but don't let duty cycle go below 1/7.
      seqLimit = 0;
  }

}
