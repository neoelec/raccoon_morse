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
 * File Name : raccoon_morse_menu.ino
 *
 * File Description :
 *
 * Author : Joo, Young Jin <neoelec@gmail.com>
 * Dept : Raccoon's Cave
 * Created Date : 15/Aug/2016
 * Version : Baby-Raccoon
 */

#include <avr/pgmspace.h>
#include <EEPROM.h>
#include <stdio.h>

#include "raccoon_morse.h"
#include "raccoon_morse_menu.h"
#include "raccoon_morse_sw_key.h"
#include "raccoon_morse_sw_uart.h"
#include "raccoon_morse_sw_paris.h"
#include "raccoon_morse_sw_demo.h"

static const char __raccoon_morse_menu_magic[] PROGMEM = __DATE__ " " __TIME__;

struct raccoon_morse_menu_save_data {
	char magic[ARRAY_SIZE(__raccoon_morse_menu_magic)];
	raccoon_morse_sw *morse_sw;
};

static raccoon_morse_menu_save_data *__raccoon_morse_menu_save_data;

static bool __raccoon_morse_menu_verify_magic(void)
{
	size_t i;
	char tmp_e, tmp_p;

	for (i = 0; i < ARRAY_SIZE(__raccoon_morse_menu_magic); i++) {
		tmp_e = EEPROM.read((int)(&__raccoon_morse_menu_save_data->magic[i]));
		tmp_p = pgm_read_byte(&__raccoon_morse_menu_magic[i]);
		if (tmp_p != tmp_e)
			return false;
	}

	return true;
}

static void __raccoon_morse_menu_save_magic(void)
{
	size_t i;
	char tmp_p;

	for (i = 0; i < ARRAY_SIZE(__raccoon_morse_menu_magic); i++) {
		tmp_p = pgm_read_byte(&__raccoon_morse_menu_magic[i]);
		EEPROM.write((int)(&__raccoon_morse_menu_save_data->magic[i]), tmp_p);
	}
}

static void __raccoon_morse_menu_load_morse_sw(void)
{
	union {
		raccoon_morse_sw *morse_sw;
		uint8_t b[];
	} morse_sw_ptr;
	uint8_t *ptr = (uint8_t *)&__raccoon_morse_menu_save_data->morse_sw;
	size_t i;

	for (i = 0; i < sizeof(void *); i++)
		morse_sw_ptr.b[i] = EEPROM.read((int)ptr + i);

	morse_sw = morse_sw_ptr.morse_sw;
}

static void __raccoon_morse_menu_save_morse_sw(void)
{
	union {
		raccoon_morse_sw *morse_sw;
		uint8_t b[];
	} morse_sw_ptr;
	uint8_t *ptr = (uint8_t *)&__raccoon_morse_menu_save_data->morse_sw;
	size_t i;

	morse_sw_ptr.morse_sw = morse_sw;

	for (i = 0; i < sizeof(void *); i++)
		EEPROM.write((int)ptr + i, morse_sw_ptr.b[i]);
}

/* */

void raccoon_morse_submenu::init(void)
{
}

void raccoon_morse_submenu::loop(void)
{
}

void raccoon_morse_submenu::key_up(void)
{
}

void raccoon_morse_submenu::key_down(void)
{
}

/* */

class raccoon_morse_submenu_mode : public raccoon_morse_submenu {
public:
	raccoon_morse_submenu_mode();
	static raccoon_morse_submenu_mode *get_instance(void)
	{
		return singleton;
	}

	virtual void init(void);
	virtual void key_up(void);
	virtual void key_down(void);

private:
	void change_item(void);

	static raccoon_morse_submenu_mode *singleton;

	raccoon_morse_sw *item_current;
	raccoon_morse_sw **item_list;
	size_t item_nr;
	size_t item_idx;
};

#define RACCOON_MORSE_SUBMENU_MODE_SW_MAX	4
static raccoon_morse_sw *__raccoon_morse_submenu_mode_item_list[RACCOON_MORSE_SUBMENU_MODE_SW_MAX];

raccoon_morse_submenu_mode::raccoon_morse_submenu_mode()
{
	item_nr = 0;
	item_list = __raccoon_morse_submenu_mode_item_list;

	item_list[item_nr++] = raccoon_morse_sw_key::get_instance();
	item_list[item_nr++] = raccoon_morse_sw_uart::get_instance();
	item_list[item_nr++] = raccoon_morse_sw_paris::get_instance();
	item_list[item_nr++] = raccoon_morse_sw_demo::get_instance();

	item_idx = 0;
	item_current = item_list[item_idx];
}

static raccoon_morse_submenu_mode __raccoon_morse_submenu_mode;

raccoon_morse_submenu_mode *raccoon_morse_submenu_mode::singleton =
		&__raccoon_morse_submenu_mode;

static const char __menu_change_mode[] PROGMEM = "- Change MODE";

void raccoon_morse_submenu_mode::init(void)
{
	morse_hw->lcd_clear();
	morse_hw->lcd_print_P(__menu_change_mode);

	if (morse_sw != item_current) {
		item_idx = 0;
		while (morse_sw != item_current)
			item_current = item_list[++item_idx];
	}

	change_item();
}

void raccoon_morse_submenu_mode::key_up(void)
{
	if (item_idx)
		item_idx--;
	else
		item_idx = item_nr - 1;
	change_item();
}

void raccoon_morse_submenu_mode::key_down(void)
{
	item_idx++;
	item_idx = item_idx % item_nr;
	change_item();
}

void raccoon_morse_submenu_mode::change_item(void)
{
	char buf[8];

	morse_hw->lcd_clear_row(1);

	morse_hw->lcd_set_cursor(0, 1);
	sprintf(buf, "%u. ", item_idx + 1);
	morse_hw->lcd_print(buf);

	item_current = item_list[item_idx];
	morse_hw->lcd_print_P(item_current->get_name());

	morse_sw = item_current;
}

/* */

class raccoon_morse_submenu_wpm : public raccoon_morse_submenu {
public:
	raccoon_morse_submenu_wpm() {}
	static raccoon_morse_submenu_wpm *get_instance(void)
	{
		return singleton;
	}

	virtual void init(void);
	virtual void loop(void);

private:
	static raccoon_morse_submenu_wpm *singleton;
	unsigned long timestamp_prev;
};

static raccoon_morse_submenu_wpm __raccoon_morse_submenu_wpm;

raccoon_morse_submenu_wpm *raccoon_morse_submenu_wpm::singleton =
		&__raccoon_morse_submenu_wpm;

static const char __menu_mode_change_wpm_0[] PROGMEM = "- Change WPM";
static const char __menu_mode_change_wpm_1[] PROGMEM = "WPM:    ADC:    ";

void raccoon_morse_submenu_wpm::init(void)
{
	morse_hw->lcd_clear();
	morse_hw->lcd_print_P(__menu_mode_change_wpm_0);
	morse_hw->lcd_set_cursor(0, 1);
	morse_hw->lcd_print_P(__menu_mode_change_wpm_1);

	timestamp_prev = 0;

	loop();
}

void raccoon_morse_submenu_wpm::loop(void)
{
	unsigned long timestamp = millis();
	unsigned int adc, wpm;
	char buf[8];

	/* FIXME: update every 200msec to prevent adc key error */
	if (200 > timestamp - timestamp_prev)
		return;

	timestamp_prev = timestamp;

	adc = morse_hw->wpm_raw();
	sprintf(buf, "%4u", adc);
	morse_hw->lcd_set_cursor(12, 1);
	morse_hw->lcd_print(buf);

	wpm = raccoon_morse_sw::get_wpm_from_hw();
	sprintf(buf, "%3u", wpm);
	morse_hw->lcd_set_cursor(4, 1);
	morse_hw->lcd_print(buf);
}

/* */

#define RACCOON_MORSE_MENU_SUBMENU_MAX		2

static raccoon_morse_submenu *__raccoon_morse_menu_submenu_list[RACCOON_MORSE_MENU_SUBMENU_MAX];

raccoon_morse_menu::raccoon_morse_menu()
{
	submenu_nr = 0;
	submenu_list = __raccoon_morse_menu_submenu_list;

	submenu_list[submenu_nr++] = raccoon_morse_submenu_mode::get_instance();
	submenu_list[submenu_nr++] = raccoon_morse_submenu_wpm::get_instance();

	submenu_idx = 0;
	submenu_current = submenu_list[submenu_idx];
}

static raccoon_morse_menu __raccoon_morse_menu;

raccoon_morse_menu *raccoon_morse_menu::singleton = &__raccoon_morse_menu;

raccoon_morse_menu *raccoon_morse_menu::get_instance(void)
{
	return singleton;
}

void raccoon_morse_menu::init(void)
{
	state = E_RACCOON_MORSE_MENU_DEACTIVE;

	if (!__raccoon_morse_menu_verify_magic()) {
		morse_sw = raccoon_morse_sw_key::get_instance();
		__raccoon_morse_menu_save_magic();
		__raccoon_morse_menu_save_morse_sw();
	} else {
		__raccoon_morse_menu_load_morse_sw();
	}

	morse_sw->start();
}

bool raccoon_morse_menu::run(void)
{
	switch (state) {
	case E_RACCOON_MORSE_MENU_ENTERED:
		change_submenu();
		sw_previous = morse_sw;
		state = E_RACCOON_MORSE_MENU_SUBMENU;
		break;
	case E_RACCOON_MORSE_MENU_SUBMENU:
		switch (keycode) {
		case E_RACCOON_MORSE_HW_KEYCODE_RUN:
			if (morse_sw != sw_previous)
				__raccoon_morse_menu_save_morse_sw();
			morse_sw->start();
			state = E_RACCOON_MORSE_MENU_DEACTIVE;
			return false;
		case E_RACCOON_MORSE_HW_KEYCODE_LEFT:
			if (submenu_idx)
				submenu_idx--;
			else
				submenu_idx = submenu_nr - 1;
			change_submenu();
			break;
		case E_RACCOON_MORSE_HW_KEYCODE_RIGHT:
			submenu_idx++;
			submenu_idx = submenu_idx % submenu_nr;
			change_submenu();
			break;
		case E_RACCOON_MORSE_HW_KEYCODE_UP:
			submenu_current->key_up();
			break;
		case E_RACCOON_MORSE_HW_KEYCODE_DOWN:
			submenu_current->key_down();
			break;
		default:
			break;
		}
		keycode = E_RACCOON_MORSE_HW_KEYCODE_INVALID;
		state = E_RACCOON_MORSE_MENU_REDY_KEY;
		break;
	default:
		submenu_current->loop();
	}

	return true;
}

void raccoon_morse_menu::loop(void)
{
	if (E_RACCOON_MORSE_MENU_DEACTIVE == state)
		return;

	while (run()) ;
}

void raccoon_morse_menu::handler(void)
{
	raccoon_morse_keystate_t keystate;

	keystate = morse_hw->button_state();
	keycode = morse_hw->button_keycode();

	if (E_RACCOON_MORSE_KEYSTATE_PRESSED != keystate) {
		keycode = E_RACCOON_MORSE_HW_KEYCODE_INVALID;
		return;
	}

	if (E_RACCOON_MORSE_MENU_DEACTIVE == state &&
	    E_RACCOON_MORSE_HW_KEYCODE_RUN == keycode) {
		state = E_RACCOON_MORSE_MENU_ENTERED;
		morse_sw->stop();
		keycode = E_RACCOON_MORSE_HW_KEYCODE_INVALID;
	} else if (E_RACCOON_MORSE_MENU_REDY_KEY == state) {
		state = E_RACCOON_MORSE_MENU_SUBMENU;
	} else {
		keycode = E_RACCOON_MORSE_HW_KEYCODE_INVALID;
	}
}

void raccoon_morse_menu::change_submenu(void)
{
	submenu_current = submenu_list[submenu_idx];
	submenu_current->init();
}
