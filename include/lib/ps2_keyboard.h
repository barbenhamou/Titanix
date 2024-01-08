#ifndef PS2_KEYBOARD_H
#define PS2_KEYBOARD_H

#include "interrupts.h"

enum KEYBOARD_ENCODER_IO {
	KEYBOARD_ENCODER_INPUT_BUF	=	0x60,
	KEYBOARD_ENCODER_CMD_REG	=	0x60
};

enum KEYBOARD_ENCODER_CMDS {
	
	KEYBOARD_ENCODER_CMD_SET_LEDS = 0xed,
	KEYBOARD_ENCODER_CMD_ECHO = 0xee,
	KEYBOARD_ENCODER_CMD_SCANCODE_SET = 0xf0,
	KEYBOARD_ENCODER_CMD_ID = 0xf2,
	KEYBOARD_ENCODER_CMD_AUTODELAY = 0xf3,
	KEYBOARD_ENCODER_CMD_ENABLE = 0xf4,
	KEYBOARD_ENCODER_CMD_RESETWAIT = 0xf5,
	KEYBOARD_ENCODER_CMD_RESETSCAN = 0xf6,
	KEYBOARD_ENCODER_CMD_ALL_AUTO = 0xf7,
	KEYBOARD_ENCODER_CMD_ALL_MAKEBREAK = 0xf8,
	KEYBOARD_ENCODER_CMD_ALL_MAKEONLY = 0xf9,
	KEYBOARD_ENCODER_CMD_ALL_MAKEBREAK_AUTO = 0xfa,
	KEYBOARD_ENCODER_CMD_SINGLE_AUTOREPEAT = 0xfb,
	KEYBOARD_ENCODER_CMD_SINGLE_MAKEBREAK = 0xfc,
	KEYBOARD_ENCODER_CMD_SINGLE_BREAKONLY = 0xfd,
	KEYBOARD_ENCODER_CMD_RESEND = 0xfe,
	KEYBOARD_ENCODER_CMD_RESET = 0xff
};
 
enum KEYBOARD_CONTROL_IO {
	KEYBOARD_CONTROL_STATS_REG	=	0x64,
	KEYBOARD_CONTROL_CMD_REG	=	0x64
};

enum KEYBOARD_CONTROL_STATS_MASK {
	KEYBOARD_CONTROL_STATS_MASK_OUT_BUF	=	1,
	KEYBOARD_CONTROL_STATS_MASK_IN_BUF	=	2,
    KEYBOARD_CONTROL_STATS_MASK_SYSTEM	=	4,
	KEYBOARD_CONTROL_STATS_MASK_CMD_DATA =	8,		
	KEYBOARD_CONTROL_STATS_MASK_LOCKED	=	0x10,		
	KEYBOARD_CONTROL_STATS_MASK_AUX_BUF	=	0x20,		
	KEYBOARD_CONTROL_STATS_MASK_TIMEOUT	=	0x40,		
	KEYBOARD_CONTROL_STATS_MASK_PARITY	=	0x80		
};

enum KEYBOARD_CONTROL_CMDS {
	KEYBOARD_CONTROL_CMD_READ = 0x20,
	KEYBOARD_CONTROL_CMD_WRITE = 0x60,
	KEYBOARD_CONTROL_CMD_SELF_TEST = 0xaa,
	KEYBOARD_CONTROL_CMD_INTEFACE_TEST = 0xab,
	KEYBOARD_CONTROL_CMD_DISABLE = 0xad,
	KEYBOARD_CONTROL_CMD_ENABLE = 0xae,
	KEYBOARD_CONTROL_CMD_READ_IN_PORT = 0xc0,
	KEYBOARD_CONTROL_CMD_READ_OUT_PORT = 0xd0,
	KEYBOARD_CONTROL_CMD_WRITE_OUT_PORT = 0xd1,
	KEYBOARD_CONTROL_CMD_READ_TEST_INPUTS = 0xe0,
	KEYBOARD_CONTROL_CMD_SYSTEM_RESET = 0xfe,
	KEYBOARD_CONTROL_CMD_MOUSE_DISABLE = 0xa7,
	KEYBOARD_CONTROL_CMD_MOUSE_ENABLE = 0xa8,
	KEYBOARD_CONTROL_CMD_MOUSE_PORT_TEST = 0xa9,
	KEYBOARD_CONTROL_CMD_MOUSE_WRITE = 0xd4
};

enum KEYBOARD_ERROR {
	KEYBOARD_ERROR_BUF_OVERRUN = 0,
	KEYBOARD_ERROR_ID_RET = 0x83ab,
	KEYBOARD_ERROR_BAT = 0xaa,
	KEYBOARD_ERROR_ECHO_RET = 0xee,
	KEYBOARD_ERROR_ACK = 0xfa,
	KEYBOARD_ERROR_BAT_FAILED = 0xfc,
	KEYBOARD_ERROR_DIAG_FAILED = 0xfd,
	KEYBOARD_ERROR_RESEND_CMD = 0xfe,
	KEYBOARD_ERROR_KET = 0xff
};

enum KEYCODE {

// Alphanumeric keys ////////////////

	KEY_SPACE             = ' ',
	KEY_0                 = '0',
	KEY_1                 = '1',
	KEY_2                 = '2',
	KEY_3                 = '3',
	KEY_4                 = '4',
	KEY_5                 = '5',
	KEY_6                 = '6',
	KEY_7                 = '7',
	KEY_8                 = '8',
	KEY_9                 = '9',

	KEY_A                 = 'a',
	KEY_B                 = 'b',
	KEY_C                 = 'c',
	KEY_D                 = 'd',
	KEY_E                 = 'e',
	KEY_F                 = 'f',
	KEY_G                 = 'g',
	KEY_H                 = 'h',
	KEY_I                 = 'i',
	KEY_J                 = 'j',
	KEY_K                 = 'k',
	KEY_L                 = 'l',
	KEY_M                 = 'm',
	KEY_N                 = 'n',
	KEY_O                 = 'o',
	KEY_P                 = 'p',
	KEY_Q                 = 'q',
	KEY_R                 = 'r',
	KEY_S                 = 's',
	KEY_T                 = 't',
	KEY_U                 = 'u',
	KEY_V                 = 'v',
	KEY_W                 = 'w',
	KEY_X                 = 'x',
	KEY_Y                 = 'y',
	KEY_Z                 = 'z',

	KEY_RETURN            = '\r',
	KEY_ESCAPE            = 0x1001,
	KEY_BACKSPACE         = '\b',

// Arrow keys ////////////////////////

	KEY_UP                = 0x1100,
	KEY_DOWN              = 0x1101,
	KEY_LEFT              = 0x1102,
	KEY_RIGHT             = 0x1103,

// Function keys /////////////////////

	KEY_F1                = 0x1201,
	KEY_F2                = 0x1202,
	KEY_F3                = 0x1203,
	KEY_F4                = 0x1204,
	KEY_F5                = 0x1205,
	KEY_F6                = 0x1206,
	KEY_F7                = 0x1207,
	KEY_F8                = 0x1208,
	KEY_F9                = 0x1209,
	KEY_F10               = 0x120a,
	KEY_F11               = 0x120b,
	KEY_F12               = 0x120b,
	KEY_F13               = 0x120c,
	KEY_F14               = 0x120d,
	KEY_F15               = 0x120e,

	KEY_DOT               = '.',
	KEY_COMMA             = ',',
	KEY_COLON             = ':',
	KEY_SEMICOLON         = ';',
	KEY_SLASH             = '/',
	KEY_BACKSLASH         = '\\',
	KEY_PLUS              = '+',
	KEY_MINUS             = '-',
	KEY_ASTERISK          = '*',
	KEY_EXCLAMATION       = '!',
	KEY_QUESTION          = '?',
	KEY_QUOTEDOUBLE       = '\"',
	KEY_QUOTE             = '\'',
	KEY_EQUAL             = '=',
	KEY_HASH              = '#',
	KEY_PERCENT           = '%',
	KEY_AMPERSAND         = '&',
	KEY_UNDERSCORE        = '_',
	KEY_LEFTPARENTHESIS   = '(',
	KEY_RIGHTPARENTHESIS  = ')',
	KEY_LEFTBRACKET       = '[',
	KEY_RIGHTBRACKET      = ']',
	KEY_LEFTCURL          = '{',
	KEY_RIGHTCURL         = '}',
	KEY_DOLLAR            = '$',
	KEY_POUND             = '�',
	KEY_EURO              = '$',
	KEY_LESS              = '<',
	KEY_GREATER           = '>',
	KEY_BAR               = '|',
	KEY_GRAVE             = '`',
	KEY_TILDE             = '~',
	KEY_AT                = '@',
	KEY_CARRET            = '^',

// Numeric keypad //////////////////////

	KEY_KP_0              = '0',
	KEY_KP_1              = '1',
	KEY_KP_2              = '2',
	KEY_KP_3              = '3',
	KEY_KP_4              = '4',
	KEY_KP_5              = '5',
	KEY_KP_6              = '6',
	KEY_KP_7              = '7',
	KEY_KP_8              = '8',
	KEY_KP_9              = '9',
	KEY_KP_PLUS           = '+',
	KEY_KP_MINUS          = '-',
	KEY_KP_DECIMAL        = '.',
	KEY_KP_DIVIDE         = '/',
	KEY_KP_ASTERISK       = '*',
	KEY_KP_NUMLOCK        = 0x300f,
	KEY_KP_ENTER          = 0x3010,

	KEY_TAB               = 0x4000,
	KEY_CAPSLOCK          = 0x4001,

// Modify keys ////////////////////////////

	KEY_LSHIFT            = 0x4002,
	KEY_LCTRL             = 0x4003,
	KEY_LALT              = 0x4004,
	KEY_LWIN              = 0x4005,
	KEY_RSHIFT            = 0x4006,
	KEY_RCTRL             = 0x4007,
	KEY_RALT              = 0x4008,
	KEY_RWIN              = 0x4009,

	KEY_INSERT            = 0x400a,
	KEY_DELETE            = 0x400b,
	KEY_HOME              = 0x400c,
	KEY_END               = 0x400d,
	KEY_PAGEUP            = 0x400e,
	KEY_PAGEDOWN          = 0x400f,
	KEY_SCROLLLOCK        = 0x4010,
	KEY_PAUSE             = 0x4011,

	KEY_UNKNOWN,
	KEY_NUMKEYCODES
};

#define is_ascii(c) ((char_t)c <= 0x7f)

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


uint8_t keyboard_control_read_status();
void keyboard_control_send_cmd(uint8_t command);
uint8_t keyboard_encoder_read_buf();
void keyboard_encoder_send_cmd(uint8_t command);
void keyboard_set_leds(bool_t num, bool_t caps, bool_t scroll);
bool_t test();
void keyboard_enable();
void keyboard_disable();
void system_reset();
void init_keyboard();

void handler(isr_frame_t* frame);
void init_3();
#endif