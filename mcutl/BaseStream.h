
#pragma once

#include <RingBuffer.h>

namespace stream
{
	class NullStream
	{
	public:
		inline static bool sendReady() { return true; }
		inline static void send( char c ) { }
		inline static void setSendISR( bool enabled ) { }

		inline static int  recvAvailable() { return 0; }
		inline static char recv() { return 0; }
	};

	template<class Stream,int TxSize = 16,int RxSize = 16> class BufferedStream : public Stream
	{
		static RingBuffer<RxSize> _rx;
		static RingBuffer<TxSize> _tx;
	public:
		static bool sendReady()
		{
			if( !_tx.isFull() ) return true;
			return Stream::sendReady();
		}
		static void send( char c )
		{
			if( _tx.isEmpty() && Stream::sendReady() )
			{
				Stream::send( c );
				Stream::enableSendISR( true );
			}
			else
				_tx.push_back( c );
		}
		static inline void sendHandler()
		{
			if( _tx.isEmpty() ) Stream::enableSendISR( false );
			else Stream::send( _tx.pop_front() );
		}

		inline static int recvAvailable() { return _rx.count(); }
		inline static char recv() { return _rx.pop_front(); }
		inline static void recvHandler()
		{
			char c = Stream::recv();
			if( !_rx.isFull() )
				_rx.push_back( c );
		}

		template<typename Str>
		static bool sendString( Str s )
		{
			while( char c = *s )
			{
				while( !sendReady() );
				send( c );
				++s;
			}
			return true;
		}
	};

	template<class Stream,int TxSize,int RxSize> RingBuffer<RxSize> BufferedStream<Stream,TxSize,RxSize>::_rx;
	template<class Stream,int TxSize,int RxSize> RingBuffer<TxSize> BufferedStream<Stream,TxSize,RxSize>::_tx;

}
