#include "../../include/lib/ps2_keyboard.h"

static char_t scancode;

static bool_t capslock, numlock, scrolllock, ctrldown, altdown, shiftdown;

static uint32_t keyboard_error = 0, keyboard_vector = 0;

static bool_t keyboard_bat_res = FALSE, keyboard_diag_res = FALSE, keyboard_resend_res = FALSE, is_keyboard_disable = FALSE;

//index = scancode
static int keyboard_scancodes [] = {

	//! key			scancode
	KEY_UNKNOWN,	//0
	KEY_ESCAPE,		//1
	KEY_1,			//2
	KEY_2,			//3
	KEY_3,			//4
	KEY_4,			//5
	KEY_5,			//6
	KEY_6,			//7
	KEY_7,			//8
	KEY_8,			//9
	KEY_9,			//0xa
	KEY_0,			//0xb
	KEY_MINUS,		//0xc
	KEY_EQUAL,		//0xd
	KEY_BACKSPACE,	//0xe
	KEY_TAB,		//0xf
	KEY_Q,			//0x10
	KEY_W,			//0x11
	KEY_E,			//0x12
	KEY_R,			//0x13
	KEY_T,			//0x14
	KEY_Y,			//0x15
	KEY_U,			//0x16
	KEY_I,			//0x17
	KEY_O,			//0x18
	KEY_P,			//0x19
	KEY_LEFTBRACKET,//0x1a
	KEY_RIGHTBRACKET,//0x1b
	KEY_RETURN,		//0x1c
	KEY_LCTRL,		//0x1d
	KEY_A,			//0x1e
	KEY_S,			//0x1f
	KEY_D,			//0x20
	KEY_F,			//0x21
	KEY_G,			//0x22
	KEY_H,			//0x23
	KEY_J,			//0x24
	KEY_K,			//0x25
	KEY_L,			//0x26
	KEY_SEMICOLON,	//0x27
	KEY_QUOTE,		//0x28
	KEY_GRAVE,		//0x29
	KEY_LSHIFT,		//0x2a
	KEY_BACKSLASH,	//0x2b
	KEY_Z,			//0x2c
	KEY_X,			//0x2d
	KEY_C,			//0x2e
	KEY_V,			//0x2f
	KEY_B,			//0x30
	KEY_N,			//0x31
	KEY_M,			//0x32
	KEY_COMMA,		//0x33
	KEY_DOT,		//0x34
	KEY_SLASH,		//0x35
	KEY_RSHIFT,		//0x36
	KEY_KP_ASTERISK,//0x37
	KEY_RALT,		//0x38
	KEY_SPACE,		//0x39
	KEY_CAPSLOCK,	//0x3a
	KEY_F1,			//0x3b
	KEY_F2,			//0x3c
	KEY_F3,			//0x3d
	KEY_F4,			//0x3e
	KEY_F5,			//0x3f
	KEY_F6,			//0x40
	KEY_F7,			//0x41
	KEY_F8,			//0x42
	KEY_F9,			//0x43
	KEY_F10,		//0x44
	KEY_KP_NUMLOCK,	//0x45
	KEY_SCROLLLOCK,	//0x46
	KEY_HOME,		//0x47
	KEY_KP_8,		//0x48	//keypad up arrow
	KEY_PAGEUP,		//0x49
	KEY_KP_2,		//0x50	//keypad down arrow
	KEY_KP_3,		//0x51	//keypad page down
	KEY_KP_0,		//0x52	//keypad insert key
	KEY_KP_DECIMAL,	//0x53	//keypad delete key
	KEY_UNKNOWN,	//0x54
	KEY_UNKNOWN,	//0x55
	KEY_UNKNOWN,	//0x56
	KEY_F11,		//0x57
	KEY_F12			//0x58
};

const uint32_t INVALID_SCANCODE = 0;

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

void keyboard_handler_2 (isr_frame_t* frame) {
    DEBUG("sd\n");

    uint32_t code = 0;
    static bool_t extended = FALSE;

    if (keyboard_control_read_status() & KEYBOARD_CONTROL_STATS_MASK_OUT_BUF) {
        
        code = keyboard_encoder_read_buf();
        
        if (code == 0xe0 || code == 0xe1) {

            extended = TRUE;

        } else {

            extended = FALSE;

            if (code & 0x80) {
                code -= 0x80;

                uint32_t key = keyboard_scancodes[code];

                switch (key) {
                    case KEY_LCTRL:

                    case KEY_RCTRL: {
                        ctrldown = FALSE;
                        break;
                    }

                    case KEY_LSHIFT:

                    case KEY_RSHIFT: {
                        shiftdown = FALSE;
                        break;
                    }

                    case KEY_LALT:

                    case KEY_RALT: {
                        altdown = FALSE;
                        break;
                    }                  
                }

            } else {
                scancode = code;
                uint32_t key = keyboard_scancodes[code];

                switch (key) {
                   case KEY_LCTRL:

                   case KEY_RCTRL: {
                       ctrldown = TRUE;
                       break;
                   }

                   case KEY_LSHIFT:

                   case KEY_RSHIFT: {
                       shiftdown = TRUE;
                       break;
                   }

                   case KEY_CAPSLOCK: {
                       capslock = (capslock) ? FALSE : TRUE;
                       keyboard_set_leds(numlock, capslock, scrolllock);
                       break;
                   }  

                   case KEY_KP_NUMLOCK: {
                       numlock = (numlock) ? FALSE : TRUE;
                       keyboard_set_leds(numlock, capslock, scrolllock);
                       break;
                   }

                   case KEY_SCROLLLOCK: {
                       scrolllock = (scrolllock) ? FALSE : TRUE;
                       keyboard_set_leds(numlock, capslock, scrolllock);
                       break;
                   }    
                }
            }
        }

        switch (code) {
            case KEYBOARD_ERROR_BAT_FAILED: {
                keyboard_bat_res = FALSE;
                break;
            }

            case KEYBOARD_ERROR_DIAG_FAILED: {
                keyboard_diag_res = FALSE;
                break;
            }

            case KEYBOARD_ERROR_RESEND_CMD: {
                keyboard_resend_res = FALSE;
                break;
            }
        }
    }

    pic_send_eoi(keyboard_vector);
}

enum KEYCODE keyboard_get_last_key() {
    return (scancode != INVALID_SCANCODE) ? ((enum KEYCODE)keyboard_scancodes[scancode]) : (KEY_UNKNOWN);
}

char_t keyboard_key_to_ascii(enum KEYCODE code) {
    uint8_t key = code;

    if (is_ascii(key)) {
        if (shiftdown || capslock) {
            if (key >= 'a' && key <= 'z') {
                key -= 32;
            }
        }

        if (shiftdown && !capslock) {
            if (key >= '0' && key <= '9') {
                switch (key) {
                    case 0: {
                        key = KEY_RIGHTPARENTHESIS;
                        break;
                    }

                    case 1: {
                        key = KEY_EXCLAMATION;
                    }

                    case 2: {
                        key = KEY_AT;
                        break;
                    }

                    case 3: {
                        key = KEY_EXCLAMATION;
                        break;
                    }

                    case 4: {
                        key = KEY_DOLLAR;
                        break;
                    }

                    case 5: {
                        key = KEY_PERCENT;
                        break;
                    }

                    case 6: {
                        key = KEY_CARRET;
                        break;
                    }

                    case 7: {
                        key = KEY_AMPERSAND;
                        break;
                    }

                    case 8: {
                        key = KEY_ASTERISK;
                        break;
                    }

                    case 9: {
                        key = KEY_LEFTPARENTHESIS;
                        break;
                    }
                }
            } else {
                switch (key) {
                    case KEY_COMMA: {
                        key = KEY_LESS;
                        break;
                    }

                    case KEY_DOT: {
                        key = KEY_GREATER;
                        break;
                    }

                    case KEY_SLASH: {
                        key = KEY_QUESTION;
                        break;
                    }

                    case KEY_SEMICOLON: {
                        key = KEY_COLON;
                        break;
                    }

                    case KEY_QUOTE: {
                        key = KEY_QUOTEDOUBLE;
                        break;
                    }

                    case KEY_LEFTBRACKET: {
                        key = KEY_LEFTCURL;
                        break;
                    }

                    case KEY_RIGHTBRACKET: {
                        key = KEY_RIGHTCURL;
                        break;
                    }

                    case KEY_GRAVE: {
                        key = KEY_TILDE;
                        break;
                    }

                    case KEY_MINUS: {
                        key = KEY_UNDERSCORE;
                        break;
                    }

                    case KEY_EQUAL: {
                        key = KEY_PLUS;
                        break;
                    }

                    case KEY_BACKSLASH: {
                        key = KEY_BAR;
                        break;
                    }
                }
            }
            return key;
        }
    }
    return 0;
}

void keyboard_enable() {
    pic_unmask_irq(32);
    keyboard_control_send_cmd(ENABLE_KEYBOARD_BYTE);
}

void keyboard_disable() {
    keyboard_control_send_cmd(DISABLE_KEYBOARD_BYTE);
    pic_mask_irq(32);
}

void system_reset() {
    keyboard_control_send_cmd(0x60);
    keyboard_encoder_send_cmd(KEYBOARD_SYSTEM_RESET);
}

void init_keyboard() {
    keyboard_vector = idt_allocate_vector();
    //outb(PS2_KEYBOARD_COMMAND_PORT, 0xae);
    idt_install_irq(keyboard_vector, (void*)&keyboard_handler_2);

}