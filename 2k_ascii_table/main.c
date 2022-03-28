#include <z8.h>
#include <stdio.h>

/*
 * near -> rdata = register file
 * far  -> edata = external memory
 */

// no inttype.h, no stdint.h
typedef   signed char  int8_t;
typedef unsigned char  uint8_t;
typedef   signed short int16_t;
typedef unsigned short uint16_t;

// tiny2k internal varaibles
#define TINY_CHR (*(unsigned char near volatile *)0x5a)
#define TINY_POS (*(unsigned char near volatile *)0x5b)


// tiny2k monitor functions
void clear_screen( void)
{
    asm( "call %08dd"); // cls, geht
}


void cursor_home( void)
{
    TINY_POS = 0;
}


void putc( char c)
{
    TINY_CHR = c;
    asm( "call %0827"); // cbs
}


int puts( char near *s)
{
    while( *s)
    {
        putc( *s);
        s++;
    }
}


char key( void)
{
    asm( "call %0c1b"); // tsk
    return TINY_CHR;
}



void main( void)
{
    uint8_t dat;

    cursor_home();
    for( dat = 32; dat < 96; dat++)
    {
        putc( dat);
    }

    // wait for keypress
    dat = key();
}
