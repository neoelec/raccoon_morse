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
 * File Name : raccoon_morse_sw.h
 *
 * File Description :
 *
 * Author : Joo, Young Jin <neoelec@gmail.com>
 * Dept : Raccoon's Cave
 * Created Date : 14/Aug/2016
 * Version : Baby-Raccoon
 */

#ifndef __RACCOON_MORSE_SW_H__
#define __RACCOON_MORSE_SW_H__

#define RACCOON_MORSE_SW_LED_ACTIVE_MS	2

#define RACCOON_MORSE_SW_WPM_MIN	5
#define RACCOON_MORSE_SW_WPM_MAX	40

class raccoon_morse_sw {
public:
	raccoon_morse_sw();

	virtual void handler(void) = 0;
	virtual void update_wpm(unsigned int new_wpm);
	virtual void start(void);
	virtual void stop(void);

	void loop(void);

	static unsigned int get_wpm_from_hw(void);

	const char *get_name(void);
	unsigned int get_wpm(void);
	unsigned long get_dit_ms(void);
	unsigned long get_dah_ms(void);
	unsigned long get_word_break_ms(void);
	unsigned long get_led_active_ms(void);

protected:
	virtual void prologue(void);
	virtual void text(void) = 0;
	virtual void epilogue(void);

	const char *name;
	unsigned int wpm;
	unsigned long dit_ms;
	unsigned long dah_ms;
	unsigned long word_break_ms;
	unsigned long led_active_ms;
	bool running;
};

#endif /* __RACCOON_MORSE_SW_H__ */
