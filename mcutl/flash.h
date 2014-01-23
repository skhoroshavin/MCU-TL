
#pragma once

#ifdef __AVR__

#include <avr/pgmspace.h>

template<typename T> class FlashPtr
{
public:
	// Init
	inline void init( const T * ptr ) { _ptr = ptr; }

	// Pointer manipulation
	inline FlashPtr<T>& operator++() { ++_ptr; return *this; }
	inline FlashPtr<T>& operator--() { --_ptr; return *this; }

	// Access data
	inline T operator*() const { return read(_ptr); }
	inline T operator[]( int i ) const { return read(_ptr + i); }

private:
	const T * _ptr;

	inline static T read( const T * ptr )
	{
		union { T value; uint8_t data[sizeof(T)]; } result;
		for( unsigned char i = 0; i < sizeof(T); ++i )
			result.data[i] = pgm_read_byte( reinterpret_cast<const uint8_t*>(ptr) + i );
		return result.value;

	}
};

#define FLASH_STORAGE PROGMEM
template<typename T> inline FlashPtr<T> fromFlash( const T * p ) { FlashPtr<T> r; r.init( p ); return r; }

#else // __AVR__

#define FLASH_STORAGE
#define fromFlash(x) x

#endif // __AVR__
