
#pragma once

#include <avr/io.h>
#include <avr/interrupt.h>

namespace timer
{
	enum TimerMode
	{
		TM_Normal  = 0,
		TM_PWM     = 1,
		TM_CTC     = 2,
		TM_FastPWM = 3,

		TM_MASK    = 3
	};

	enum CompareMode
	{
		CM_Off    = 0,
		CM_Toggle = 1,
		CM_Clear  = 2,
		CM_Set    = 3,

		CM_MASK   = 3
	};

	enum TimerClock
	{
		TC_Off        = 0,
		TC_Div1       = 1,
		TC_Div8       = 2,
		TC_Div64      = 3,
		TC_Div256     = 4,
		TC_Div1024    = 5,
		TC_ExtFalling = 6,
		TC_ExtRising  = 7,

		TC_MASK       = 7
	};
}

class Timer0
{
	DEFINE_IO_REGISTER(TccrA,TCCR0A,uint8_t)
	DEFINE_IO_REGISTER(TccrB,TCCR0B,uint8_t)
	DEFINE_IO_REGISTER(Tifr, TIFR0, uint8_t)
	DEFINE_IO_REGISTER(Timsk,TIMSK0,uint8_t)
public:
	inline static void setMode( timer::TimerMode mode ) { TccrA::maskedSet( timer::TM_MASK, mode ); }

	inline static void start( timer::TimerClock clock ) { TccrB::maskedSet( timer::TC_MASK, clock ); }
	inline static void stop() { TccrB::clear( timer::TC_MASK ); }

	inline static uint8_t get() { return TCNT0; }
	inline static void set( uint8_t value ) { TCNT0 = value; }
	inline static bool checkFlag() { return Tifr::isBitSet(TOV0); }
	inline static void clearFlag() { Tifr::setBit(TOV0); }
	inline static void enableISR() { Timsk::setBit(TOIE0); }
	inline static void disableISR() { Timsk::clearBit(TOIE0); }

	struct CompareA
	{
		inline static uint8_t get() { return OCR0A; }
		inline static void set( uint8_t value ) { OCR0A = value; }
		inline static void setMode( timer::CompareMode mode ) { TccrA::maskedSet( timer::CM_MASK << 6, mode << 6 ); }
		inline static bool checkFlag() { return Tifr::isBitSet(OCF0A); }
		inline static void clearFlag() { Tifr::setBit(OCF0A); }
		inline static void enableISR() { Timsk::setBit(OCIE0A); }
		inline static void disableISR() { Timsk::clearBit(OCIE0A); }
	};

	struct CompareB
	{
		inline static uint8_t get() { return OCR0B; }
		inline static void set( uint8_t value ) { OCR0B = value; }
		inline static void setMode( timer::CompareMode mode ) { TccrB::maskedSet( timer::CM_MASK << 4, mode << 4 ); }
		inline static bool checkFlag() { return Tifr::isBitSet(OCF0B); }
		inline static void clearFlag() { Tifr::setBit(OCF0B); }
		inline static void enableISR() { Timsk::setBit(OCIE0B); }
		inline static void disableISR() { Timsk::clearBit(OCIE0B); }
	};
};

#define ISR_TIMER0_OVERFLOW  ISR(TIMER0_OVF_vect)
#define ISR_TIMER0_COMPARE_A ISR(TIMER0_COMPA_vect)
#define ISR_TIMER0_COMPARE_B ISR(TIMER0_COMPB_vect)

