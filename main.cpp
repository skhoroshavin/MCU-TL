
#include "GPIO.h"
#include "Usart.h"
#include "Flash.h"

#include <util/delay.h>

typedef stream::BufferedStream<stream::Usart0> uart;

ISR_USART0_RECV
{
	unsigned char c = UDR0;
	uart::send( c );
	//uart::recvHandler();
}

ISR_USART0_SEND
{
	uart::sendHandler();
}

const uint8_t max_wait = 61;
uint8_t wait = max_wait;

ISR(TIMER0_OVF_vect)
{
	wait--;
	if( !wait )
	{
		gpio::PinB5::toggle();
		wait = max_wait;
	}
}

const char FLASH_STORAGE pHello[] = "Hello AVR!\r\n";

int main()
{
	TCCR0A = 0;
	TCCR0B = 0b101;
	TIMSK0 = 1;

	gpio::PinB5::setOutput();

	uart::setBaudRate<57600>();
	uart::start();
	sei();

	uart::sendString( fromFlash(pHello) );

	while(1);
	return 0;
}
