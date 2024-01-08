#ifndef PS2_KEYBOARD_H
#define PS2_KEYBOARD_H

#include "interrupts.h"

#define KEYBOARD_ENCODER_CMD_SET_LEDS 0xed
#define ENABLE_KEYBOARD_BYTE 0xae
#define DISABLE_KEYBOARD_BYTE 0xad
#define KEYBOARD_SYSTEM_RESET 0xfe

#define PS2_KEYBOARD_COMMAND_PORT 0x64
#define PS2_KEYBOARD_STATUS_REG 0x64
#define PS2_KEYBOARD_DATA_PORT 0x60

#define KEYBOARD_IRQ 1
#define MASKED TRUE
#define NOT_MASKED FALSE

#define PS2_KEYBOARD_ACK 0xfa
#define PS2_KEYBOARD_ERROR 0xfc
#define PS2_KEYBOARD_RESEND 0xfe
#define PS2_KEYBOARD_ECHO_RESPONSE 0xee

#define SCAN_CODE_SET_1 0x01
#define SCAN_CODE_SET_2 0x02
#define SCAN_CODE_SET_3 0x03
#define SUPPOTED_SET SCAN_CODE_SET_2

#define NO_OPERAND 0xff

static bool_t responsivity_test();
static void ps2_wait();
static bool_t keyboard_send_command(uint8_t command, uint8_t operand);
static bool_t is_valid_keyboard();
void keyboard_handler(isr_frame_t* frame);
void set_mask(bool_t mask);
static void keyboard_sync_leds();
static void keyboard_reset_state();
static char_t analayze_ascii(uint64_t scancode);
void keyboard_init();

enum CODES {
	A = 0x1c,
	B = 0x32,
	C = 0x21,
	D = 0x23,
	E = 0x24,
	F = 0x2b,
	G = 0x34,
	H = 0x33,
	I = 0x43,
	J = 0x3b,
	K = 0x42,
	L = 0x4b,
	M = 0x3a,
	N = 0x31,
	O = 0x44,
	P = 0x4d,
	Q = 0x15,
	R = 0x2d,
	S = 0x1b,
	T = 0x2c,
	U = 0x3c,
	V = 0x2a,
	W = 0x1d,
	X = 0x22,
	Y = 0x35,
	Z = 0x1a,

	NO_1 = 0x16,
	NO_2 = 0x1e,
	NO_3 = 0x26,
	NO_4 = 0x25,
	NO_5 = 0x2e,
	NO_6 = 0x36,
	NO_7 = 0x3d,
	NO_8 = 0x3e,
	NO_9 = 0x46,
	NO_0 = 0x45,

	SPACE = 0x29,
	DOT = 0x49,
	COMMA = 0x41,

	CAPSLOCK = 0x58,
	SHIFT = 0x12,
	SHIFT_UP = 0xf012,
	SCROLLLOCK = 0x7e,
	CTRL = 0x14,
	CTRL_UP = 0xf014,
	SLASH = 0x4a,
	MINUS = 0x4e,
	ENTER = 0x5e,
	BACKSPACE = 0x66
};

#endif