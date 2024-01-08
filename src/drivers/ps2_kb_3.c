#include "../../include/lib/ps2_keyboard.h"

uint8_t keyboard_vector = 0;

void handler(isr_frame_t* frame) {

    DEBUG("Asd0\n");
    uint8_t scancode = inb(0x60);

    switch (scancode) {
        case 0x0:
            monitor_write("ERROR");
            break;
        case 0x1:
            monitor_write("ESC");
            break;
        case 0x2:
            monitor_write("1");
            break;
        case 0x3:
            monitor_write("2");
            break;
        case 0x4:
            monitor_write("3");
            break;
        case 0x5:
            monitor_write("4");
            break;
        case 0x6:
            monitor_write("5");
            break;
        case 0x7:
            monitor_write("6");
            break;
        case 0x8:
            monitor_write("7");
            break;
        case 0x9:
            monitor_write("8");
            break;
        case 0x0A:
            monitor_write("9");
            break;
        case 0x0B:
            monitor_write("0");
            break;
        case 0x0C:
            monitor_write("-");
            break;
        case 0x0D:
            monitor_write("+");
            break;
        case 0x0E:
            monitor_write("Backspace");
            break;
        case 0x0F:
            monitor_write("Tab");
            break;
        case 0x10:
            monitor_write("Q");
            break;
        case 0x11:
            monitor_write("W");
            break;
        case 0x12:
            monitor_write("E");
            break;
        case 0x13:
            monitor_write("R");
            break;
        case 0x14:
            monitor_write("T");
            break;
        case 0x15:
            monitor_write("Y");
            break;
        case 0x16:
            monitor_write("U");
            break;
        case 0x17:
            monitor_write("I");
            break;
        case 0x18:
            monitor_write("O");
            break;
        case 0x19:
            monitor_write("P");
            break;
        case 0x1A:
            monitor_write("[");
            break;
        case 0x1B:
            monitor_write("]");
            break;
        case 0x1C:
            monitor_write("ENTER");
            break;
        case 0x1D:
            monitor_write("LCtrl");
            break;
        case 0x1E:
            monitor_write("A");
            break;
        case 0x1F:
            monitor_write("S");
            break;
        case 0x20:
            monitor_write("D");
            break;
        case 0x21:
            monitor_write("F");
            break;
        case 0x22:
            monitor_write("G");
            break;
        case 0x23:
            monitor_write("H");
            break;
        case 0x24:
            monitor_write("J");
            break;
        case 0x25:
            monitor_write("K");
            break;
        case 0x26:
            monitor_write("L");
            break;
        case 0x27:
            monitor_write(";");
            break;
        case 0x28:
            monitor_write("'");
            break;
        case 0x29:
            monitor_write("`");
            break;
        case 0x2A:
            monitor_write("LShift");
            break;
        case 0x2B:
            monitor_write("\\");
            break;
        case 0x2C:
            monitor_write("Z");
            break;
        case 0x2D:
            monitor_write("X");
            break;
        case 0x2E:
            monitor_write("C");
            break;
        case 0x2F:
            monitor_write("V");
            break;
        case 0x30:
            monitor_write("B");
            break;
        case 0x31:
            monitor_write("N");
            break;
        case 0x32:
            monitor_write("M");
            break;
        case 0x33:
            monitor_write(",");
            break;
        case 0x34:
            monitor_write(".");
            break;
        case 0x35:
            monitor_write("/");
            break;
        case 0x36:
            monitor_write("Rshift");
            break;
        case 0x37:
            monitor_write("Keypad *");
            break;
        case 0x38:
            monitor_write("LAlt");
            break;
        case 0x39:
            monitor_write("Space");
            break;
        default:
            /* 'keuyp' event corresponds to the 'keydown' + 0x80
             * it may still be a scancode we haven't implemented yet, or
             * maybe a control/escape sequence */
            if (scancode <= 0x7f) {
                monitor_write("Unknown key down");
            } else if (scancode <= 0x39 + 0x80) {
                monitor_write("key up ");
                monitor_put(scancode - 0x80);
            } else monitor_write("Unknown key up");
            break;
    }

    pic_send_eoi(keyboard_vector);
}

void init_3() {
    keyboard_vector = idt_allocate_vector();
    idt_install_irq(keyboard_vector, (void*)&handler);
}