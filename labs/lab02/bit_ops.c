#include <stdio.h>
#include "bit_ops.h"

// Return the nth bit of x.
// Assume 0 <= n <= 31
unsigned get_bit(unsigned x,
                 unsigned n) {
    // YOUR CODE HERE
    // Returning -1 is a placeholder (it makes
    // no sense, because get_bit only returns 
    // 0 or 1)
    if( n < 32 ) {
        // if n is valid
        x = x >> n;
        return ( x & 1 );
    }
    return -1;
}
// Set the nth bit of the value of x to v.
// Assume 0 <= n <= 31, and v is 0 or 1
void set_bit(unsigned * x,
             unsigned n,
             unsigned v) {
    // YOUR CODE HERE
    // 1 << n 会生成一个值，只有第n位是1，其余位是0;
    *x &= ~(1 << n);        // 使用掩码清除第n位；
    *x |= (v << n);         // 如果v为1，将第n位设置为1；
}
// Flip the nth bit of the value of x.
// Assume 0 <= n <= 31
void flip_bit(unsigned * x,
              unsigned n) {
    // YOUR CODE HERE
    if( get_bit(*x, n) ) set_bit( x, n, 0 );
    else set_bit( x, n, 1 );
}