
#include "RingBuffer.h"

#include "GPIO.h"
#include "Usart.h"
#include "Flash.h"

#include <util/delay.h>

typedef serial::Usart<16,16> uart;
ISR(USART_UDRE_vect) { uart::udreHandler(); }
ISR(USART_RX_vect)
{
	unsigned char c = UDR0;
	uart::send( c );
	//uart::rxHandler();
}

const char FLASH_STORAGE pHello[] = "Hello AVR!\r\n";

int main()
{
	gpio::D13::setOutput();

	uart::setBaudRate<57600>();
	uart::start();
	sei();

	uart::sendString( fromFlash(pHello) );

	while(1)
	{
		gpio::D13::toggle();
		_delay_ms( 1000 );

	}
	return 0;
}
