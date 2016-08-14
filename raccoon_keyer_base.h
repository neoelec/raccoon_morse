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
 * File Name : raccoon_keyer_base.h
 *
 * File Description : FSM based keyer module
 *
 * Author : Joo, Young Jin <neoelec@gmail.com>
 * Dept : Raccoon's Cave
 * Created Date : 14/Aug/2016
 * Version : Baby-Raccoon
 */

#ifndef __RACCOON_KEYER_BASE_H__
#define __RACCOON_KEYER_BASE_H__

#include <stdint.h>

typedef enum {
	E_RACCOON_KEYER_STATE_LED_ON = 0,
	E_RACCOON_KEYER_STATE_PULSE_LOW,
	E_RACCOON_KEYER_STATE_NEW_CHAR,
	E_RACCOON_KEYER_STATE_PULSE_HIGH,
	E_RACCOON_KEYER_STATE_IDLE,
} raccoon_keyer_state_t;

typedef enum {
	E_RACCOON_KEYER_PULSE_DIT = 0,
	E_RACCOON_KEYER_PULSE_DAH,
} raccoon_keyer_pulse_t;

class raccoon_keyer_base {
public:
	raccoon_keyer_base();

	virtual void loop(void);
	virtual void start(void);
	virtual void stop(void);

protected:
	virtual void prologue(void);
	virtual void text(void);
	virtual void epilogue(void);

	virtual char do_get_char(void) = 0;

	virtual void do_at_led_on(void);
	virtual void do_at_falling_edge(void);
	virtual void do_at_new_char_started(void);
	virtual void do_at_rising_edge(void);
	virtual void do_at_idle(void);

	char pulse_char;
	raccoon_keyer_pulse_t pulse_type;

	/* timing variables */
	unsigned long dit_ms;
	unsigned long dah_ms;
	unsigned long dididi_ms;
	unsigned long word_break_ms;
	unsigned long led_active_ms;

private:
	bool is_word_break(void);
	bool decode_pulse_char(void);
	void update_pulse_type(void);

	raccoon_keyer_state_t state;
	unsigned long timestamp;
	unsigned long timeout;
	uint16_t pulse_code;
	uint16_t pulse_count;

	bool running;
};

#endif /* __RACCOON_KEYER_BASE_H__ */
