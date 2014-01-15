
#pragma once

#include <avr/io.h>
#include <avr/interrupt.h>

#include <BaseGPIO.h>
#include <BaseStream.h>

#define AVR_DEFINE_USART(Num) \
	namespace avr { \
		struct UsartReg##Num \
		{ \
			DEFINE_REGISTER(Udr,UDR##Num,uint8_t) \
			DEFINE_REGISTER(UbrrL,UBRR##Num##L,uint8_t) \
			DEFINE_REGISTER(UbrrH,UBRR##Num##H,uint8_t) \
			DEFINE_REGISTER(UcsrA,UCSR##Num##A,uint8_t) \
			DEFINE_REGISTER(UcsrB,UCSR##Num##B,uint8_t) \
			DEFINE_REGISTER(UcsrC,UCSR##Num##C,uint8_t) \
		}; \
	} \
	typedef avr::Usart<avr::UsartReg##Num> Usart##Num;

#if defined(UDR) && !defined(UDR0)
#define UDR0   UDR
#define UBRR0L UBRRL
#define UBRR0H UBRRH

#define UCSR0A UCSRA
#define RXC0   RXC
#define TXC0   TXC
#define UDRE0  UDRE
#define U2X0   U2X

#define UCSR0B  UCSRB
#define RXCIE0  RXCIE
#define TXCIE0  TXCIE
#define UDRIE0  UDRIE
#define RXEN0   RXEN
#define TXEN0   TXEN

#define UCSR0C  UCSRC
#define USBS0   USBS
#define UCSZ01  UCSZ1
#define UCSZ00  UCSZ0

#endif // UDR

namespace stream
{
	namespace avr
	{

	template<class Regs> class Usart
	{
	public:
		template<uint32_t rate>
		inline static void setBaudRate()
		{
			const uint16_t ubrr1x = (F_CPU/16/rate-1);	// 16
			const uint16_t ubrr2x = (F_CPU/8/rate-1);	// 33
			const uint32_t rate1x = (F_CPU/16/(ubrr1x+1));	// 58823
			const uint32_t rate2x = (F_CPU/8/(ubrr2x+1));	// 58823
			const uint32_t err1 = rate > rate1x ? (rate - rate1x) : (rate1x - rate);
			const uint32_t err2 = rate > rate2x ? (rate - rate2x) : (rate2x - rate);

			if( err2 < err1 )
			{
				Regs::UcsrA::write( (1 << U2X0) );
				Regs::UbrrH::write( (uint8_t)(ubrr2x >> 8) );
				Regs::UbrrL::write( (uint8_t)ubrr2x );
			}
			else
			{
				Regs::UcsrA::write( 0 );
				Regs::UbrrH::write( (uint8_t)(ubrr1x >> 8) );
				Regs::UbrrL::write( (uint8_t)ubrr1x );
			}
		}

		inline static void start()
		{
			Regs::UcsrC::write( (1 << USBS0) | (1 << UCSZ00) | (1 << UCSZ01) );	// 8 data bits, 2 stop bits
			Regs::UcsrB::write( (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0) ); // start interface and enable interrupts
		}

		inline static void stop()
		{
			Regs::UcsrB::write( 0 );
			Regs::UcsrC::write( 0 );
			Regs::UcsrA::write( 0 );
		}

		inline static bool sendReady() { return Regs::UcsrA::read() & (1 << UDRE0); }
		inline static void send( char c ) { Regs::Udr::write( c ); }
		inline static void setSendIRQ( bool enabled ) { if( enabled ) Regs::UcsrB::set( 1 << UDRIE0 ); else Regs::UcsrB::clear( 1 << UDRIE0 ); }

		inline static int  recvAvailable() { return (bool)(Regs::UcsrA::read() & (1 << RXC0)); }
		inline static char recv() { return Regs::Udr::read(); }
	};

	}

#ifdef UDR0
AVR_DEFINE_USART(0)

#ifdef USART_RX_vect
#define IRQ_USART0_RECV ISR(USART_RX_vect)
#define IRQ_USART0_SEND ISR(USART_UDRE_vect)
#else // USART_RX_vect
#define IRQ_USART0_RECV ISR(USART0_RX_vect)
#define IRQ_USART0_SEND ISR(USART0_UDRE_vect)
#endif // USART_RX_vect

#endif // UDR0

#ifdef UDR1
AVR_DEFINE_USART(1)
#define IRQ_USART1_RECV ISR(USART1_RX_vect)
#define IRQ_USART1_SEND ISR(USART1_UDRE_vect)
#endif // UDR1

#ifdef UDR2
AVR_DEFINE_USART(2)
#define IRQ_USART2_RECV ISR(USART2_RX_vect)
#define IRQ_USART2_SEND ISR(USART2_UDRE_vect)
#endif // UDR2

#ifdef UDR3
AVR_DEFINE_USART(3)
#define IRQ_USART3_RECV ISR(USART3_RX_vect)
#define IRQ_USART3_SEND ISR(USART3_UDRE_vect)
#endif // UDR3

}
