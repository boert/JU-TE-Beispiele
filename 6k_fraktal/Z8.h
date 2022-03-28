/*======================================================================
								       
		     Copyright (C) 1987-1992 by                        
		  Production Languages Corporation                     
			All Rights Reserved                            
								       
			    P.O. Box 109                               
		    Weatherford, Texas 76086-0109                      
			   (817) 599-8363                              
								       

========================================================================
				Z8

				<Z8.h>

=======================================================================*/
#ifndef IZ8_H
#define IZ8_H

/*      Interrupt vectors */

#define RESET   0               /* Reset interrupt */
#define IRQ0    1               /* Interrupt request 0 */
#define IRQ1    2               /* Interrupt request 1 */
#define IRQ2    3               /* Interrupt request 2 */
#define IRQ3    4               /* Interrupt request 3 */
#define IRQ4    5               /* Interrupt request 4 */
#define IRQ5    6               /* Interrupt request 5 */

/*      Special Function Registers */

#define SPL     (*(unsigned char near volatile *)0xff)
#define SPH     (*(unsigned char near volatile *)0xfe)
#define RP      (*(unsigned char near volatile *)0xfd)
#define FLAGS   (*(unsigned char near volatile *)0xfc)
#define IMR     (*(unsigned char near volatile *)0xfb)
#define IRQ     (*(unsigned char near volatile *)0xfa)
#define IPR     (*(unsigned char near volatile *)0xf9)
#define P01M    (*(unsigned char near volatile *)0xf8)
#define P3M     (*(unsigned char near volatile *)0xf7)
#define P2M     (*(unsigned char near volatile *)0xf6)
#define PRE0    (*(unsigned char near volatile *)0xf5)
#define T0      (*(unsigned char near volatile *)0xf4)
#define PRE1    (*(unsigned char near volatile *)0xf3)
#define T1      (*(unsigned char near volatile *)0xf2)
#define TMR     (*(unsigned char near volatile *)0xf1)
#define SIO     (*(unsigned char near volatile *)0xf0)

#define PCON    (*(unsigned char near volatile *)0x00)
#define SMR     (*(unsigned char near volatile *)0x0b)
#define WDTMR   (*(unsigned char near volatile *)0x0f)

#define P0      (*(unsigned char near volatile *)0x00)
#define P1      (*(unsigned char near volatile *)0x01)
#define P2      (*(unsigned char near volatile *)0x02)
#define P3      (*(unsigned char near volatile *)0x03)

/*      Register Enumerations   */

#define R0      1
#define R1      2
#define R2      3
#define R3      4
#define R4      5
#define R5      6
#define R6      7
#define R7      8
#define R8      9
#define R9      10
#define R10     11
#define R11     12
#define R12     13
#define R13     14
#define R14     15
#define R15     16
#define RR0     17
#define RR2     18
#define RR4     19
#define RR6     20
#define RR8     21
#define RR10    22
#define RR12    23
#define RR14    24

/*      Macros to enable and disable interrupts */

intrinsic void EI(void);
intrinsic void DI(void);
intrinsic void SET_VECTOR(int vect,void (*hndlr)(void));
intrinsic char GET_REG(int regnum);
intrinsic short GET_RREG(int regnum);
intrinsic void SET_REG(int regnum,char value);
intrinsic void SET_RREG(int regnum,short value);
intrinsic void SET_ERF(unsigned char erfname,unsigned char value);
intrinsic unsigned char GET_ERF(unsigned char value);
#endif
