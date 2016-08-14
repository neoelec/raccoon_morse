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
 * File Name : raccoon_morse_sw_uart.h
 *
 * File Description :
 *
 * Author : Joo, Young Jin <neoelec@gmail.com>
 * Dept : Raccoon's Cave
 * Created Date : 17/Aug/2016
 * Version : Baby-Raccoon
 */

#ifndef __RACCOON_MORSE_SW_UART_H__
#define __RACCOON_MORSE_SW_UART_H__

#include "raccoon_morse_sw_keyer.h"

#define RACCOON_MORSE_SW_UART_BAUD	9600

class raccoon_morse_sw_uart : public raccoon_morse_sw_keyer {
public:
	raccoon_morse_sw_uart(raccoon_keyer_base *keyer);
	static raccoon_morse_sw_uart *get_instance(void);

protected:
	virtual void prologue(void);
	virtual void epilogue(void);

private:
	static raccoon_morse_sw_uart *singleton;
};

#endif /* __RACCOON_MORSE_SW_UART_H__ */
