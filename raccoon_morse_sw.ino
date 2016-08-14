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
 * File Name : raccoon_morse_sw.ino
 *
 * File Description :
 *
 * Author : Joo, Young Jin <neoelec@gmail.com>
 * Dept : Raccoon's Cave
 * Created Date : 14/Aug/2016
 * Version : Baby-Raccoon
 */

#include "raccoon_morse.h"
#include "raccoon_morse_sw.h"

raccoon_morse_sw::raccoon_morse_sw()
		: running(false)
{
}

void raccoon_morse_sw::start(void)
{
	running = true;
}

void raccoon_morse_sw::stop(void)
{
	running = false;
}

const char *raccoon_morse_sw::get_name(void)
{
	return name;
}

unsigned int raccoon_morse_sw::get_wpm(void)
{
	return wpm;
}

unsigned long raccoon_morse_sw::get_dit_ms(void)
{
	return dit_ms;
}

unsigned long raccoon_morse_sw::get_dah_ms(void)
{
	return dah_ms;
}

unsigned long raccoon_morse_sw::get_word_break_ms(void)
{
	return word_break_ms;
}

unsigned long raccoon_morse_sw::get_led_active_ms(void)
{
	return led_active_ms;
}

void raccoon_morse_sw::prologue(void)
{
	update_wpm(get_wpm_from_hw());
	led_active_ms = RACCOON_MORSE_SW_LED_ACTIVE_MS;
}

void raccoon_morse_sw::loop(void)
{
	prologue();
	while (running)
		text();
	epilogue();
}

void raccoon_morse_sw::epilogue(void)
{
	morse_hw->dit_led_off();
	morse_hw->dah_led_off();
	morse_hw->tone_off();
	morse_hw->pulse_off();
}

void raccoon_morse_sw::update_wpm(unsigned int new_wpm)
{
	wpm = new_wpm;

	dit_ms = 1200 / wpm;
	dah_ms = 3 * 1200 / wpm;
	word_break_ms = dah_ms + dah_ms + dit_ms;
}

unsigned int raccoon_morse_sw::get_wpm_from_hw(void)
{
	unsigned int raw = morse_hw->wpm_raw();
	uint32_t new_wpm;
	new_wpm = raw * (RACCOON_MORSE_SW_WPM_MAX
			- RACCOON_MORSE_SW_WPM_MIN + 1);
	new_wpm /= 1024;
	new_wpm += RACCOON_MORSE_SW_WPM_MIN;

	return new_wpm;
}
