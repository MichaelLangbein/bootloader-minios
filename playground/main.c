#include <stdio.h>


/**
 * pos: 3 2 1 0
 * val: 8 4 2 1
 * 
 * (1 << 3) 
 * === put a one into the 4th position
 * ==> yields 8
 * 
 * int a = (1 << 3);
 * int b = (1 << 0);
 * a | b
 * === |: put a one in resulting bin if there is on at that location in either a or b.
 * === put one into 4th and one into 0th position
 * ==> yields 9
 * 
 * int a = (1 << 3) | (1 << 2);
 * int b = (1 << 2);
 * a & b
 * === &: put a 1 in resulting bin if there is one at that location in both a and b.
*/


int main() {
    int a = (1 << 3) | (1 << 2);
    int b = (1 << 1);
    printf("Value: %i\n", a & b);
    return 0;
}