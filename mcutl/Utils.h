
#pragma once

/////////////////////////////////////////////////////////////////////////
/// Software register
/////////////////////////////////////////////////////////////////////////

template<typename DataType>
class SoftRegister
{
	static DataType _data;
public:
	inline static void write( DataType data ) { _data = data; } \
	inline static void set( DataType data ) { _data |= data; } \
	inline static void clear( DataType data ) { _data &= ~data; } \
	inline static void toggle( DataType data ) { _data ^= data; } \
	inline static void maskedSet( DataType mask, DataType data ) { _data = (_data & ~mask) | data; } \
	inline static DataType read() { return _data; } \
};
template<typename DataType> DataType SoftRegister<DataType>::_data = 0;

/////////////////////////////////////////////////////////////////////////
/// MCU register
/////////////////////////////////////////////////////////////////////////

#define DEFINE_MCU_REGISTER(ClassName,RegName,DataType) \
	register DataType __mcu_##RegName asm(#RegName); \
	struct ClassName \
	{ \
		inline static void write( DataType data ) { __mcu_##RegName = data; } \
		inline static void set( DataType data ) { __mcu_##RegName |= data; } \
		inline static void clear( DataType data ) { __mcu_##RegName &= ~data; } \
		inline static void toggle( DataType data ) { __mcu_##RegName ^= data; } \
		inline static void maskedSet( DataType mask, DataType data ) { __mcu_##RegName = (__mcu_##RegName & ~mask) | data; } \
		inline static DataType read() { return __mcu_##RegName; } \
	};

/////////////////////////////////////////////////////////////////////////
/// I/O register
/////////////////////////////////////////////////////////////////////////

#define DEFINE_IO_REGISTER(ClassName,RegName,DataType) \
	struct ClassName \
	{ \
		inline static void write( DataType data ) { RegName = data; } \
		inline static void set( DataType data ) { RegName |= data; } \
		inline static void clear( DataType data ) { RegName &= ~data; } \
		inline static void toggle( DataType data ) { RegName ^= data; } \
		inline static void maskedSet( DataType mask, DataType data ) { RegName = (RegName & ~mask) | data; } \
		inline static DataType read() { return RegName; } \
	};
