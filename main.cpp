
#include "GPIO.h"
#include "Timer.h"
#include "Dispatcher.h"

const uint8_t period = F_CPU / 256 / 256;

struct LedOn;
struct LedOff;

typedef Dispatcher<
	1,
	LedOn,
	LedOff
> dispatcher;

struct LedOn
{
	inline static void process()
	{
		gpio::PinB5::set();
		dispatcher::setDelayedTask<LedOff>( period );
	}
};

struct LedOff
{
	inline static void process()
	{
		gpio::PinB5::clear();
		dispatcher::setDelayedTask<LedOn>( period );
	}
};

typedef timer::Timer0 tickTimer;

ISR_TIMER0_OVERFLOW { dispatcher::timerHandler(); }

int main()
{
	dispatcher::init();

	gpio::PinB5::setOutput();
	dispatcher::setTask<LedOn>();

	tickTimer::start( timer::TC_Div256 );
	tickTimer::enableISR();

	sei();

	while(1)
		dispatcher::process();

	return 0;
}
