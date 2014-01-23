
#pragma once

#include <mcutl/registers.h>

/////////////////////////////////////////////////////////////////////
/// Base port
/////////////////////////////////////////////////////////////////////

template<typename DataType,class Dir,class Out,class In>
struct BasePort
{
	typedef DataType data_type;

	inline static void setDir( DataType data )                      { Dir::write( data ); }
	inline static void setOutput( DataType data )                   { Dir::set( data ); }
	inline static void setInput( DataType data )                    { Dir::clear( data ); }
	inline static void maskedSetDir( DataType mask, DataType data ) { Dir::maskedSet( mask, data ); }

	inline static void write( DataType data )                    { Out::write( data ); }
	inline static void set( DataType data )                      { Out::set( data ); }
	inline static void clear( DataType data )                    { Out::clear( data ); }
	inline static void toggle( DataType data )                   { Out::toggle( data ); }
	inline static void maskedSet( DataType mask, DataType data ) { Out::maskedSet( mask, data ); }

	inline static DataType read() { return In::read();  }
};

/////////////////////////////////////////////////////////////////////
/// Null port
/////////////////////////////////////////////////////////////////////

template<typename DataType = unsigned char> struct NullPort :
		public BasePort<DataType,
						NullRegister<DataType>,
						NullRegister<DataType>,
						NullRegister<DataType>
		> { };

/////////////////////////////////////////////////////////////////////
/// Pin
/////////////////////////////////////////////////////////////////////

template<class Port,unsigned char Pos> class Pin
{
public:
	typedef Port port;
	typedef typename Port::data_type port_type;

	static const unsigned char pos = Pos;
	static const port_type     mask = (1 << Pos);

	inline static void setDir( bool out ) { if( out ) setOutput(); else setInput(); }
	inline static void setOutput() { Port::setOutput( mask ); }
	inline static void setInput() { Port::setInput( mask ); }

	inline static void write( bool value ) { if( value ) set(); else clear(); }
	inline static void set() { Port::set( mask ); }
	inline static void clear() { Port::clear( mask ); }
	inline static void toggle() { Port::toggle( mask ); }

	inline static bool read() { return Port::read() & mask; }
};

/////////////////////////////////////////////////////////////////////
/// Platform-dependent GPIO
/////////////////////////////////////////////////////////////////////

#ifdef __AVR__
#include <mcutl/avr/_ports.h>
#endif // __AVR__
