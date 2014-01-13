
#pragma once

#include <avr/io.h>

#include <IOReg.h>

namespace gpio
{
	namespace avr
	{
		template<class Port,class Ddr,class Pin>
		class IOPort
		{
		public:
			inline static void setDir( uint8_t data )                     { Ddr::write( data ); }
			inline static void setOutput( uint8_t data )                  { Ddr::set( data ); }
			inline static void setInput( uint8_t data )                   { Ddr::clear( data ); }
			inline static void maskedSetDir( uint8_t mask, uint8_t data ) { Ddr::maskedSet( mask, data ); }

			inline static void write( uint8_t data )                   { Port::write( data ); }
			inline static void set( uint8_t data )                     { Port::set( data ); }
			inline static void clear( uint8_t data )                   { Port::clear( data ); }
			inline static void toggle( uint8_t data )                  { Pin::write( data ); }
			inline static void maskedSet( uint8_t mask, uint8_t data ) { Port::maskedSet( mask, data ); }

			inline static uint8_t read() { return Pin::read();  }
		};
	}
}

#define MAKE_AVR_PORT(Name) \
	namespace avr { \
		DEFINE_REGISTER(Port##Name,PORT##Name,uint8_t) \
		DEFINE_REGISTER(Ddr##Name,DDR##Name,uint8_t) \
		DEFINE_REGISTER(Pin##Name,PIN##Name,uint8_t) \
	} \
	typedef avr::IOPort<avr::Port##Name,avr::Ddr##Name,avr::Pin##Name> Port##Name; \
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
