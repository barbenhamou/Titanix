#include "../../include/lib/monitor.h"

void putch(char c) {
    outb(0x3f8, c);
}

// // Stores the cursor position.
static uint8_t cursor_x;
static uint8_t cursor_y;
static uint16_t* terminal_buffer = (uint16_t*)0xb8000; 

static word_t create_cell(char c, uint8_t fore_color, uint8_t back_color) {
    word_t word = 0;
    byte_t upper = 0, lower = 0;

    upper = back_color;
    upper <<= 4;
    upper |= fore_color;
    word = upper;
    word <<= 8;
    lower = c;
    word |= lower;

    return word;
}

static word_t blank() {
    return create_cell(' ', white , black);
}

// // Updates the hardware cursor.
static void move_cursor() {
    uint16_t pos = cursor_y * 80 + cursor_x;
    outb(0x3D4, 14);
    outb(0x3D5, ((pos >> 8) & 0x00FF));
    outb(0x3D4, 15);
    outb(0x3D5, pos & 0x00FF);
}

static void scroll() {
    // Need to scroll.
    if(cursor_y >= 25) {
        int i;
        for (i = 0*80; i < 24*80; i++) {
            terminal_buffer[i] = terminal_buffer[i+80];
        }

        // The last line should now be blank. Do this by writing
        // 80 spaces to it.
        for (i = 24*80; i < 25*80; i++) {
            terminal_buffer[i] = blank();
        }
        // The cursor should now be on the last line.
        cursor_y = 24;
    }
}

static void monitor_put(char c) {
    uint8_t backColour = 0;
    uint8_t foreColour = 15;

    // Handle a backspace, by moving the cursor back one space
    if (c == 0x08 && cursor_x) {
        cursor_x--;
        terminal_buffer[cursor_x + 80 * cursor_y] = blank();
    }

    // Handles a tab, moving to position divisable by 8.
    else if (c == 0x09) {
        cursor_x = (cursor_x+8) & ~(8-1);
    }

    // Handles carriage return.
    else if (c == '\r') {
        cursor_x = 0;
    }

    // Handles newline.
    else if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    }

    // Handle any other printable character.
    else if(c > ' ' || c == ' ' || ('9' > c && c > '0'))  {
        uint8_t pos = cursor_x + 80 * cursor_y;
        terminal_buffer[cursor_x + 80 * cursor_y] = create_cell(c, white, black);

        cursor_x++;
    }

    // New line is needed.
    if (cursor_x >= 80) {
        cursor_x = 0;
        cursor_y ++;
    }

    // Scroll the screen if needed.
    scroll(terminal_buffer);

    // Move the hardware cursor.
    move_cursor();

}

static void monitor_clear() {
    for (int i = 0; i < 25*80; i++) {
        terminal_buffer[i] = blank();
    }

    // Move the hardware cursor back to the start.
    cursor_x = 0;
    cursor_y = 0;
    move_cursor();
}

static void monitor_write(char *c) {
    int i = 0;
    while (c[i]) {
        monitor_put(c[i++]);
    }
}

void monitor_put_hex(uint32_t n) {  
    monitor_write("0x");
    if (n == 0) {
        monitor_write("0");
        return;
    }

    uint32_t num = n, m = 0, i = 0;
    char_t hex[8];


    while (num > 0) {
        m = (num%16);
        if (m == 10) {
            hex[i] = 'a';
        } else if (m == 11) {
            hex[i] = 'b';
        } else if (m == 12) {
            hex[i] = 'c';
        } else if (m == 13) {
            hex[i] = 'd';
        } else if (m == 14) {
            hex[i] = 'e';
        } else if (m == 15) {
            hex[i] = 'f';
        } else {
            hex[i] = '0' + m;
        }
        num /= 16;
        ++i;
    }

    for (int j = 0; j < 8; ++j) {
            monitor_put(hex[7-j]);
    }
}

void monitor_put_dec(uint32_t n) {
    uint32_t num = n, i = 0, m;
    char_t dec[10];
    while (num > 0) {
        m = num % 10;
        dec[i] = '0' + m;
        num /= 10;
        ++i;
    }
    
    for (int j = 0; j < 8; ++j) {
            monitor_put(dec[7-j]);
    }
}

void init_monitor() {
    cursor_x = 0, cursor_y = 0;
    monitor_clear();
}
