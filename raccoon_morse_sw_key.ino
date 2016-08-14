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
 * File Name : raccoon_morse_sw_key.ino
 *
 * File Description : this sw module emulate 'Magic Morse on Arduino'
 * found in this url
 *   https://create.arduino.cc/projecthub/rayburne/magic-morse-on-arduino-f48633
 * The Magic-Morse algorithm is rewriten for this module and found in here
 *   https://github.com/neoelec/morse
 *
 * Author : Joo, Young Jin <neoelec@gmail.com>
 * Dept : Raccoon's Cave
 * Created Date : 14/Aug/2016
 * Version : Baby-Raccoon
 */

#include <avr/pgmspace.h>
#include <stdint.h>

#include "raccoon_morse.h"
#include "raccoon_morse_sw_key.h"

static const char __raccoon_morse_sw_key_name[] PROGMEM = "Morse Key";

raccoon_morse_sw_key::raccoon_morse_sw_key()
{
	name =__raccoon_morse_sw_key_name;
}

static raccoon_morse_sw_key __raccoon_morse_sw_key;

raccoon_morse_sw_key *raccoon_morse_sw_key::singleton =
		&__raccoon_morse_sw_key;

raccoon_morse_sw_key *raccoon_morse_sw_key::get_instance(void)
{
	return singleton;
}

void raccoon_morse_sw_key::clear_data(void)
{
	magic_available = false;
	magic_code = 0;
	magic_count = 0;

	left_key_state = E_RACCOON_MORSE_KEYSTATE_NOT_PRESSED;
	left_key_pressed_ms = 0;
	left_key_released_ms = 0;

	space_is_printed = true;
}

void raccoon_morse_sw_key::prologue(void)
{
	raccoon_morse_sw::prologue();

	morse_hw->lcd_clear();

	clear_data();
}

void raccoon_morse_sw_key::epilogue(void)
{
	clear_data();

	raccoon_morse_sw::epilogue();
}

void raccoon_morse_sw_key::text(void)
{
	unsigned long timestamp = millis();
	unsigned long pulse_low_width;
	uint16_t code;
	char magic_char;

	if (E_RACCOON_MORSE_KEYSTATE_PRESSED == left_key_state)
		return;

	if (timestamp - left_key_released_ms > led_active_ms) {
		morse_hw->dit_led_off();
		morse_hw->dah_led_off();
	}

	pulse_low_width = timestamp - left_key_released_ms;

	if (magic_available && pulse_low_width >= di_di_di_ms) {
		code = (magic_code << (16 - magic_count)) | magic_count;
		magic_char = morse_code2char(code);
		if ('\0' != magic_char) {
			morse_hw->lcd_print(magic_char);
		}
		magic_code = 0;
		magic_count = 0;
		magic_available = false;
		space_is_printed = false;
	}

	if (!space_is_printed && pulse_low_width >= word_break_ms) {
		morse_hw->lcd_print(' ');
		space_is_printed = true;
	}
}

void raccoon_morse_sw_key::handler(void)
{
	unsigned long timestamp = millis();
	unsigned long pulse_high_width;

	left_key_state = morse_hw->left_key_state();
	if (E_RACCOON_MORSE_KEYSTATE_PRESSED == left_key_state) {
		left_key_pressed_ms = timestamp;
		morse_hw->tone_on();
		morse_hw->pulse_on();
	} else {
		left_key_released_ms = timestamp;
		pulse_high_width = left_key_released_ms -
			left_key_pressed_ms;
		if (pulse_high_width < half_dah_ms) {		/* DIT */
			morse_hw->dit_led_on();
			magic_code = magic_code << 1;
			magic_count++;
			magic_available = true;
		} else if (pulse_high_width < dit_dah_ms) {	/* DAH */
			morse_hw->dah_led_on();
			magic_code = (magic_code << 1) | 1;
			magic_count++;
			magic_available = true;
		}
		morse_hw->tone_off();
		morse_hw->pulse_off();
	}
}

void raccoon_morse_sw_key::update_wpm(unsigned int wpm)
{
	raccoon_morse_sw::update_wpm(wpm);

	half_dah_ms = dah_ms / 2;
	dit_dah_ms = dit_ms + dah_ms;
	di_di_di_ms = dit_ms + half_dah_ms;
	word_break_ms = dit_dah_ms + di_di_di_ms;
}
