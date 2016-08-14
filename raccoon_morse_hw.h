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
 * File Name : raccoon_morse_hw.h
 *
 * File Description :
 *
 * Author : Joo, Young Jin <neoelec@gmail.com>
 * Dept : Raccoon's Cave
 * Created Date : 14/Aug/2016
 * Version : Baby-Raccoon
 */

#ifndef __RACCOON_MORSE_HW_H__
#define __RACCOON_MORSE_HW_H__

#include <stdint.h>

typedef enum {
	E_RACCOON_MORSE_HW_KEYCODE_RUN = 0,
	E_RACCOON_MORSE_HW_KEYCODE_LEFT,
	E_RACCOON_MORSE_HW_KEYCODE_RIGHT,
	E_RACCOON_MORSE_HW_KEYCODE_UP,
	E_RACCOON_MORSE_HW_KEYCODE_DOWN,
	E_RACCOON_MORSE_HW_KEYCODE_INVALID,
} raccoon_morse_keycode_t;

typedef enum {
	E_RACCOON_MORSE_KEYSTATE_NOT_PRESSED = 0,
	E_RACCOON_MORSE_KEYSTATE_PRESSED,
} raccoon_morse_keystate_t;

class raccoon_morse_hw {
public:
	virtual void init(void);

	virtual raccoon_morse_keystate_t left_key_state(void) = 0;
	virtual raccoon_morse_keystate_t right_key_state(void) = 0;

	virtual raccoon_morse_keystate_t button_state(void) = 0;
	virtual raccoon_morse_keycode_t button_keycode(void) = 0;

	virtual int wpm_raw(void) = 0;

	virtual void dit_led_on(void) = 0;
	virtual void dah_led_on(void) = 0;
	virtual void dit_led_off(void) = 0;
	virtual void dah_led_off(void) = 0;

	virtual void tone_on(void) = 0;
	virtual void tone_off(void) = 0;

	virtual void pulse_on(void) = 0;
	virtual void pulse_off(void) = 0;

	virtual void lcd_clear(void) = 0;
	virtual void lcd_clear_row(uint8_t row) = 0;
	virtual void lcd_set_cursor(uint8_t col, uint8_t row) = 0;
	virtual void lcd_print(char ch) = 0;
	virtual void lcd_print(const char *str) = 0;
	virtual void lcd_print_P(const char *str) = 0;
};

#endif /* __RACCOON_MORSE_HW_H__ */
