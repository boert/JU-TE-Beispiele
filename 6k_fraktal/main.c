#include <z8.h>
#include <stdio.h>
#include <string.h> // memset
#include <math.h>

//#define float   double

/*
 * far  -> edata = external memory
 */

/*
 * Sprungtablle
 *
 * Adr.  Name       Funktion
 * %0812 KOMMAND    Einsprung nach RESET
 * %0815 CHARIN     liefert in %13 die Zeichen einer Zeile
 * %0818 CHAROUT    Zeichen in %15 ausgeben   
 * %081B KEY        ASCII-Code in %6D
 * %081E WKEY       wartet mit Tastencode in %13
 * %0821 SAVE       Speicherbereich auf Kassette speichern
 * %0824 LOAD       lädt Daten von Kasetten
 * %0827 SCRFUN     X, Y, Z
 *                      Z=0 Cursorpositioni lesen,
 *                      Z=1 Cursorposition setzen,
 *                      Z=2 ASCII-Core rücklesen,
 *                      
 * %082A MONITOR    ruft den Maschinenmonitor MON auf
 * %082D PRISTRI    Zeichenkette über CHAROUT ausgeben
 * %0830 SHOWPLAYER 
 * %0833 HIDEPLAYER
 * %0836 RND        liefert Zufallszahl in %74/%75
 *
 *
 * zusätzliche Systemfunktionen beim 6k-Monitor
 *
 * %0AF7 PMON druckt "Mon" und einen Returncode
 * %0C69 HTA16 druckt die 16 Bit aus %18/19
 * %0C72 HTA8 druckt die 8 Bit aus %19
 * %0C7B HTA4 druckt die niederen 4 Bit aus %19
 * %0C8D PRRET druckt einen Return-Kode aus (%0D)
 * %0C91 RWCONT druckt einen Return-Kode aus und wartet dann auf Tastendruck
 * %0C9B PCAS druckt das Zeichen aus %15 + 16 Bit aus %1A/1B als + Leerzeichen
 * %0CA9 ADRE wandelt die 4 Ziffern ab Adresse (%1E/1F) nach %1C/1D
 * %0CB8 ATH4 wandelt die Ziffer von (%1E/1F) in niedere 4 Bit von %1D,
 * %0CD5 ATH16 wandelt 4 Ziffern ab (%1E/1F) nach %1C/1D
 * %OCDC ATH8 wandelt 2 Ziffern ab (%1E/1F) nach %1D
 * %0A52 DAXTH16 wandelt eine 1 bis 5 Ziffern lange Dezimalzahl
 *
 * Grafikroutinen
 *  %17F7 DRAW
 *  %17FA PTEST
 *  %17FD PLOT
 *
 * freie Register:
 * %20 bis %54
 */

// no inttype.h, no stdint.h
typedef   signed char  int8_t;
typedef unsigned char  uint8_t;
typedef   signed short int16_t;
typedef unsigned short uint16_t;


// Prototypes
void putc( char c);
int puts( char *s);
void clear_screen( void);
void cursor_home( void);
void cursor_set( uint8_t x, uint8_t y);
void new_line( void);
char key( void);
char wkey( void);
void puthex( uint8_t value);
void puthex16( uint16_t value);
void set_pixel( uint16_t x, uint8_t y, uint8_t color);
void clear_pixel( uint16_t x, uint8_t y);
void line( uint16_t x1, uint8_t y1, uint16_t x2, uint8_t y2, uint8_t color);
void draw_frame( void);

#define TICK    (*(unsigned char near volatile *)0x6f)

void clear_screen( void)
{
#if 1
    putc( 0x0c);
#else
    // optimiertes löschen
    // 170 ms statt 500 ms
    uint16_t addr;
    void far * st = (void far *)0x4000;
    addr = 0x6000 + (2 << 4);
    (*(unsigned char far volatile *) addr) = 0;
    memset( st, 0x00, 7680);
    addr = 0x6000 + ((~2 & 0x0f) << 4);
    (*(unsigned char far volatile *) addr) = 0;
    memset( st, 0xFF, 7680);
#endif
}

#define VAR_X16 (*(unsigned int  near volatile *)0x4e)
#define VAR_X8  (*(unsigned char near volatile *)0x4f)
#define VAR_Y8  (*(unsigned char near volatile *)0x51)
#define VAR_Z8  (*(unsigned char near volatile *)0x53)

// tiny2k 64*64 Bildspeicher
// tiny6k 320x240 Bildspeicher
far uint8_t * bws = (far uint8_t *) 0x4000;


void cursor_home( void)
{
    putc( 0x05);
}

void cursor_set( uint8_t x, uint8_t y)
{
    VAR_X8 = x;
    VAR_Y8 = y;
    VAR_Z8 = 1;
    asm( "call %0827"); // SCRFUN
}

void new_line( void)
{
    putc( 0x0d);
    putc( 0x0a);
}



void putc( char c)
{ 
    (*(unsigned char near volatile *)0x15) = c;
    asm( "call %0818"); // CHAROUT
}


int puts( char *s)
{
    while( *s)
    {
        putc( *s);
        s++;
    }
}

char key( void)
{
    asm( "call %81b"); // KEY
    return (*(unsigned char near volatile *)0x6D);
}

char wkey( void)
{
    asm( "call %81e"); // WKEY
    return (*(unsigned char near volatile *)0x13);
}




void set_pixel( uint16_t x, uint8_t y, uint8_t color)
{
    VAR_X16 = x;
    VAR_Y8  = y;
    VAR_Z8  = color;
    asm( "call %17FD"); // PLOT
}


void clear_pixel( uint16_t x, uint8_t y)
{
    set_pixel( x, y, 0);
}


char digit2hex( uint8_t digit)
{
    char result;
    result = digit < 10 ? '0' : 'A' - 10;
    result += digit;
    return result;
}


void puthex8( uint8_t value)
{
    putc( digit2hex(value >> 4));
    putc( digit2hex(value & 0xF));
}


void puthex16( uint16_t value)
{
    putc( digit2hex(( value >> 12)& 0xF));
    putc( digit2hex(( value >> 8 )& 0xF));
    putc( digit2hex(( value >> 4 )& 0xF));
    putc( digit2hex(( value >> 0 )& 0xF));
}


void putuint( uint16_t value)
{
    uint16_t divisor = 1;
    while( value / divisor > 9)
    {
        divisor *= 10;
    }
    do
    {
        putc((char)('0' + (value / divisor % 10)));
        divisor /= 10;
    } while ( divisor > 0);
}


void putint( int16_t value)
{
    uint16_t divisor = 1;
    if( value < 0)
    {
        putc( '-');
        value = -value;
    }
    putuint( value);
}


uint8_t teste( const float x, const float y)
{
    float  t;
    float  a;
    float  b;
    float  m;
    uint8_t z;

    a = b = m = 0.0;
    z = 16;

    while(( z > 0) && ( m < 4.0))
    {
        z--;
        t = a * a - b * b;
        b = 2.0 * a * b;
        a = t;
        a = a + x;
        b = b + y;
        m = a * a + b * b;
#if 0
        // debug
        putint( z); puts( ": ");
        putint( a); putc( ' ');
        putint( b); new_line();
#endif
    }
    return z;
}

// voll
#define MAX_X 320
#define MAX_Y 192

// viertel
//#define MAX_X 160
//#define MAX_Y 100
//

void main( void)
{
    const float x_min = -2.0;
    const float x_max =  0.5;
    
    const float y_min = -1.0;
    const float y_max =  1.0;

    const float x_len = x_max - x_min;
    const float y_len = y_max - y_min;

    uint8_t  color;
    uint8_t  zeile;
    uint16_t spalte;

    float   x;
    float   y;

    clear_screen();
    
    x = x_min;
    for( spalte = 0; spalte < MAX_X; spalte++)
    {
        y = y_min;
        for( zeile = 0; zeile < MAX_Y; zeile++)
        {
            color = teste( x, y);
            set_pixel( spalte, zeile, color);
            
            y = y + y_len / MAX_Y;
        }
        x = x + x_len / MAX_X;
        if( key() != 0)
        {
            break;
        }
    }
    
    while( key() == 0) {};
}
