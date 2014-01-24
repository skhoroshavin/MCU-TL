
#include <mcutl/ports.h>
#include <mcutl/pin_list.h>
#include <util/delay.h>

typedef PinList<arduino::D8,arduino::D11> Motor1Dir;
typedef arduino::D9 Motor1En;
typedef PinList<arduino::D12,arduino::D13> Motor2Dir;
typedef arduino::D10 Motor2En;

template<typename Pin, unsigned Time> inline void pinPulse()
{
	Pin::set();
	_delay_ms( Time );
	Pin::clear();
}

template<unsigned StepTime> void rotateForward()
{
	Motor1Dir::write( 0b10 );
	pinPulse<Motor1En,StepTime>();

	Motor2Dir::write( 0b10 );
	pinPulse<Motor2En,StepTime>();

	Motor1Dir::write( 0b01 );
	pinPulse<Motor1En,StepTime>();

	Motor2Dir::write( 0b01 );
	pinPulse<Motor2En,StepTime>();
}

template<unsigned StepTime> void rotateBackward()
{
	Motor1Dir::write( 0b10 );
	pinPulse<Motor1En,StepTime>();

	Motor2Dir::write( 0b01 );
	pinPulse<Motor2En,StepTime>();

	Motor1Dir::write( 0b01 );
	pinPulse<Motor1En,StepTime>();

	Motor2Dir::write( 0b10 );
	pinPulse<Motor2En,StepTime>();
}

int main()
{
	Motor1En::setOutput();
	Motor1En::clear();

	Motor2En::setOutput();
	Motor2En::clear();

	Motor1Dir::setDir( 0xff );
	Motor2Dir::setDir( 0xff );

	while(1)
	{
		for( int i = 0; i < 150; ++i )
			rotateForward<2>();
		_delay_ms( 200 );

		for( int i = 0; i < 150; ++i )
			rotateBackward<2>();
		_delay_ms( 200 );
	}

	return 0;
}
