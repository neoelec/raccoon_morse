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
 * File Name : raccoon_morse_sw_keyer.ino
 *
 * File Description :
 *
 * Author : Joo, Young Jin <neoelec@gmail.com>
 * Dept : Raccoon's Cave
 * Created Date : 27/Aug/2016
 * Version : Baby-Raccoon
 */

#include "raccoon_morse_sw_keyer.h"

void raccoon_morse_sw_keyer::handler(void)
{
}

void raccoon_morse_sw_keyer::start(void)
{
	raccoon_morse_sw::start();
	keyer->start();
}

void raccoon_morse_sw_keyer::stop(void)
{
	keyer->stop();
	raccoon_morse_sw::stop();
}

void raccoon_morse_sw_keyer::text(void)
{
	keyer->loop();
}