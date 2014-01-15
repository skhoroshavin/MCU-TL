
#pragma once

#define DEFINE_REGISTER(ClassName,RegName,DataType) \
	struct ClassName \
	{ \
		inline static void write( DataType data ) { RegName = data; } \
		inline static void set( DataType data ) { RegName |= data; } \
		inline static void clear( DataType data ) { RegName &= ~data; } \
		inline static void toggle( DataType data ) { RegName ^= data; } \
		inline static void maskedSet( DataType mask, DataType data ) { RegName = (RegName & ~mask) | data; } \
		inline static DataType read() { return RegName; } \
	};

namespace gpio
{
	/////////////////////////////////////////////////////////////////////
	/// Null register
	/////////////////////////////////////////////////////////////////////

	template<typename DataType = unsigned char> struct NullRegister
	{
		inline static void write( DataType data ) { }
		inline static void set( DataType data ) { }
		inline static void clear( DataType data ) { }
		inline static void maskedSet( DataType mask, DataType data ) { }

		inline static DataType read() { return 0; }
		template<DataType Bit>
		inline static bool isSet() { return false; }
	};

	/////////////////////////////////////////////////////////////////////
	/// Null port
	/////////////////////////////////////////////////////////////////////

	template<typename DataType = unsigned char> struct NullPort
	{
		typedef DataType data_type;

		inline static void setDir( DataType data ) { }
		inline static void setOutput( DataType data ) { }
		inline static void setInput( DataType data ) { }
		inline static void maskedSetDir( DataType mask, DataType data ) { }

		inline static void write( DataType data ) { }
		inline static void set( DataType data ) { }
		inline static void clear( DataType data ) { }
		inline static void toggle( DataType data ) { }
		inline static void maskedSet( DataType mask, DataType data ) { }

		inline static DataType read() { return 0; }
	};

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
}
