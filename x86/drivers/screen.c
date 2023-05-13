#include "./screen.h"
#include "../kernel/device_controller_io.h"

void print_char(char character, int col, int row, char attribute_byte) {

    // create a byte (char) pointer to start of video memory
    unsigned char* vidmem = (unsigned char *) VIDEO_ADDRESS;

    // if attribute_byte is zero, assume default style
    if (!attribute_byte) {
        attribute_byte = WHITE_ON_BLACK;
    }

    // get vidmem-offset for screen-location
    int offset;
    if (col >= 0 && row >= 0) {
        offset = get_screen_offset(col, row);
    } else {
        offset = get_cursor();
    }

    // special case: newline char
    if (character == '\n') {
        int rows = offset / (2 * MAX_COLS);
        offset = get_screen_offset(79, rows);
    }

    // otherwise, write to vidmem
    else {
        vidmem[offset] = character;
        vidmem[offset + 1] = attribute_byte;
    }

    offset += 2;

    offset = handle_scrolling(offset);
    set_cursor(offset);
}


int get_screen_offset(int col, int row) {
    return (MAX_COLS * row + col) * 2;
}

int get_cursor() {
    // reg 14: high byte of cursor's offset
    // reg 15: low byte of cursor's offset
    
    port_byte_out(REG_SCREEN_CTRL, 14);
    int offset = port_byte_in(REG_SCREEN_DATA) << 8;
    port_byte_out(REG_SCREEN_CTRL, 15);
    offset += port_byte_in(REG_SCREEN_DATA);

    // mult by two to account for styling
    return offset * 2;
}

int set_cursor(int offset) {
    offset /= 2;    // div by 2 to account for styling

    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
    port_byte_out(REG_SCREEN_CTRL, 15);
}

int handle_scrolling(int offset) {
    return offset;
}

void print_at(char* message, int col, int row) {
    if (col >= 0 && row >= 0) {
        set_cursor(get_screen_offset(col, row));
    }
    int i = 0;
    while (message[i] != 0) {
        i += 1;
        print_char(message[i], col, row, WHITE_ON_BLACK);
    }
}

void print(char* message) {
    print_at(message, -1, -1);
}

void clear_screen() {
    int row = 0;
    int col = 0;

    for (row = 0; row < MAX_ROWS; row++) {
        for (col = 0; col < MAX_COLS; col++) {
            print_char(' ', col, row, WHITE_ON_BLACK);
        }
    }

    set_cursor(get_screen_offset(0, 0));
}