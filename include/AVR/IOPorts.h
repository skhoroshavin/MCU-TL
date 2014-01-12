
#pragma once

#include <avr/io.h>

#define MAKE_AVR_PORT(Name) \
	class Port##Name \
	{ \
	public: \
		inline static void setDir( uint8_t data )                     { DDR##Name = data;  } \
		inline static void setOutput( uint8_t data )                  { DDR##Name |= data;  } \
		inline static void setInput( uint8_t data )                   { DDR##Name &= ~data;  } \
		inline static void maskedSetDir( uint8_t mask, uint8_t data ) { DDR##Name = (DDR##Name & ~mask) | data; } \
	\
		inline static void write( uint8_t data )                      { PORT##Name = data; } \
		inline static void set( uint8_t data )                        { PORT##Name |= data; } \
		inline static void clear( uint8_t data )                      { PORT##Name &= ~data; } \
		inline static void toggle( uint8_t data )                     { PIN##Name = data; } \
		inline static void maskedSet( uint8_t mask, uint8_t data )    { PORT##Name = (PORT##Name & ~mask) | data; } \
	\
		inline static uint8_t read()                                  { return PIN##Name;  } \
	}; \
	typedef Pin<Port##Name,0> Pin##Name##0; \
	typedef Pin<Port##Name,1> Pin##Name##1; \
	typedef Pin<Port##Name,2> Pin##Name##2; \
	typedef Pin<Port##Name,3> Pin##Name##3; \
	typedef Pin<Port##Name,4> Pin##Name##4; \
	typedef Pin<Port##Name,5> Pin##Name##5; \
	typedef Pin<Port##Name,6> Pin##Name##6; \
	typedef Pin<Port##Name,7> Pin##Name##7;

namespace gpio
{

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

}
