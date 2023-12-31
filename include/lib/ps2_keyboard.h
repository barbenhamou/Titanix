#ifndef PS2_KEYBOARD_H
#define PS2_KEYBOARD_H

#include "interrupts.h"

enum KEYBOARD_ENCODER_IO {
	KEYBOARD_ENCODER_INPUT_BUF	=	0x60,
	KEYBOARD_ENCODER_CMD_REG	=	0x60
};
 
enum KEYBOARD_CONTROL_IO {
	KEYBOARD_CONTROL_STATS_REG	=	0x64,
	KEYBOARD_CONTROL_CMD_REG	=	0x64
};

enum KEYBOARD_CONTROL_STATS_MASK {
	KEYBOARD_CONTROL_STATS_MASK_OUT_BUF	=	1,
	KEYBOARD_CONTROL_STATS_MASK_IN_BUF	=	2,
    KEYBOARD_CONTROL_STATS_MASK_SYSTEM	=	4,
	KEYBOARD_CONTROL_STATS_MASK_CMD_DATA	=	8,		
	KEYBOARD_CONTROL_STATS_MASK_LOCKED	=	0x10,		
	KEYBOARD_CONTROL_STATS_MASK_AUX_BUF	=	0x20,		
	KEYBOARD_CONTROL_STATS_MASK_TIMEOUT	=	0x40,		
	KEYBOARD_CONTROL_STATS_MASK_PARITY	=	0x80		
};

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

#define SCAN_CODE_SET_2 0x01
#define SCAN_CODE_SET_2 0x02
#define SCAN_CODE_SET_2 0x03
#define SUPPOTED_SET SCAN_CODE_SET_2

#define NO_OPERAND 0xff

static bool_t responsivity_test();
static void ps2_wait();
static bool_t keyboard_send_command(uint8_t command, uint8_t operand);
static bool_t is_valid_keyboard();
void keyboard_handler();
void set_mask(bool_t mask);
static void keyboard_sync_leds();
static void keyboard_reset_state();
void keyboard_init();


uint8_t keyboard_control_read_status();
void keyboard_control_send_cmd(uint8_t command);
uint8_t keyboard_encoder_read_buf();
void keyboard_encoder_send_cmd(uint8_t command);
void keyboard_set_leds(bool_t num, bool_t caps, bool_t scroll);
bool_t test();
void keyboard_enable();
void keyboard_disable();
void system_reset(); 
#endif