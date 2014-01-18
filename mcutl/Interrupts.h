
#pragma once

#include <avr/interrupt.h>

struct NullLocker { };

class ISRLocker
{
	uint8_t _sreg;
public:
	inline ISRLocker() : _sreg(SREG) { cli(); }
	inline ~ISRLocker() { SREG = _sreg; }
};
