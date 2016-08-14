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
 * File Name : raccoon_morse.h
 *
 * File Description :
 *
 * Author : Joo, Young Jin <neoelec@gmail.com>
 * Dept : Raccoon's Cave
 * Created Date : 14/Aug/2016
 * Version : Baby-Raccoon
 */

#ifndef __RACCOON_MORSE_H__
#define __RACCOON_MORSE_H__

#include "raccoon_morse_hw.h"
#include "raccoon_morse_sw.h"

#define ARRAY_SIZE(_a)		(sizeof(_a) / sizeof(_a[0]))

extern raccoon_morse_hw *morse_hw;
extern raccoon_morse_sw *morse_sw;
extern raccoon_morse_menu *morse_menu;

#endif /* __RACCOON_MORSE_H__ */
