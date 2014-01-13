
#include "RingBuffer.h"

#include "GPIO.h"
#include "Usart.h"
#include "PinList.h"

#include <avr/interrupt.h>
#include <util/delay.h>

typedef serial::Usart<16,16> uart;
ISR(USART_UDRE_vect) { uart::udreHandler(); }
ISR(USART_RX_vect) { uart::rxHandler(); }

int main()
{
	gpio::D13::setOutput();

	uart::setBaudRate<57600>();
	uart::start();
	sei();

	uart::send( 'H' );
	uart::send( 'e' );
	uart::send( 'l' );
	uart::send( 'l' );
	uart::send( 'o' );
	uart::send( ' ' );
	uart::send( 'A' );
	uart::send( 'V' );
	uart::send( 'R' );
	uart::send( '!' );
	uart::send( '\n' );

	while(1)
	{
		while( uart::bytesReceived() )
			uart::send( uart::recv() );

		gpio::D13::toggle();
		_delay_ms( 1000 );

	}
	return 0;
}
