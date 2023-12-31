#include "../../include/lib/ps2_keyboard.h"

uint8_t keyboard_control_read_status() {
 
	return inb(KEYBOARD_CONTROL_STATS_REG);
}

void keyboard_control_send_cmd(uint8_t command) {

	while (TRUE) {
		if ((keyboard_control_read_status() & KEYBOARD_CONTROL_STATS_MASK_IN_BUF) == 0) {
			break;
        }
    }
	outb(KEYBOARD_CONTROL_CMD_REG, command);

}

uint8_t keyboard_encoder_read_buf() {
    return inb(KEYBOARD_ENCODER_INPUT_BUF);
}

void keyboard_encoder_send_cmd(uint8_t command) {
    while(TRUE) {
        if (keyboard_control_read_status() & KEYBOARD_CONTROL_STATS_MASK_IN_BUF == 0) {
            break;
        }
    }
    
    outb(KEYBOARD_ENCODER_CMD_REG, command);
}

void keyboard_set_leds(bool_t num, bool_t caps, bool_t scroll) {
    uint8_t data = 0;

    data = scroll ? (data | 1) : (data & 1);
    data = num ? (data | 2) : (data & 2);
    data = caps ? (data | 4) : (data & 4);

    keyboard_encoder_send_cmd(KEYBOARD_ENCODER_CMD_SET_LEDS);
    keyboard_encoder_send_cmd(data);
}

bool_t test() {
    keyboard_control_send_cmd(0xaa);

    while (TRUE) {
        if (keyboard_control_read_status() & KEYBOARD_CONTROL_STATS_MASK_OUT_BUF) {
            break;
        }
    }

    return keyboard_encoder_read_buf() == 0x55 ? TRUE : FALSE;
}

void keyboard_enable() {
    pic_unmask_irq(33);
    keyboard_control_send_cmd(ENABLE_KEYBOARD_BYTE);
}

void keyboard_disable() {
    keyboard_control_send_cmd(DISABLE_KEYBOARD_BYTE);
    pic_mask_irq(33);
}

void system_reset() {
    keyboard_control_send_cmd(0x60);
    keyboard_encoder_send_cmd(KEYBOARD_SYSTEM_RESET);
}

void init_keyboard() {
    keyboard_vector = idt_allocate_vector();

}