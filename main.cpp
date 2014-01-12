
#include "IOPorts.h"
#include "Arduino.h"
#include "PinList.h"

#include <util/delay.h>

/*
#include "mcucpp/ioports.h"
#include "mcucpp/iopins.h"
#include "mcucpp/pinlist.h"
*/

using namespace gpio;
using namespace arduino;

typedef PinList<D10,D11,D12,D13> SPI;

int main()
{
	SPI::setOutput( 0xff );

	while(1)
	{
		SPI::toggle( 0xff );
		_delay_ms( 1000 );
	}
	return 0;
}
