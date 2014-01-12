
#pragma once

#include "Platform.h"

namespace gpio
{

template<class Port,uint8_t Pos> class Pin
{
public:
	typedef Port port;
	static constexpr uint8_t pos  = Pos;
	static constexpr uint8_t mask = (1 << Pos);

	inline static void setDir( bool out ) { if( out ) setOutput(); else setInput(); }
	inline static void setOutput() { Port::setOutput( mask ); }
	inline static void setInput() { Port::setInput( mask ); }

	inline static void write( bool value ) { if( value ) set(); else clear(); }
	inline static void set() { Port::set( mask ); }
	inline static void clear() { Port::clear( mask ); }
	inline static void toggle() { Port::toggle( mask ); }

	inline static bool read() { return Port::read() & mask; }
};

}
