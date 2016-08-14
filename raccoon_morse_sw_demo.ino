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
 * File Name : raccoon_morse_sw_demo.h
 *
 * File Description : this sw module emulate 'Arduino Morse Shield' found in
 *   this url
 *   http://www.instructables.com/id/Arduino-Morse-Code-Shield/
 *
 * Author : Joo, Young Jin <neoelec@gmail.com>
 * Dept : Raccoon's Cave
 * Created Date : 14/Aug/2016
 * Version : Baby-Raccoon
 */

#include <avr/pgmspace.h>
#include <stdio.h>

#include "raccoon_morse.h"
#include "raccoon_morse_sw_demo.h"

typedef enum {
	E_RACCOON_MORSE_SW_DEMO_LETTER = 0,
	E_RACCOON_MORSE_SW_DEMO_NUMERAL,
	E_RACCOON_MORSE_SW_DEMO_PUNCTUATION,
	E_RACCOON_MORSE_SW_DEMO_RANDOM,
	E_RACCOON_MORSE_SW_DEMO_NR_MODE,
} raccoon_morse_demo_mode_t;

class raccoon_keyer_demo : public raccoon_keyer_base {
protected:
	virtual void prologue(void);
	virtual char do_get_char(void);
	virtual void do_at_new_char_started(void);
	virtual void do_at_rising_edge(void);

private:
	raccoon_morse_demo_mode_t mode;
	bool mode_changed;
	unsigned int mode_offset;
	unsigned int idx, idx_lcd;
	bool first_run;
};

static raccoon_keyer_demo __raccoon_keyer_demo;

void raccoon_keyer_demo::prologue(void)
{
	raccoon_keyer_base::prologue();

	dididi_ms = 2000;	/* wait 2sec between characters */
	word_break_ms = 5000;	/* wait 5sec between mode */

	mode = E_RACCOON_MORSE_SW_DEMO_LETTER;
	mode_changed = true;
	idx = 0;
	first_run = true;
}

static const char __demo_letter_name[] PROGMEM = "Letters";
static const char __demo_numeral_name[] PROGMEM = "Numerals";
static const char __demo_punctuation_name[] PROGMEM = "Punctuations";
static const char __demo_random_name[] PROGMEM = "Random";
static const char __demo_char[] PROGMEM =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"0123456789"
		".,?'!/()&:;=+-_\"$@]ei[uw";

#define RACCOON_MORSE_DEMO_NR_LETTER		('Z' - 'A' + 1)

#define RACCOON_MORSE_DEMO_NR_NUMERAL		('9' - '0' + 1)
#define RACCOON_MORSE_DEMO_OFFSET_NUMERAL	\
		(RACCOON_MORSE_DEMO_NR_LETTER)

#define RACCOON_MORSE_DEMO_NR_PUNCTUATION	((ARRAY_SIZE(__demo_char) - 1) - \
		RACCOON_MORSE_DEMO_NR_LETTER - RACCOON_MORSE_DEMO_NR_NUMERAL)
#define RACCOON_MORSE_DEMO_OFFSET_PUNCTUATION	\
		(RACCOON_MORSE_DEMO_OFFSET_NUMERAL + RACCOON_MORSE_DEMO_NR_NUMERAL)

#define RACCOON_MORSE_DEMO_NR_RANDOM		50
#define RACCOON_MORSE_DEMO_OFFSET_RANDOM \
		(RACCOON_MORSE_DEMO_OFFSET_PUNCTUATION + RACCOON_MORSE_DEMO_NR_PUNCTUATION)

#define RACCOON_MORSE_DEMO_OFFSET_LETTER	\
		(RACCOON_MORSE_DEMO_OFFSET_RANDOM + RACCOON_MORSE_DEMO_NR_RANDOM)

char raccoon_keyer_demo::do_get_char(void)
{
	char char_ret;
	const char *mode_name[] = {
		__demo_letter_name,
		__demo_numeral_name,
		__demo_punctuation_name,
		__demo_random_name
	};
	unsigned int idx_offset[] = {
		0,
		RACCOON_MORSE_DEMO_OFFSET_NUMERAL,
		RACCOON_MORSE_DEMO_OFFSET_PUNCTUATION,
		RACCOON_MORSE_DEMO_OFFSET_RANDOM,
	};

	if (mode_changed) {
		/* FIXME: not a good idea */
		if (!first_run)
			delay(dididi_ms);
		morse_hw->lcd_clear();
		morse_hw->lcd_print_P(mode_name[mode]);
		mode_changed = false;
		first_run = false;
		return ' ';	/* word break */
	}

	if (idx < RACCOON_MORSE_DEMO_OFFSET_RANDOM)
		char_ret = pgm_read_byte(&__demo_char[idx]);
	else
		char_ret = pgm_read_byte(&__demo_char[random(ARRAY_SIZE(__demo_char))]);

	idx_lcd = idx++ - idx_offset[mode];

	if (RACCOON_MORSE_DEMO_OFFSET_LETTER == idx) {
		idx = 0;
		mode_changed = true;
		mode = E_RACCOON_MORSE_SW_DEMO_LETTER;
	} else if (RACCOON_MORSE_DEMO_OFFSET_NUMERAL == idx) {
		mode_changed = true;
		mode = E_RACCOON_MORSE_SW_DEMO_NUMERAL;
	} else if (RACCOON_MORSE_DEMO_OFFSET_PUNCTUATION == idx) {
		mode_changed = true;
		mode = E_RACCOON_MORSE_SW_DEMO_PUNCTUATION;
	} else if (RACCOON_MORSE_DEMO_OFFSET_RANDOM == idx) {
		mode_changed = true;
		mode = E_RACCOON_MORSE_SW_DEMO_RANDOM;
	}

	return char_ret;
}

void raccoon_keyer_demo::do_at_new_char_started(void)
{
	char buf[8];

	raccoon_keyer_base::do_at_new_char_started();

	/* CHAR : A
	 * CODE : ._
	 */
	morse_hw->lcd_clear();
	morse_hw->lcd_print("CHAR : ");
	sprintf(buf, "%c [%u]", pulse_char, idx_lcd);
	morse_hw->lcd_print(buf);
	morse_hw->lcd_set_cursor(0, 1);
	morse_hw->lcd_print("CODE : ");
}

void raccoon_keyer_demo::do_at_rising_edge(void)
{
	char pulse;

	raccoon_keyer_base::do_at_rising_edge();

	if (E_RACCOON_KEYER_PULSE_DIT == pulse_type)
		pulse = '.';
	else /* if (E_RACCOON_KEYER_PULSE_DIT == pulse_type) */
		pulse = '_';

	morse_hw->lcd_print(pulse);
}

/* */

static const char __raccoon_morse_sw_demo_name[] PROGMEM = "Morse DEMO";

raccoon_morse_sw_demo::raccoon_morse_sw_demo(raccoon_keyer_base *keyer)
{
	this->keyer = keyer;

	name = __raccoon_morse_sw_demo_name;
}

static raccoon_morse_sw_demo __raccoon_morse_sw_demo(&__raccoon_keyer_demo);

raccoon_morse_sw_demo *raccoon_morse_sw_demo::singleton =
		&__raccoon_morse_sw_demo;

raccoon_morse_sw_demo *raccoon_morse_sw_demo::get_instance(void)
{
	return singleton;
}
