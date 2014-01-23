
#include <mcutl/flash.h>
#include <mcutl/interrupts.h>
#include <mcutl/ports.h>
#include <mcutl/usart.h>
#include <util/delay.h>

typedef BufferedStream<Usart0> Uart;
const char FLASH_STORAGE pHello[] = "Hello AVR!\n";

ISR_USART0_SEND
{
	Uart::sendHandler();
}

ISR_USART0_RECV
{
	Uart::recvHandler();
	while( Uart::recvAvailable() )
		Uart::send( Uart::recv() );
}

int main()
{
	PinB5::setOutput();

	Uart::init();
	Uart::setBaudRate<57600>();
	Uart::start();

	enableISR();

	Uart::sendString( fromFlash(pHello) );

	while(1)
	{
		PinB5::toggle();
		_delay_ms( 1000 );
	}

	return 0;
}
