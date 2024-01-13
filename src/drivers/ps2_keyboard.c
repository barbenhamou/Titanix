#include "../../include/lib/ps2_keyboard.h"

static uint8_t keyboard_vector, scan_set;
static bool_t capslock, numlock, scrolllock, ctrldown, altdown, fndown, shiftdown;
static bool_t is_masked;
static uint64_t counter = 0;
static bool_t responsivity_test() {
    uint8_t st = 0; //status
    outb(PS2_KEYBOARD_DATA_PORT, 0xee);
    st = inb(PS2_KEYBOARD_DATA_PORT);
    return st == 0xee;
}

static void ps2_wait() {
    uint8_t status = inb(PS2_KEYBOARD_STATUS_REG);
    while ((status & 0x02) != 0) {
        status = inb(PS2_KEYBOARD_STATUS_REG);
    }
}

static bool_t keyboard_send_command(uint8_t command, uint8_t operand) {
    if (!responsivity_test()) return FALSE;
    uint8_t st = inb(PS2_KEYBOARD_DATA_PORT);
    if (st != PS2_KEYBOARD_ERROR) {
        outb(PS2_KEYBOARD_DATA_PORT, command);
        ps2_wait();

        if (operand != NO_OPERAND) {
            outb(PS2_KEYBOARD_DATA_PORT, operand);
        }
    }

    uint64_t timeout = 0;
    while (st != PS2_KEYBOARD_ACK) {
        if (st == PS2_KEYBOARD_RESEND && timeout < 1) {
            outb(PS2_KEYBOARD_COMMAND_PORT, command);
            timeout++;
        }
        if (st == PS2_KEYBOARD_ERROR) return FALSE;

        st = inb(PS2_KEYBOARD_DATA_PORT);
    }
    return TRUE;
}

static bool_t next_byte_check(uint8_t* scancode, uint64_t bytes) {
    switch (bytes) {
        case 1: {
            if (scancode[0] == 0xe0) return TRUE;
            if (scancode[0] == 0xf0) return TRUE;
            if (scancode[0] == 0xe1) return TRUE;
            break;
        }
        case 2: {
            if (scancode[1] == 0xf0) return TRUE;
            if (scancode[0] == 0xe0 && scancode[1] == 0x12) return TRUE;
            if (scancode[0] == 0xe1 && scancode[1] == 0x14) return TRUE;
            break;
        }
        case 3: {
            if (scancode[2] == 0x77) return TRUE;
            if (scancode[2] == 0x7c) return TRUE;
            if (scancode[2] == 0xe0) return TRUE;
            break;
        }
        case 4: {
            if (scancode[3] == 0xe0) return TRUE;
            if (scancode[3] == 0xe1) return TRUE;
            break;
        }
        case 5: {
            if (scancode[4] == 0xf0) return TRUE;
            break;
        }
        case 6: {
            if (scancode[5] == 0x14) return TRUE;
            break;
        }
        case 7: {
            if (scancode[6] == 0xf0) return TRUE;
            break;
        } 
        default: 
    }
    return FALSE;
}

static bool_t is_valid_keyboard() {
    return (scan_set == SUPPOTED_SET) && !is_masked;
}

uint64_t ps2_keyboard_get_scancode(uint8_t* scancode_out) {
    if (!is_valid_keyboard()) return 0;
    uint64_t bytes;
    for (bytes = 0; bytes < 8; ++bytes) {
        scancode_out[bytes] = inb(PS2_KEYBOARD_DATA_PORT);
        if (!next_byte_check(scancode_out, bytes + 1)) return ++bytes;
    }

    return 0;
}

void ps2_keyboard_handle_scancode(uint8_t* scancode, uint64_t bytes) {
    uint64_t rendered_scancode = 0x00;
    if (counter == 100000) {
        set_mask(FALSE);
        return;
    }
    counter++;
    if (bytes == 0) return;
    for (uint64_t i = 0; i < bytes; ++i) {
        rendered_scancode <<= 8;
        rendered_scancode |= scancode[i];
    }

    char_t c = analayze_ascii(rendered_scancode);
    if (c == '~') return;
    if (c == '\n') {
        monitor_put('\n');
        return;
    }
    monitor_put(c);
}

char_t analayze_ascii(uint64_t scancode) {
    switch (scancode)
    {
        case SHIFT: {
            shiftdown = TRUE;
            return '~';
            break;
        }

        case SHIFT_UP: {
            shiftdown = FALSE;
            return '~';
            break;
        }

        case CAPSLOCK: {
            if (capslock) {
                capslock = FALSE;
            } else {
                capslock = TRUE;
            }
            return '~';
            break;
        }

        case SCROLLLOCK: {
            scrolllock != scrolllock;
            return '~';
            break;
        }

        case CTRL: {
            ctrldown = TRUE;
            return '~';
            break;
        }

        case CTRL_UP: {
            ctrldown = FALSE;
            return '~';
            break;
        }

        case SLASH: {
            if (shiftdown) return '?';
            return '/';
            break;
        }

        case ENTER: {
            return '\n';
            break;
        }

        case BACKSPACE: {
            handle_back_space();
            return '~';
            break;
        }

        case SPACE: {
            return ' ';
            break;
        }

        case DOT: {
            if (shiftdown) return '>';
            return '.';
            break;
        }

        case COMMA: {
            if (shiftdown) return '<';
            return ',';
            break;
        }

        case A: {
            if (shiftdown || capslock) return 'A';
            return 'a';
            break;
        }

        case B: {
            if (shiftdown || capslock) return 'B';
            return 'b';
            break;
        }

        case C: {
            if (shiftdown || capslock) return 'C';
            return 'c';
            break;
        }

        case D: {
            if (shiftdown || capslock) return 'D';
            return 'd';
            break;
        }

        case E: {
            if (shiftdown || capslock) return 'E';
            return 'e';
            break;
        }

        case F: {
            if (shiftdown || capslock) return 'F';
            return 'f';
            break;
        }

        case G: {
            if (shiftdown || capslock) return 'G';
            return 'g';
            break;
        }

        case H: {
            if (shiftdown || capslock) return 'H';
            return 'h';
            break;
        }

        case I: {
            if (shiftdown || capslock) return 'I';
            return 'i';
            break;
        }

        case J: {
            if (shiftdown || capslock) return 'J';
            return 'j';
            break;
        }

        case K: {
            if (shiftdown || capslock) return 'K';
            return 'k';
            break;
        }

        case L: {
            if (shiftdown || capslock) return 'L';
            return 'l';
            break;
        }

        case M: {
            if (shiftdown || capslock) return 'M';
            return 'm';
            break;
        }

        case N: {
            if (shiftdown || capslock) return 'N';
            return 'n';
            break;
        }

        case O: {
            if (shiftdown || capslock) return 'O';
            return 'o';
            break;
        }

        case P: {
            if (shiftdown || capslock) return 'P';
            return 'p';
            break;
        }

        case Q: {
            if (shiftdown || capslock) return 'Q';
            return 'q';
            break;
        }

        case R: {
            if (shiftdown || capslock) return 'R';
            return 'r';
            break;
        }

        case S: {
            if (shiftdown || capslock) return 'S';
            return 's';
            break;
        }

        case T: {
            if (shiftdown || capslock) return 'T';
            return 't';
            break;
        }

        case U: {
            if (shiftdown || capslock) return 'U';
            return 'u';
            break;
        }

        case V: {
            if (shiftdown || capslock) return 'V';
            return 'v';
            break;
        }

        case W: {
            if (shiftdown || capslock) return 'W';
            return 'w';
            break;
        }

        case X: {
            if (shiftdown || capslock) return 'X';
            return 'x';
            break;
        }

        case Y: {
            if (shiftdown || capslock) return 'Y';
            return 'y';
            break;
        }

        case Z: {
            if (shiftdown || capslock) return 'Z';
            return 'z';
            break;
        }

        case NO_1: {
            if (shiftdown) return '!';
            return '1';
            break;
        }

        case NO_2: {
            if (shiftdown) return '@';
            return '2';
            break;
        }

        case NO_3: {
            if (shiftdown) return '#';
            return '3';
            break;
        }

        case NO_4: {
            if (shiftdown) return '$';
            return '4';
            break;
        }

        case NO_5: {
            if (shiftdown) return '%';
            return '5';
            break;
        }

        case NO_6: {
            if (shiftdown) return '^';
            return '6';
            break;
        }

        case NO_7: {
            if (shiftdown) return '&';
            return '7';
            break;
        }

        case NO_8: {
            if (shiftdown) return '*';
            return '8';
            break;
        }

        case NO_9: {
            if (shiftdown) return '(';
            return '9';
            break;
        }

        case NO_0: {
            if (shiftdown) return ')';
            return '0';
            break;
        }

        default: {
            return '~';
            break;
        }   
    }
}

void keyboard_handler(isr_frame_t* frame) {
    if (!is_valid_keyboard()) {
        if (is_masked) {
            pic_mask_irq(keyboard_vector);
        } else {
            return;
        }
    }

    uint8_t* scancode = {0, 0, 0, 0, 0, 0, 0, 0};
    uint64_t scancode_size = ps2_keyboard_get_scancode(&scancode[0]);
    if (scancode_size == 0) return;
    ps2_keyboard_handle_scancode(&scancode[0], scancode_size);
    keyboard_sync_leds();
    pic_send_eoi(keyboard_vector);
}

void set_mask(bool_t mask) {
    if (mask) {
        pic_mask_irq(keyboard_vector);
    } else {
        pic_unmask_irq(keyboard_vector);
    }
}

static void keyboard_sync_leds() {
    uint8_t leds = 0x00;
    if (scrolllock) leds |= (1 << 0);
    if (numlock) leds |= (1 << 1);
    if (capslock) leds |= (1 << 2);
    keyboard_send_command(0xed, leds);
}

static void keyboard_reset_state() {
    capslock = FALSE;
    numlock = FALSE;
    scrolllock = FALSE;
    ctrldown = FALSE;
    altdown = FALSE;
    shiftdown = FALSE;
    fndown = FALSE;
    keyboard_sync_leds();
}

void ps2_keyboard_disable_set_translation() {
    uint8_t config_byte = 0;
    ps2_wait();
    outb(PS2_KEYBOARD_COMMAND_PORT, keyboard_vector);
    ps2_wait();
    config_byte = inb(PS2_KEYBOARD_DATA_PORT);
    config_byte &= ~(1 << 6);
    outb(PS2_KEYBOARD_COMMAND_PORT, 0x60);
    outb(PS2_KEYBOARD_DATA_PORT, config_byte);
    ps2_wait();
}

void ps2_keyboard_set_scancodes(uint8_t scancode_set) {
    ps2_keyboard_disable_set_translation();
    scan_set = scancode_set;
    if (scancode_set == 0) return;
    keyboard_send_command(0xf0, scancode_set);
    ps2_wait();
    outb(PS2_KEYBOARD_DATA_PORT, 0xf0);
    ps2_wait();
    outb(PS2_KEYBOARD_DATA_PORT, 0x00);
    ps2_wait();
    inb(PS2_KEYBOARD_DATA_PORT);
    uint8_t set = inb(PS2_KEYBOARD_DATA_PORT);
    if (set != scancode_set) {
        ps2_keyboard_set_scancodes(scancode_set);
    }
} 

void keyboard_init() {
    if (keyboard_vector != 0x00) return;

    keyboard_vector = 3;
    outb(PS2_KEYBOARD_COMMAND_PORT, 0xae);
    ps2_keyboard_set_scancodes(SCAN_CODE_SET_2);
    keyboard_reset_state();

    keyboard_vector = idt_allocate_vector();
    DEBUG("%d\n", keyboard_vector);
    idt_install_irq(keyboard_vector, (void*)&keyboard_handler);
    if (responsivity_test()) {
        DEBUG("d\n");
    }
}
