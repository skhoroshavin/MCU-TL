
#include "GPIO.h"
#include "Timer.h"
#include "Dispatcher.h"

struct LedOn;
struct LedOff;
typedef typename meta::MakeIndexedList<LedOn,LedOff>::result TaskList;

//typedef SoftRegister<uint8_t> TaskFlags;
//DEFINE_MCU_REGISTER(TaskFlags,r2,uint8_t)
DEFINE_IO_REGISTER(TaskFlags,GPIOR0,uint8_t)
typedef StaticDispatcher<TaskList,TaskFlags> Dispatcher;

ISR_TIMER0_OVERFLOW
{
	timer::Timer0::set( 256 - F_CPU / 1000 / 64 );
	Dispatcher::timerHandler();
}

struct LedOn
{
	inline static void process()
	{
		gpio::PinB5::set();
		Dispatcher::setTimer<LedOff>( 1000 );
	}
};

struct LedOff
{
	inline static void process()
	{
		gpio::PinB5::clear();
		Dispatcher::setTimer<LedOn>( 1000 );
	}
};

int main()
{
	Dispatcher::init();

	gpio::PinB5::setOutput();
	Dispatcher::setTask<LedOn>();

	timer::Timer0::start( timer::TC_Div64 );
	timer::Timer0::enableISR();

	sei();

	while(1)
		Dispatcher::process();

	return 0;
}
