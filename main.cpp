
#include "IOPorts.h"
#include "Arduino.h"
#include "PinList.h"

#include <util/delay.h>

/*
#include "mcucpp/ioports.h"
#include "mcucpp/iopins.h"
#include "mcucpp/pinlist.h"
*/

int main()
{
	gpio::PortB::setDirection( 0xff );

	while(1)
	{
		arduino::D13::toggle();
		_delay_ms( 1000 );
	}
	return 0;
}
