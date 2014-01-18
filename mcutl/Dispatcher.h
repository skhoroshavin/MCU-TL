
#pragma once

#include <MetaList.h>

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

	template<typename Flags,class List> struct TaskIterator;
	template<typename Flags> struct TaskIterator<Flags,meta::List<>> { inline static void process( Flags& flags ) { } };
	template<typename Flags,typename Head,typename... Tail> struct TaskIterator<Flags,meta::List<Head,Tail...>>
	{
		inline static void process( Flags& flags )
		{
			const Flags mask = 1 << Head::index;
			if( flags & mask )
			{
				flags &= ~mask;
				Head::type::process();
				return;
			}
			TaskIterator<Flags,meta::List<Tail...>>::process( flags );
		}
	};
}

template<unsigned TimerCount,class... Tasks>
class Dispatcher
{
	// Tasks
	typedef typename meta::MakeIndexedList<Tasks...>::result TaskList;
	static const unsigned taskCount = TaskList::size;
	typedef typename meta::FlagsType<taskCount>::result TaskFlags;
	static TaskFlags _flags;

	// Timers
	struct SoftTimer { uint16_t ticks; uint8_t task; };
	static SoftTimer _timers[TimerCount];

public:
	static void init()
	{
		for( SoftTimer& timer : _timers )
			timer.ticks = 0;
	}

	template<typename Task>
	inline static void setTask()
	{
		typedef typename internal::FindTask<Task,TaskList>::result TaskItem;
		_flags |= (1 << TaskItem::index);
	}

	template<typename Task>
	inline static void setTimer( uint16_t delay )
	{
		typedef typename internal::FindTask<Task,TaskList>::result TaskItem;
		for( SoftTimer& timer : _timers )
		{
			if( !timer.ticks )
			{
				timer.ticks = delay;
				timer.task = TaskItem::index;
				return;
			}
		}
		// TODO: Fail!!!
	}

	static void timerHandler()
	{
		for( SoftTimer& timer : _timers )
		{
			if( !timer.ticks ) continue;

			--timer.ticks;
			if( !timer.ticks )
				_flags |= (1 << timer.task);
		}
	}

	static void process()
	{
		internal::TaskIterator<TaskFlags,TaskList>::process( _flags );
	}
};

template<unsigned TimerCount,class... Tasks> typename Dispatcher<TimerCount,Tasks...>::TaskFlags Dispatcher<TimerCount,Tasks...>::_flags = 0;
template<unsigned TimerCount,class... Tasks> typename Dispatcher<TimerCount,Tasks...>::SoftTimer Dispatcher<TimerCount,Tasks...>::_timers[TimerCount];
