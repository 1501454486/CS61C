#include <stdio.h>

int factorial( int n );

int main()
{
    
}

int factorial( int n )
{
    int i = n - 1;
    while( i ) {
        n *= i;
        i--;
    }
    return n;
}