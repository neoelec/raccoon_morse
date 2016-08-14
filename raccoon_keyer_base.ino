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
 * File Name : raccoon_keyer_base.ino
 *
 * File Description : FSM based keyer module
 *
 * Author : Joo, Young Jin <neoelec@gmail.com>
 * Dept : Raccoon's Cave
 * Created Date : 14/Aug/2016
 * Version : Baby-Raccoon
 */

#include <stdint.h>
#include <morse.h>

#include "raccoon_morse.h"
#include "raccoon_keyer_base.h"

raccoon_keyer_base::raccoon_keyer_base()
		: running(false)
{
}

void raccoon_keyer_base::start(void)
{
	running = true;
}

void raccoon_keyer_base::stop(void)
{
	running = false;
}

void raccoon_keyer_base::loop(void)
{
	prologue();
	while (running)
		text();
	epilogue();
}

void raccoon_keyer_base::prologue(void)
{
	dit_ms = morse_sw->get_dit_ms();
	dah_ms = morse_sw->get_dah_ms();
	dididi_ms = morse_sw->get_dah_ms();
	word_break_ms = morse_sw->get_word_break_ms();
	led_active_ms = morse_sw->get_led_active_ms();

	timestamp = 0;
	timeout = 0;
	pulse_code = 0;
	pulse_count = 0;
	state = E_RACCOON_KEYER_STATE_PULSE_HIGH;
}

void raccoon_keyer_base::text(void)
{
	unsigned long timestamp_curr = millis();
	unsigned long elapsed_time = timestamp_curr - timestamp;

	if (timeout > elapsed_time)
		return;

	timestamp = timestamp_curr;

	switch (state) {
	case E_RACCOON_KEYER_STATE_LED_ON:
		do_at_led_on();
		break;
	case E_RACCOON_KEYER_STATE_PULSE_HIGH:
		do_at_falling_edge();
		break;
	case E_RACCOON_KEYER_STATE_NEW_CHAR:
		do_at_new_char_started();
		break;
	case E_RACCOON_KEYER_STATE_PULSE_LOW:
		do_at_rising_edge();
		break;
	case E_RACCOON_KEYER_STATE_IDLE:
		do_at_idle();
		break;
	}
}

void raccoon_keyer_base::epilogue(void)
{
}

void raccoon_keyer_base::do_at_led_on(void)
{
	if (E_RACCOON_KEYER_PULSE_DIT == pulse_type) {
		morse_hw->dit_led_off();
		timeout = dit_ms - led_active_ms;
	} else { /* if (E_RACCOON_KEYER_PULSE_DAH == pulse_type) { */
		morse_hw->dah_led_off();
		timeout = dah_ms - led_active_ms;
	}

	state = E_RACCOON_KEYER_STATE_PULSE_HIGH;
}

void raccoon_keyer_base::do_at_falling_edge(void)
{
	morse_hw->tone_off();
	morse_hw->pulse_off();

	if (!pulse_count) {
		timeout = dididi_ms;
		state = E_RACCOON_KEYER_STATE_NEW_CHAR;

		pulse_char = do_get_char();
		if (is_word_break()) {
			/* this is a REAL word break */
			state = E_RACCOON_KEYER_STATE_IDLE;
			timeout = word_break_ms;
			return;
		}

		if (!decode_pulse_char()) {
			/* if decode fail, then does not send anything */
			state = E_RACCOON_KEYER_STATE_IDLE;
			return;
		}
	} else {
		timeout = dit_ms;
		state = E_RACCOON_KEYER_STATE_PULSE_LOW;
	}

	update_pulse_type();
}

void raccoon_keyer_base::do_at_new_char_started(void)
{
	timeout = 0;
	state = E_RACCOON_KEYER_STATE_PULSE_LOW;
}

void raccoon_keyer_base::do_at_rising_edge(void)
{
	morse_hw->tone_on();
	morse_hw->pulse_on();

	if (E_RACCOON_KEYER_PULSE_DIT == pulse_type)
		morse_hw->dit_led_on();
	else
		morse_hw->dah_led_on();

	timeout = led_active_ms;
	state = E_RACCOON_KEYER_STATE_LED_ON;
}

void raccoon_keyer_base::do_at_idle(void)
{
	timeout = 0;

	pulse_char = do_get_char();
	if (is_word_break()) {
		state = E_RACCOON_KEYER_STATE_IDLE;
		return;
	} else {
		if (!decode_pulse_char()) {
			state = E_RACCOON_KEYER_STATE_IDLE;
			return;
		}
		state = E_RACCOON_KEYER_STATE_NEW_CHAR;
	}

	update_pulse_type();
}

bool raccoon_keyer_base::is_word_break(void)
{
	return ('\0' == pulse_char || ' ' == pulse_char ||
		    '\n' == pulse_char || '\r' == pulse_char);
}

bool raccoon_keyer_base::decode_pulse_char(void)
{
	pulse_code = morse_char2code(pulse_char);
	if ((uint16_t)'e' == pulse_code)
		return false;

	pulse_count = pulse_code & 0x07;	// extract counter
	pulse_code &= 0xFC00;			// extract code

	return true;
}

void raccoon_keyer_base::update_pulse_type(void)
{
	/* determine a pulse which will be used 'do_at_rising_edge' */
	pulse_type = pulse_code & 0x8000 ?
			E_RACCOON_KEYER_PULSE_DAH : E_RACCOON_KEYER_PULSE_DIT;

	/* update pulse */
	pulse_code = pulse_code << 1;
	pulse_count--;
}
