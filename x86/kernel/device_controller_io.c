
unsigned char port_byte_in(unsigned short port) {


    unsigned char result;

    /**
     * "=a" (result) means: put AL register in variable RESULT when done
     * "d" (port)   means: load edx with port
     * 
     * 
     * Yields:
     * ```asm
     *  mov edx, <port>     ; "d" (port)
     *  in al, edx          ; 
     *  mov [result], al    ; "=a" (result)
     * ```
    */
    
    __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
    
    return result;
}


void port_byte_out(unsigned short port, unsigned char data) {

    /**
     * "a" (data)   means:  load EAX with data
     * "d" (port)   means: load EDX with port
     * 
     * Yields:
     * ```asm
     *  mov eax, data
     *  mov edx, <port>
     *  out edx, eal
     * 
    */
    __asm__("out %%al, %%dx" : : "a" (data), "d" (port));
}


unsigned short port_word_in(unsigned short port) {
    unsigned short result;
    __asm__("in %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
}


void port_word_out(unsigned short port, unsigned short data) {
    __asm__("out %%ax, %%dx" : : "a" (data), "d" (port));
}