
#pragma once

/////////////////////////////////////////////////////////////////////
/// Null register
/////////////////////////////////////////////////////////////////////

template<typename DataType = unsigned char> struct NullRegister
{
	inline static void write( DataType data ) { } \
	inline static void set( DataType data ) { } \
	inline static void clear( DataType data ) { } \
	inline static void toggle( DataType data ) { } \
	inline static void maskedSet( DataType mask, DataType data ) { } \
	inline static DataType read() { return 0; } \
	inline static void setBit( unsigned char bit ) { } \
	inline static void clearBit( unsigned char bit ) { } \
	inline static void writeBit( unsigned char bit, bool value ) { } \
	inline static bool isBitSet( unsigned char bit ) { return false; } \
};

/////////////////////////////////////////////////////////////////////////
/// Software register
/////////////////////////////////////////////////////////////////////////

template<typename Tag,typename DataType>
class SoftRegister
{
	static volatile DataType _data;
public:
	inline static void write( DataType data ) { _data = data; } \
	inline static void set( DataType data ) { _data |= data; } \
	inline static void clear( DataType data ) { _data &= ~data; } \
	inline static void toggle( DataType data ) { _data ^= data; } \
	inline static void maskedSet( DataType mask, DataType data ) { _data = (_data & ~mask) | data; } \
	inline static DataType read() { return _data; } \
	inline static void setBit( unsigned char bit ) { _data |= (DataType)(1 << bit); } \
	inline static void clearBit( unsigned char bit ) { _data &= ~(DataType)(1 << bit); } \
	inline static void writeBit( unsigned char bit, bool value ) { if( value ) setBit(bit); else clearBit(bit); } \
	inline static bool isBitSet( unsigned char bit ) { return _data & (DataType)(1 << bit); } \
};
template<typename Tag,typename DataType> volatile DataType SoftRegister<Tag,DataType>::_data = 0;

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
		inline static void setBit( unsigned char bit ) { __mcu_##RegName |= (DataType)(1 << bit); } \
		inline static void clearBit( unsigned char bit ) { __mcu_##RegName &= ~(DataType)(1 << bit); } \
		inline static void writeBit( unsigned char bit, bool value ) { if( value ) setBit(bit); else clearBit(bit); } \
		inline static bool isBitSet( unsigned char bit ) { return __mcu_##RegName & (DataType)(1 << bit); } \
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
		inline static void setBit( unsigned char bit ) { RegName |= (DataType)(1 << bit); } \
		inline static void clearBit( unsigned char bit ) { RegName &= ~(DataType)(1 << bit); } \
		inline static void writeBit( unsigned char bit, bool value ) { if( value ) setBit(bit); else clearBit(bit); } \
		inline static bool isBitSet( unsigned char bit ) { return RegName & (DataType)(1 << bit); } \
	};
