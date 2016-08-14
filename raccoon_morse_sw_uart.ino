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
 * File Name : raccoon_morse_sw_uart.ino
 *
 * File Description :
 *
 * Author : Joo, Young Jin <neoelec@gmail.com>
 * Dept : Raccoon's Cave
 * Created Date : 17/Aug/2016
 * Version : Baby-Raccoon
 */

#include <avr/pgmspace.h>
#include <morse.h>
#include <stdint.h>
#include <stdio.h>

#include "raccoon_morse.h"
#include "raccoon_morse_sw_uart.h"

class raccoon_keyer_uart : public raccoon_keyer_base {
protected:
	virtual void prologue(void);
	virtual char do_get_char(void);

private:
	unsigned long char_timestamp;
	bool space_is_printed;
};

static raccoon_keyer_uart __raccoon_keyer_uart;

void raccoon_keyer_uart::prologue(void)
{
	raccoon_keyer_base::prologue();

	space_is_printed = true;

	morse_hw->lcd_clear();
}

char raccoon_keyer_uart::do_get_char(void)
{
	unsigned long timestamp = millis();
	unsigned long timeout;
	char char_ret;

	if (!Serial.available()) {
		timeout = timestamp - char_timestamp;
		if (timeout > word_break_ms && !space_is_printed) {
			space_is_printed = true;
			Serial.print(' ');
			morse_hw->lcd_print(' ');
		}
		return '\0';
	}

	char_ret = Serial.read();
	if ('\r' == char_ret || '\n' == char_ret) {
		if (!space_is_printed) {
			space_is_printed = true;
			Serial.println("");
			morse_hw->lcd_print(' ');
		}
		return ' ';
	} else if (' ' == char_ret || '\t' == char_ret) {
		if (!space_is_printed) {
			space_is_printed = true;
			Serial.print(' ');
			morse_hw->lcd_print(' ');
		}
		return ' ';
	} else if ((uint16_t)'e' == morse_char2code(char_ret)) {
		char_timestamp = timestamp;
		space_is_printed = false;
		Serial.print(char_ret);
		morse_hw->lcd_print('e');
		return '\0';
	}

	char_timestamp = timestamp;
	space_is_printed = false;
	Serial.print(char_ret);
	morse_hw->lcd_print(char_ret);

	return char_ret;
}

/* */

static const char __raccoon_morse_sw_uart_name[] PROGMEM = "UART Keyer";

raccoon_morse_sw_uart::raccoon_morse_sw_uart(raccoon_keyer_base *keyer)
{
	this->keyer = keyer;

	name = __raccoon_morse_sw_uart_name;
}

static raccoon_morse_sw_uart __raccoon_morse_sw_uart(&__raccoon_keyer_uart);

raccoon_morse_sw_uart *raccoon_morse_sw_uart::singleton =
		&__raccoon_morse_sw_uart;

raccoon_morse_sw_uart *raccoon_morse_sw_uart::get_instance(void)
{
	return singleton;
}

void raccoon_morse_sw_uart::prologue(void)
{
	raccoon_morse_sw_keyer::prologue();

	Serial.begin(RACCOON_MORSE_SW_UART_BAUD);
	Serial.flush();
}

void raccoon_morse_sw_uart::epilogue(void)
{
	Serial.end();

	raccoon_morse_sw_keyer::epilogue();
}
