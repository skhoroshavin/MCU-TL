
#include "GPIO.h"
#include "Timer.h"
#include "Dispatcher.h"

struct LedOn;
struct LedOff;

typedef Dispatcher<4,
	LedOn,
	LedOff
> dispatcher;

ISR_TIMER0_OVERFLOW
{
	timer::Timer0::set( 256 - F_CPU / 1000 / 64 );
	dispatcher::timerHandler();
}

struct LedOn
{
	inline static void process()
	{
		gpio::PinB5::set();
		dispatcher::setTimer<LedOff>( 1000 );
	}
};

struct LedOff
{
	inline static void process()
	{
		gpio::PinB5::clear();
		dispatcher::setTimer<LedOn>( 1000 );
	}
};

int main()
{
	dispatcher::init();

	gpio::PinB5::setOutput();
	dispatcher::setTask<LedOn>();

	timer::Timer0::start( timer::TC_Div64 );
	timer::Timer0::enableISR();

	sei();

	while(1)
		dispatcher::process();

	return 0;
}
