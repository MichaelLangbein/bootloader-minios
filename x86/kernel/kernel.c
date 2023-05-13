#include "../drivers/screen.h"


void main() {
    // point to first text-cell of VGA MMIO video memory.
    // char* video_memory = (char*) 0xb8000;
    // *video_memory = 'X';

    clear_screen();
    print_at("hi", 1, 1);
    
}