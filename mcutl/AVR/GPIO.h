
#pragma once

#include <BaseGPIO.h>
#include <avr/io.h>

#define AVR_DEFINE_PORT(Name) \
	namespace avr { \
		DEFINE_REGISTER(Port##Name,PORT##Name,uint8_t) \
		DEFINE_REGISTER(Ddr##Name,DDR##Name,uint8_t) \
		DEFINE_REGISTER(Pin##Name,PIN##Name,uint8_t) \
	} \
	typedef avr::NativePort<avr::Port##Name,avr::Ddr##Name,avr::Pin##Name> Port##Name; \
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
	namespace avr
	{
		template<class Port,class Ddr,class Pin>
		struct NativePort
		{
			typedef uint8_t data_type;

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

#ifdef PORTA
	AVR_DEFINE_PORT(A)
#endif // PORTA

#ifdef PORTB
	AVR_DEFINE_PORT(B)
#endif // PORTB

#ifdef PORTC
	AVR_DEFINE_PORT(C)
#endif // PORTC

#ifdef PORTD
	AVR_DEFINE_PORT(D)
#endif // PORTD

#ifdef PORTE
	AVR_DEFINE_PORT(E)
#endif // PORTE

#ifdef PORTF
	AVR_DEFINE_PORT(F)
#endif // PORTF

/////////////////////////////////////////////////////////////////////////
// Arduino Uno
/////////////////////////////////////////////////////////////////////////

#ifdef __AVR_ATmega328__
	typedef PinC0 A0;
	typedef PinC1 A1;
	typedef PinC2 A2;
	typedef PinC3 A3;
	typedef PinC4 A4;
	typedef PinC5 A5;

	typedef PinD0 D0;
	typedef PinD1 D1;
	typedef PinD2 D2;
	typedef PinD3 D3;
	typedef PinD4 D4;
	typedef PinD5 D5;
	typedef PinD6 D6;
	typedef PinD7 D7;
	typedef PinB0 D8;
	typedef PinB1 D9;
	typedef PinB2 D10;
	typedef PinB3 D11;
	typedef PinB4 D12;
	typedef PinB5 D13;
#endif
}