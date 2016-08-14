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
 * File Name : raccoon_morse_sw_key.h
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

#ifndef __RACCOON_MORSE_SW_KEY_H__
#define __RACCOON_MORSE_SW_KEY_H__

#include <stdint.h>

#include "raccoon_morse_sw.h"

class raccoon_morse_sw_key : public raccoon_morse_sw {
public:
	raccoon_morse_sw_key();
	static raccoon_morse_sw_key *get_instance(void);

	virtual void handler(void);
	virtual void update_wpm(unsigned int wpm);

protected:
	virtual void prologue(void);
	virtual void text(void);
	virtual void epilogue(void);

private:
	void clear_data(void);

	static raccoon_morse_sw_key *singleton;

	bool magic_available;
	uint16_t magic_code;
	uint16_t magic_count;

	raccoon_morse_keystate_t left_key_state;
	unsigned long left_key_pressed_ms;
	unsigned long left_key_released_ms;

	unsigned long half_dah_ms;
	unsigned long dit_dah_ms;
	unsigned long di_di_di_ms;

	bool space_is_printed;
};

#endif /* __RACCOON_MORSE_SW_KEY_H__ */
