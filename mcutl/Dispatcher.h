
#pragma once

#include <MetaList.h>
#include <Interrupts.h>

namespace internal
{
	template<typename Task,class List> struct FindTask;
	template<typename Task> struct FindTask<Task,meta::List<>> { typedef void result; };
	template<typename Task,typename Head,typename... Tail> struct FindTask<Task,meta::List<Head,Tail...>>
	{
		typedef typename meta::StaticIf<
			meta::IsEqual<Task,typename Head::type>::value,
			Head,
			typename FindTask<Task,meta::List<Tail...>>::result
		>::result result;
	};

	template<class Flags,class List> struct TaskIterator;
	template<class Flags> struct TaskIterator<Flags,meta::List<>> { inline static void process() { } };
	template<class Flags,typename Head,typename... Tail> struct TaskIterator<Flags,meta::List<Head,Tail...>>
	{
		inline static void process()
		{
			if( Flags::read() & (1 << Head::index) )
			{
				Flags::clear( 1 << Head::index );
				Head::type::process();
				return;
			}
			TaskIterator<Flags,meta::List<Tail...>>::process();
		}
	};
}


template<class Flags,unsigned TimerCount = 4,typename TickType = uint16_t>
class SoftTimer
{
	// Timers
	struct Entry { TickType ticks; uint8_t flag; };
	static Entry _timers[TimerCount];
public:
	static void init()
	{
		for( Entry& e : _timers )
			e.ticks = 0;
	}

	inline static bool start( uint8_t flag, TickType ticks )
	{
		ISRLocker __locker;

		for( Entry& e : _timers )
		{
			if( !e.ticks )
			{
				e.ticks = ticks;
				e.flag = flag;
				return true;
			}
		}
		return false;
	}

	static void tick()
	{
		for( Entry& e : _timers )
		{
			if( !e.ticks ) continue;

			--e.ticks;
			if( !e.ticks )
				Flags::set( 1 << e.flag );
		}
	}
};

template<class Flags,unsigned TimerCount,typename TickType>
typename SoftTimer<Flags,TimerCount,TickType>::Entry SoftTimer<Flags,TimerCount,TickType>::_timers[TimerCount];


template<class TaskList,class Flags,unsigned TimerCount = 4,typename TickType = uint16_t>
class StaticDispatcher
{
	// Timer
	typedef SoftTimer<Flags,TimerCount,TickType> Timer;

public:
	inline static void init() { Timer::init(); }
	inline static void timerHandler() { Timer::tick(); }

	template<typename Task>
	inline static void setTask()
	{
		typedef typename internal::FindTask<Task,TaskList>::result TaskItem;
		Flags::set( 1 << TaskItem::index );
	}

	template<typename Task>
	inline static void setTimer( TickType ticks )
	{
		typedef typename internal::FindTask<Task,TaskList>::result TaskItem;
		Timer::start( TaskItem::index, ticks );
	}

	static void process()
	{
		internal::TaskIterator<Flags,TaskList>::process();
	}
};
