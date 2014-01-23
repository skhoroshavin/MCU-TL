
#pragma once

#include <meta/common.h>

template<unsigned Size,typename DataType=unsigned char>
class RingBuffer
{
	static_assert( !(Size & (Size-1)), "Ring buffer size must be power of two!" );

	typedef typename meta::SizeType<Size>::result IndexType;
	static const IndexType mask = (Size - 1);

	DataType _data[Size];
	volatile IndexType _readCount;
	volatile IndexType _writeCount;

public:
	inline void init() { _readCount = _writeCount = 0; }

	inline bool isEmpty() const { return _readCount == _writeCount; }
	inline bool isFull() const { return !(((_writeCount - _readCount) & mask) ^ mask); }
	inline IndexType count() const { return (_writeCount - _readCount) & mask; }
	inline DataType front() const { return _data[_readCount & mask]; }

	inline void clear() { _readCount = _writeCount = 0; }
	inline void push_back( DataType value ) { _data[_writeCount++ & mask] = value; }
	inline DataType pop_front() { return _data[_readCount++ & mask]; }
};
