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

#ifndef __RACCOON_MORSE_SW_DEMO_H__
#define __RACCOON_MORSE_SW_DEMO_H__

#include "raccoon_morse_sw_keyer.h"

class raccoon_morse_sw_demo : public raccoon_morse_sw_keyer {
public:
	raccoon_morse_sw_demo(raccoon_keyer_base *keyer);
	static raccoon_morse_sw_demo *get_instance(void);

private:
	static raccoon_morse_sw_demo *singleton;
};

#endif /* __RACCOON_MORSE_SW_DEMO_H__ */
