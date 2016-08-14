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
 * File Name : raccoon_morse.ino
 *
 * File Description :
 *
 * Author : Joo, Young Jin <neoelec@gmail.com>
 * Dept : Raccoon's Cave
 * Created Date : 14/Aug/2016
 * Version : Baby-Raccoon
 */

#include <avr/pgmspace.h>

#include "pcint.h"

#include "raccoon_morse_hw.h"
#include "raccoon_morse_hw_uno.h"
#include "raccoon_morse_menu.h"

raccoon_morse_hw *morse_hw;
raccoon_morse_sw *morse_sw;
raccoon_morse_menu *morse_menu;

static void morse_pcint_handler(void)
{
	morse_sw->handler();
	morse_menu->handler();
}

static const char __morse_banner[] PROGMEM =
		"Raccoon's Morse "
		"    Code Trainer";

void setup(void)
{
	// put your setup code here, to run once:
	morse_hw = raccoon_morse_hw_uno::get_instance();

	morse_hw->init();
	morse_hw->lcd_print_P(__morse_banner);

	delay(1000);

	morse_menu = raccoon_morse_menu::get_instance();
	morse_menu->init();

	pcint_attach(E_PCINT1, morse_pcint_handler);
}

void loop(void)
{
	// put your main code here, to run repeatedly:
	morse_sw->loop();
	morse_menu->loop();
}
