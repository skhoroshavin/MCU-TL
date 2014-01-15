
#include "RingBuffer.h"

#include "GPIO.h"
#include "Usart.h"
#include "Flash.h"

#include <util/delay.h>

typedef stream::BufferedStream<stream::Usart0> uart;

IRQ_USART0_RECV
{
	unsigned char c = UDR0;
	uart::send( c );
	//uart::recvHandler();
}

IRQ_USART0_SEND
{
	uart::sendHandler();
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
