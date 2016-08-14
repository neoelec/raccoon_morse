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
 * File Name : pcint.h
 *
 * File Description : the original implementation found in this url
 *  http://playground.arduino.cc/Main/PinChangeInterrupt
 *
 * Author : Joo, Young Jin <neoelec@gmail.com>
 * Dept : Raccoon's Cave
 * Created Date : 13/Aug/2016
 * Version : Baby-Raccoon
 */

#ifndef __PCINT_H__
#define __PCINT_H__

enum {
	E_PCINT0 = 0,
	E_PCINT1,
	E_PCINT2,
	E_NR_PCINT,
};

int pcint_attach(unsigned int idx, void (*handler)(void));
int pcint_dettach(unsigned int idx);
void pcint_enable(unsigned int pin);
void pcint_disable(unsigned int pin);

#endif /* __PCINT_H__ */
