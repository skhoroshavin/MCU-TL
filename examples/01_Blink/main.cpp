
#include <mcutl/ports.h>
#include <util/delay.h>

int main()
{
	PinB5::setOutput();

	while(1)
	{
		PinB5::toggle();
		_delay_ms( 1000 );
	}

	return 0;
}
