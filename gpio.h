#ifndef GPIO_H
#define GPIO_H

#include <avr/io.h>

#define MAKE_AVR_PORT(Name) \
	class Port##Name \
{ \
	public: \
	inline static void setDirection( uint8_t data )            { DDR##Name = data;  } \
	inline static void write( uint8_t data )                   { PORT##Name = data; } \
	inline static void set( uint8_t data )                     { PORT##Name |= data; } \
	inline static void clear( uint8_t data )                   { PORT##Name &= ~data; } \
	inline static void maskedSet( uint8_t mask, uint8_t data ) { PORT##Name = (PORT##Name & ~mask) | data; } \
	inline static uint8_t read()                               { return PIN##Name;  } \
	};

#ifdef PORTA
MAKE_AVR_PORT(A)
#endif // PORTA

#ifdef PORTB
MAKE_AVR_PORT(B)
#endif // PORTB

#ifdef PORTC
MAKE_AVR_PORT(C)
#endif // PORTC

#ifdef PORTD
MAKE_AVR_PORT(D)
#endif // PORTD

#ifdef PORTE
MAKE_AVR_PORT(E)
#endif // PORTE

#ifdef PORTF
MAKE_AVR_PORT(F)
#endif // PORTF

#endif // GPIO_H
