/**
 * Copyright (C) 2016. Joo, Young Jin <neoelec@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

/**
 * Project Name : Raccoon's Morse Trainer
 *
 * Project Description :
 *
 * Comments : tabstop = 8, shiftwidth = 8, noexpandtab
 */

/**
 * File Name : pcint.ino
 *
 * File Description : the original implementation found in this url
 *  http://playground.arduino.cc/Main/PinChangeInterrupt
 *
 * Author : Joo, Young Jin <neoelec@gmail.com>
 * Dept : Raccoon's Cave
 * Created Date : 13/Aug/2016
 * Version : Baby-Raccoon
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#include "pcint.h"

static void (*__pcint_handler[E_NR_PCINT])(void);

int pcint_attach(unsigned int idx, void (*handler)(void))
{
	if (idx >= E_NR_PCINT)
		return -1;

	__pcint_handler[idx] = handler;

	return 0;
}

int pcint_dettach(unsigned int idx)
{
	if (idx >= E_NR_PCINT)
		return -1;

	__pcint_handler[idx] = NULL;

	return 0;
}

void pcint_enable(unsigned int pin)
{
	*digitalPinToPCMSK(pin) |= bit (digitalPinToPCMSKbit(pin));
	PCIFR |= bit (digitalPinToPCICRbit(pin));
	PCICR |= bit (digitalPinToPCICRbit(pin));
}

void pcint_disable(unsigned int pin)
{
	*digitalPinToPCMSK(pin) &= ~(bit(digitalPinToPCMSKbit(pin)));
	PCIFR &= ~(bit(digitalPinToPCICRbit(pin)));
	PCICR &= ~(bit(digitalPinToPCICRbit(pin)));
}

// handle pin change interrupt for D8 to D13 here
ISR (PCINT0_vect)
{
	if (__pcint_handler[E_PCINT0])
		__pcint_handler[E_PCINT0]();
}

// handle pin change interrupt for A0 to A5 here
ISR (PCINT1_vect)
{
	if (__pcint_handler[E_PCINT1])
		__pcint_handler[E_PCINT1]();
}

// handle pin change interrupt for D0 to D7 here
ISR (PCINT2_vect)
{
	if (__pcint_handler[E_PCINT2])
		__pcint_handler[E_PCINT2]();
}
