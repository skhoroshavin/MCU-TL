
#include "GPIO.h"
#include "Timer.h"
#include "Usart.h"
#include "Flash.h"

#include <util/delay.h>

typedef stream::BufferedStream<stream::Usart0> uart;
typedef timer::Timer0 tickTimer;

ISR_USART0_RECV { uart::recvHandler(); }
ISR_USART0_SEND { uart::sendHandler(); }

const uint8_t max_wait = F_CPU / 256 / 256;
uint8_t wait = max_wait;

ISR_TIMER0_OVERFLOW
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
	tickTimer::start( timer::TC_Div256 );
	tickTimer::enableISR();

	gpio::PinB5::setOutput();

	uart::setBaudRate<57600>();
	uart::start();
	sei();

	uart::sendString( fromFlash(pHello) );

	while(1)
	{
		while( uart::recvAvailable() )
			uart::send( uart::recv() );
	}
	return 0;
}
