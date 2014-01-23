
#include "GPIO.h"
#include <util/delay.h>

int main()
{
	gpio::PinB5::setOutput();

	while(1)
	{
		gpio::PinB5::toggle();
		_delay_ms( 1000 );
	}

	return 0;
}
