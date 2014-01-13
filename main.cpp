
#include "RingBuffer.h"

#include "IOPorts.h"
#include "Arduino.h"
#include "PinList.h"

#include <avr/interrupt.h>
#include <util/delay.h>

using namespace gpio;
using namespace arduino;

typedef PinList<D10,D11,D12,D13> Test;

template<int TxSize,int RxSize> class Usart
{
	static RingBuffer<RxSize> _rx;
	static RingBuffer<TxSize> _tx;
public:
	template<uint32_t rate>
	inline static void setBaudRate()
	{
		const uint16_t ubrr1x = (F_CPU/16/rate-1);
		const uint16_t ubrr2x = (F_CPU/8/rate-1);
		const uint32_t rate1x = (F_CPU/16/(ubrr1x+1));
		const uint32_t rate2x = (F_CPU/8/(ubrr2x+1));
		const uint32_t err1 = rate > rate1x ? (rate - rate1x) : (rate1x - rate);
		const uint32_t err2 = rate > rate2x ? (rate - rate2x) : (rate2x - rate);

		if( err1 < err2 )
		{
			UCSR0A = 0;
			UBRR0H = (uint8_t)(rate1x >> 8);
			UBRR0L = (uint8_t)(rate1x & 0xff);
		}
		else
		{
			UCSR0A = (1 << U2X0);
			UBRR0H = (uint8_t)(rate2x >> 8);
			UBRR0L = (uint8_t)(rate2x & 0xff);
		}
	}

	inline static void start()
	{
		UCSR0C = (1 << USBS0) | (1 << UCSZ00) | (1 << UCSZ01);
		UCSR0B = (1 << RXCIE0) | (1 << UDRIE0) | (1 << RXEN0) | (1 << TXEN0);
	}

	inline static void stop()
	{
		UCSR0B = 0;
		UCSR0C = 0;
		_rx.clear();
		_tx.clear();
	}

	static bool send( uint8_t c )
	{
		if( _tx.isFull() ) return false;
		if( _tx.isEmpty() )
		{
			while( !(UCSR0A & (1 << UDRE0)) );
			UDR0 = c;
			UCSR0B |= (1 << UDRIE0);
		}
		else
			_tx.push_back( c );
		return true;
	}

	static uint8_t bytesReceived() { return _rx.count(); }
	static uint8_t recv() { return _rx.pop_front(); }

	static inline void udreHandler()
	{
		if( _tx.isEmpty() )
			UCSR0B &= ~(1 << UDRIE0);
		else
			UDR0 = _tx.pop_front();
	}

	static inline void rxHandler()
	{
		if( !_rx.isFull() )
			_rx.push_back( UDR0 );
	}
};

template<int TxSize, int RxSize> RingBuffer<RxSize> Usart<TxSize, RxSize>::_rx;
template<int TxSize, int RxSize> RingBuffer<TxSize> Usart<TxSize, RxSize>::_tx;

typedef Usart<16,16> usart;

ISR(USART_UDRE_vect) { usart::udreHandler(); }
ISR(USART_RX_vect) { usart::rxHandler(); }

int main()
{
	Test::setOutput( 0xff );

	usart::setBaudRate<57600>();
	usart::start();
	sei();

	while(1)
	{
		Test::toggle( 0xff );
		_delay_ms( 1000 );

	}
	return 0;
}
