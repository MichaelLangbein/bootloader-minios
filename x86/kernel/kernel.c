#include "../drivers/screen.h"


void main() {
    // point to first text-cell of VGA MMIO video memory.
    // char* video_memory = 0xb8000;
    // *video_memory = 'X';

    clear_screen();
    print_at("hi", 1, 1);

    print_char('A', 0, 0, WHITE_ON_BLACK);
    print_char('B', 1, 1, WHITE_ON_BLACK);
    print_char('C', 2, 2, WHITE_ON_BLACK);
    print_char('D', 3, 3, WHITE_ON_BLACK);
    
}