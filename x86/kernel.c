void main() {
    // point to first text-cell of VGA MMIO video memory.
    char* video_memory = (char*) 0xb8000;
    *video_memory = 'X';
}