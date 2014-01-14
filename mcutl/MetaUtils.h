
#pragma once

#include <stdint.h>

namespace meta
{
	/////////////////////////////////////////////////////////////////////////
	// Type equality
	/////////////////////////////////////////////////////////////////////////

	template<typename A,typename B> struct IsEqual { static const bool value = false; };
	template<typename A> struct IsEqual<A,A> { static const bool value = true; };

	/////////////////////////////////////////////////////////////////////////
	// Static if
	/////////////////////////////////////////////////////////////////////////

	template<bool Condition,typename True,typename False> struct StaticIf;
	template<typename True,typename False> struct StaticIf<true,True,False> { typedef True result; };
	template<typename True,typename False> struct StaticIf<false,True,False> { typedef False result; };

	/////////////////////////////////////////////////////////////////////////
	// Size type
	/////////////////////////////////////////////////////////////////////////

	template<unsigned int Size> struct SizeType
	{
		typedef typename StaticIf<
			(Size <= 0xff),
			uint8_t,
			typename StaticIf<
				(Size <= 0xffff),
				uint16_t,
				uint32_t
			>::result
		>::result result;
	};

	/////////////////////////////////////////////////////////////////////////
	// Null type
	/////////////////////////////////////////////////////////////////////////

	struct Null { };

}