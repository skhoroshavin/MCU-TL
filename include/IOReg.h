
#pragma once

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

#define DEFINE_REGISTER(ClassName,RegName,DataType) \
	struct ClassName \
	{ \
		inline static void write( DataType data ) { RegName = data; } \
		inline static void set( DataType data ) { RegName |= data; } \
		inline static void clear( DataType data ) { RegName &= ~data; } \
		inline static void maskedSet( DataType mask, DataType data ) { RegName = (RegName & ~mask) | data; } \
		inline static DataType read() { return RegName; } \
		template<DataType Bit> \
		inline static bool isSet() { return RegName & (1 << Bit); } \
	};

