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


// helper function
char digit2hex( uint8_t digit)
{
    char result;
    result = digit < 10 ? '0' : 'A' - 10;
    result += digit;
    return result;
}



void main( void)
{
    far uint8_t * bws = (far uint8_t *) 0xFE00;
    uint8_t dat;
    char str[] = "HALLO USER OF JU+TE TINY!\r\n";

    clear_screen();
    puts( str);

    dat = key();

    // print code for pressed key
    putc( digit2hex(dat >> 4));
    putc( digit2hex(dat & 0xF));

    // wait for another keypress
    dat = key();
}
