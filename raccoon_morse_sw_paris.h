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
 * File Name : raccoon_morse_sw_paris.h
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

#ifndef __RACCOON_MORSE_SW_PARIS_H__
#define __RACCOON_MORSE_SW_PARIS_H__

#include "raccoon_morse_sw_keyer.h"

class raccoon_morse_sw_paris : public raccoon_morse_sw_keyer {
public:
	raccoon_morse_sw_paris(raccoon_keyer_base *keyer);
	static raccoon_morse_sw_paris *get_instance(void);

private:
	static raccoon_morse_sw_paris *singleton;
};

#endif /* __RACCOON_MORSE_SW_PARIS_H__ */
