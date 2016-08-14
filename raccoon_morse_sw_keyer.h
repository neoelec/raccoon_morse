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
 * File Name : raccoon_morse_sw_keyer.h
 *
 * File Description :
 *
 * Author : Joo, Young Jin <neoelec@gmail.com>
 * Dept : Raccoon's Cave
 * Created Date : 14/Aug/2016
 * Version : Baby-Raccoon
 */

#ifndef __RACCOON_MORSE_SW_KEYER_H__
#define __RACCOON_MORSE_SW_KEYER_H__

#include "raccoon_keyer_base.h"
#include "raccoon_morse_sw.h"

class raccoon_morse_sw_keyer : public raccoon_morse_sw {
public:
	virtual void handler(void);
	virtual void start(void);
	virtual void stop(void);

protected:
	virtual void text(void);

	raccoon_keyer_base *keyer;
};

#endif /* __RACCOON_MORSE_SW_KEYER_H__ */
