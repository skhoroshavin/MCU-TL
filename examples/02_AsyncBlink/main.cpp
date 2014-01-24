
#include <mcutl/ports.h>
#include <mcutl/timers.h>
#include <mcutl/dispatcher.h>

struct LedOn;
struct LedOff;
typedef meta::List<LedOn,LedOff> TaskList;

DEFINE_IO_REGISTER(TaskFlags,GPIOR0,uint8_t)
typedef StaticDispatcher<TaskList,TaskFlags> Dispatcher;

ISR_TIMER0_OVERFLOW
{
	Timer0::set( 256 - F_CPU / 1000 / 64 );
	Dispatcher::timerHandler();
}

struct LedOn
{
	inline static void process()
	{
		PinB5::set();
		Dispatcher::setTimer<LedOff>( 1000 );
	}
};

struct LedOff
{
	inline static void process()
	{
		PinB5::clear();
		Dispatcher::setTimer<LedOn>( 1000 );
	}
};

int main()
{
	Dispatcher::init();

	PinB5::setOutput();
	Dispatcher::setTask<LedOn>();

	Timer0::start( timer::TC_Div64 );
	Timer0::enableISR();

	enableISR();

	while(1)
		Dispatcher::process();

	return 0;
}
