
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


template<unsigned TimerCount,class _Flags>
class SoftTimer
{
	// Timers
	struct Entry { uint16_t ticks; uint8_t flag; };
	static Entry _timers[TimerCount];
public:
	typedef _Flags Flags;

	static void init()
	{
		for( Entry& e : _timers )
			e.ticks = 0;
	}

	inline static bool start( uint8_t flag, uint16_t ticks )
	{
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

	static void timerHandler()
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

template<unsigned TimerCount,class Flags>
typename SoftTimer<TimerCount,Flags>::Entry SoftTimer<TimerCount,Flags>::_timers[TimerCount];


template<class Flags,unsigned TimerCount,class... Tasks>
class StaticDispatcher
{
	// Tasks
	typedef typename meta::MakeIndexedList<Tasks...>::result TaskList;

	// Timer
	typedef SoftTimer<TimerCount,Flags> Timer;

public:
	inline static void init() { Timer::init(); }
	inline static void timerHandler() { Timer::timerHandler(); }

	template<typename Task>
	inline static void setTask()
	{
		typedef typename internal::FindTask<Task,TaskList>::result TaskItem;
		Flags::set( 1 << TaskItem::index );
	}

	template<typename Task>
	inline static void setTimer( uint16_t ticks )
	{
		typedef typename internal::FindTask<Task,TaskList>::result TaskItem;
		Timer::start( TaskItem::index, ticks );
	}

	static void process()
	{
		internal::TaskIterator<Flags,TaskList>::process();
	}
};
