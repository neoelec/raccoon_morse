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
 * File Name : raccoon_morse_sw_paris.ino
 *
 * File Description : this sw module emulate 'Magic Morse on Arduino'
 * found in this url
 *   https://create.arduino.cc/projecthub/rayburne/magic-morse-on-arduino-f48633
 * This module only repeats 'PARIS' and this is a similar operation as
 * 'Magic Morse on Arduin' when 'forcePARISpin' is on.
 *
 * Author : Joo, Young Jin <neoelec@gmail.com>
 * Dept : Raccoon's Cave
 * Created Date : 14/Aug/2016
 * Version : Baby-Raccoon
 */

#include <avr/pgmspace.h>
#include <stdio.h>

#include "raccoon_morse.h"
#include "raccoon_morse_sw_paris.h"

class raccoon_keyer_paris : public raccoon_keyer_base {
protected:
	virtual void prologue(void);
	virtual char do_get_char(void);
	virtual void do_at_falling_edge(void);
	virtual void do_at_new_char_started(void);

private:
	void lcd_clear_paris(void);

	bool is_word_end;
	size_t idx;
};

static const char paris_str[] PROGMEM = "PARIS";

static raccoon_keyer_paris __raccoon_keyer_paris;

void raccoon_keyer_paris::prologue(void)
{
	unsigned int wpm;
	char buf[8];

	raccoon_keyer_base::prologue();

	is_word_end = false;
	idx = 0;

	morse_hw->lcd_clear();
	morse_hw->lcd_print("WPM  : ");
	wpm = morse_sw->get_wpm();
	sprintf(buf, "%2u", wpm);
	morse_hw->lcd_print(buf);

	lcd_clear_paris();
}

char raccoon_keyer_paris::do_get_char(void)
{
	char ch = pgm_read_byte(&paris_str[idx++]);

	if ('\0' == ch) {
		is_word_end = true;
		idx = 0;
	}

	return ch;
}

void raccoon_keyer_paris::do_at_falling_edge(void)
{
	raccoon_keyer_base::do_at_falling_edge();

	/* a new word is started */
	if (is_word_end) {
		is_word_end = false;
		lcd_clear_paris();
	}
}

void raccoon_keyer_paris::do_at_new_char_started(void)
{
	raccoon_keyer_base::do_at_new_char_started();

	morse_hw->lcd_print(pulse_char);
}

void raccoon_keyer_paris::lcd_clear_paris(void)
{
	morse_hw->lcd_set_cursor(0, 1);
	morse_hw->lcd_print("CHAR :          ");
	morse_hw->lcd_set_cursor(7, 1);
}

/* */

static const char __raccoon_morse_sw_paris_name[] PROGMEM = "Repeat PARIS";

raccoon_morse_sw_paris::raccoon_morse_sw_paris(raccoon_keyer_base *keyer)
{
	this->keyer = keyer;

	name = __raccoon_morse_sw_paris_name;
}

static raccoon_morse_sw_paris __raccoon_morse_sw_paris(&__raccoon_keyer_paris);

raccoon_morse_sw_paris *raccoon_morse_sw_paris::singleton =
		&__raccoon_morse_sw_paris;

raccoon_morse_sw_paris *raccoon_morse_sw_paris::get_instance(void)
{
	return singleton;
}
