.data
n:	.word 10

.text
	
	mov #1 , &r1
	mov &n , (1)r1
	
	top
	cmp.b #0 , (1)r1
	goto out	 /* if carry flag on go to exit while */

	xor &P1OUT , &LEDS
	call  #__delay_cycles(1000000)
//	sub #1, &n
	sub #1 , (1)r1
	goto top
	
	out
	bic &P1OUT , ~LEDS
	pop r1 , r0
	add #1 , r1
