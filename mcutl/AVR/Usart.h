
#pragma once

#include <avr/io.h>
#include <avr/interrupt.h>

#include <BaseGPIO.h>

#define AVR_DEFINE_USART_REGS(ClassName,RegName) \
	struct ClassName \
	{ \
		DEFINE_REGISTER(UbrrL,UBR##RegName##L,uint8_t) \
		DEFINE_REGISTER(UbrrH,UBR##RegName##H,uint8_t) \
		DEFINE_REGISTER(UcsrA,UCS##RegName##A,uint8_t) \
		DEFINE_REGISTER(UcsrB,UCS##RegName##B,uint8_t) \
		DEFINE_REGISTER(UcsrC,UCS##RegName##C,uint8_t) \
		DEFINE_REGISTER(Udr,UD##RegName,uint8_t) \
	};

namespace serial
{
	namespace avr
	{
#ifdef UDR
		AVR_DEFINE_USART_REGS(Usart0,R)
#else // UDR

#ifdef UDR0
		AVR_DEFINE_USART_REGS(Usart0,R0)
#endif // UDR

#ifdef UDR1
		AVR_DEFINE_USART_REGS(Usart1,R1)
#endif // UDR

#endif // UDR
	};

	template<int TxSize,int RxSize,class Regs = avr::Usart0> class Usart
	{
		static RingBuffer<RxSize> _rx;
		static RingBuffer<TxSize> _tx;
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
			Regs::UcsrB::write( (1 << RXCIE0) | (1 << UDRIE0) | (1 << RXEN0) | (1 << TXEN0) ); // start interface and enable interrupts
		}

		inline static void stop()
		{
			Regs::UcsrB::write( 0 );
			Regs::UcsrC::write( 0 );
			Regs::UcsrA::write( 0 );

			_rx.clear();
			_tx.clear();
		}

		static bool send( uint8_t c )
		{
			if( _tx.isFull() )
				return false;

			if( _tx.isEmpty() )
			{
				while( !(Regs::UcsrA::read() & (1 << UDRE0)) );

				Regs::Udr::write( c );
				Regs::UcsrB::set( 1 << UDRIE0 );
			}
			else
				_tx.push_back( c );

			return true;
		}

		template<typename Str>
		static bool sendString( Str s )
		{
			while( char c = *s )
			{
				if( !send( c ) )
					return false;
				++s;
			}
			return true;
		}

		static uint8_t bytesReceived() { return _rx.count(); }
		static uint8_t recv() { return _rx.pop_front(); }

		static inline void udreHandler()
		{
			if( _tx.isEmpty() ) Regs::UcsrB::clear( 1 << UDRIE0 );
			else Regs::Udr::write( _tx.pop_front() );
		}

		static inline void rxHandler()
		{
			uint8_t c = Regs::Udr::read();
			if( !_rx.isFull() )
				_rx.push_back( c );
		}
	};

	template<int TxSize,int RxSize,class Regs> RingBuffer<RxSize> Usart<TxSize,RxSize,Regs>::_rx;
	template<int TxSize,int RxSize,class Regs> RingBuffer<TxSize> Usart<TxSize,RxSize,Regs>::_tx;
}
