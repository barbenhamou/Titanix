#include "../../include/lib/ps2_keyboard.h"

//uint8_t keyboard_vector = -1;

// static uint8_t keyboard_vector, scan_set;
// static bool_t capslock, numlock, scrolllock, ctrldown, altdown, fndown, shiftdown;
// static bool_t is_masked;

// static bool_t responsivity_test() {
//     uint8_t st = 0; //status
//     outb(PS2_KEYBOARD_DATA_PORT, 0xee);
//     st = inb(PS2_KEYBOARD_DATA_PORT);
//     return st == 0xee;
// }

// static void ps2_wait() {
//     uint8_t st = inb(PS2_KEYBOARD_STATUS_REG);
//     while (st & 0x02) {
//         st = inb(PS2_KEYBOARD_STATUS_REG);
//     }
// }

// static bool_t keyboard_send_command(uint8_t command, uint8_t operand) {
//     if (!responsivity_test()) return FALSE;
//     uint8_t st = inb(PS2_KEYBOARD_DATA_PORT);
//     if (st != PS2_KEYBOARD_ERROR) {
//         outb(PS2_KEYBOARD_DATA_PORT, command);
//         ps2_wait();

//         if (operand != NO_OPERAND) {
//             outb(PS2_KEYBOARD_DATA_PORT, operand);
//         }
//     }

//     uint64_t timeout = 0;
//     while (st != PS2_KEYBOARD_ACK) {
//         if (st = PS2_KEYBOARD_RESEND && timeout < 1) {
//             outb(PS2_KEYBOARD_COMMAND_PORT, command);
//             timeout++;
//         }

//         if (st == PS2_KEYBOARD_ERROR) return FALSE;

//         st = inb(PS2_KEYBOARD_DATA_PORT);
//     }
//     return TRUE;
// }

// static bool_t next_byte_check(uint8_t* scancode, size_t bytes) {
//     switch (bytes) {
//         case 1: {
//             if (scancode[0] == 0xe0) return TRUE;
//             if (scancode[0] == 0xf0) return TRUE;
//             if (scancode[0] == 0xe1) return TRUE;
//             break;
//         }
//         case 2: {
//             if (scancode[1] == 0xe0) return TRUE;
//             if (scancode[0] == 0xf0) return TRUE;
//             if (scancode[0] == 0xe1) return TRUE;
//             break;
//         }
//         case 3: {
//             break;
//         }
//         case 4: {
//             break;
//         }
//         case 5: {
//             break;
//         }
//         case 6: {
//             break;
//         }
//         case 7: {
//             break;
//         } 
//         default: 
//     }
// }

// static bool_t is_valid_keyboard() {
//     return (scan_set == SUPPOTED_SET) && !is_masked;
// }

// void keyboard_handler() {
//     monitor_write("hi");
//     pic_send_eoi(keyboard_vector);
// }

// void set_mask(bool_t mask) {
//     if (mask) {
//         pic_mask_irq(keyboard_vector);
//     } else {
//         pic_unmask_irq(keyboard_vector);
//     }
// }

// static void keyboard_sync_leds() {
//     uint8_t leds = 0x00;
//     if (scrolllock) leds |= (1 << 0);
//     if (numlock) leds |= (1 << 1);
//     if (capslock) leds |= (1 << 2);
//     keyboard_send_command(0xed, leds);
// }

// static void keyboard_reset_state() {
//     capslock = FALSE;
//     numlock = FALSE;
//     scrolllock = FALSE;
//     ctrldown = FALSE;
//     altdown = FALSE;
//     shiftdown = FALSE;
//     fndown = FALSE;
//     keyboard_sync_leds();
// }

// void keyboard_init() {
//     set_mask(TRUE);
//     if (keyboard_vector != 0x00) return;

//     outb(PS2_KEYBOARD_COMMAND_PORT, 0xae);
//     keyboard_reset_state();
    
//     keyboard_vector = idt_allocate_vector();
//     idt_install_irq(keyboard_vector, (void*)&keyboard_handler);
//     set_mask(FALSE);
//     DEBUG("d");
// }
