#include "../drivers/screen.h"


void main() {
    // point to first text-cell of VGA MMIO video memory.
    // char* video_memory = 0xb8000;
    // *video_memory = 'X';

    clear_screen();
    print_at("hi there, this is a message", 1, 1);

    print_at('AAAA', 0, 0);
    print_at('BBBB', 1, 1);
    print_at('CCCC', 2, 2);
    print_at('DDDD', 3, 3);
    
}