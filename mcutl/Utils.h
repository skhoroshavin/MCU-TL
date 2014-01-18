
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

