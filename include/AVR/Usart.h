
#pragma once

namespace avr
{
	DEFINE_REGISTER(UbrrL,UBRR0L,uint8_t)
	DEFINE_REGISTER(UbrrH,UBRR0H,uint8_t)
	DEFINE_REGISTER(UcsrA,UCSR0A,uint8_t)
	DEFINE_REGISTER(UcsrB,UCSR0B,uint8_t)
	DEFINE_REGISTER(UcsrC,UCSR0C,uint8_t)
	DEFINE_REGISTER(Udr,UDR0,uint8_t)

	template<int TxSize,int RxSize> class Usart
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
				UcsrA::write( (1 << U2X0) );
				UbrrH::write( (uint8_t)(ubrr2x >> 8) );
				UbrrL::write( (uint8_t)ubrr2x );
			}
			else
			{
				UcsrA::write( 0 );
				UbrrH::write( (uint8_t)(ubrr1x >> 8) );
				UbrrL::write( (uint8_t)ubrr1x );
			}
		}

		inline static void start()
		{
			UcsrC::write( (1 << USBS0) | (1 << UCSZ00) | (1 << UCSZ01) );	// 8 data bits, 2 stop bits
			UcsrB::write( (1 << RXCIE0) | (1 << UDRIE0) | (1 << RXEN0) | (1 << TXEN0) ); // start interface and enable interrupts
		}

		inline static void stop()
		{
			UcsrB::write( 0 );
			UcsrC::write( 0 );
			UcsrA::write( 0 );

			_rx.clear();
			_tx.clear();
		}

		static bool send( uint8_t c )
		{
			if( _tx.isFull() )
				return false;

			if( _tx.isEmpty() )
			{
				while( !UcsrA::isSet<UDRE0>() );
				Udr::write( c );
				UcsrB::set( 1 << UDRIE0 );
			}
			else
				_tx.push_back( c );

			return true;
		}

		static uint8_t bytesReceived() { return _rx.count(); }
		static uint8_t recv() { return _rx.pop_front(); }

		static inline void udreHandler()
		{
			if( _tx.isEmpty() ) UcsrB::clear( 1 << UDRIE0 );
			else Udr::write( _tx.pop_front() );
		}

		static inline void rxHandler()
		{
			uint8_t c = Udr::read();
			if( !_rx.isFull() )
				_rx.push_back( c );
		}
	};

	template<int TxSize, int RxSize> RingBuffer<RxSize> Usart<TxSize, RxSize>::_rx;
	template<int TxSize, int RxSize> RingBuffer<TxSize> Usart<TxSize, RxSize>::_tx;
};
