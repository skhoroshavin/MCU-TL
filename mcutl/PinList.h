
#pragma once

#include <MetaList.h>

namespace gpio
{
	namespace internal
	{
		/////////////////////////////////////////////////////////////////////////
		// Utility
		/////////////////////////////////////////////////////////////////////////

		template<class Pins> struct GetPortMask;
		template<> struct GetPortMask<meta::List<>> { static const uint8_t value = 0; };
		template<class Pin,class... Tail> struct GetPortMask<meta::List<Pin,Tail...>>
		{
			static const uint8_t value = Pin::type::mask | GetPortMask<meta::List<Tail...>>::value;
		};

		template<class Pin> struct IsPinTransparent { static const bool value = (Pin::index == Pin::type::pos); };
		template<class Pin> struct IsPinShifted { static const bool value = (Pin::index != Pin::type::pos); };

		template<class Pins> struct GetSerialCount;
		template<> struct GetSerialCount<meta::List<>>
		{
			static const uint8_t port_pos = -1;
			static const uint8_t data_pos = -1;
			static const uint8_t value = 0;
		};
		template<class Pin,class... Tail> struct GetSerialCount<meta::List<Pin,Tail...>>
		{
			typedef GetSerialCount<meta::List<Tail...>> next;
			static const uint8_t port_pos = Pin::type::pos;
			static const uint8_t data_pos = Pin::index;
			static const uint8_t value = ((port_pos == next::port_pos-1) && (data_pos == next::data_pos-1)) ? next::value+1 : 1;
		};

		template<uint8_t Offset> struct ShiftLeft  { inline static uint8_t shift( uint8_t value ) { return value << Offset; } };
		template<uint8_t Offset> struct ShiftRight { inline static uint8_t shift( uint8_t value ) { return value >> Offset; } };
		template<uint8_t First, uint8_t Second, bool Out = true>
		class Shifter
		{
			static const bool FirstGreater = First > Second;
			typedef typename meta::StaticIf<FirstGreater,ShiftLeft<First-Second>,ShiftRight<Second-First>>::result OutShifter;
			typedef typename meta::StaticIf<FirstGreater,ShiftRight<First-Second>,ShiftLeft<Second-First>>::result InShifter;
			typedef typename meta::StaticIf<Out,OutShifter,InShifter>::result ActualShifter;
		public:
			inline static uint8_t shift( uint8_t value ) { return ActualShifter::shift(value); }
		};

		/////////////////////////////////////////////////////////////////////////
		// Pin iterator
		/////////////////////////////////////////////////////////////////////////

		template<class Pins> struct PinIterator;
		template<> struct PinIterator<meta::List<>>
		{
			inline static uint8_t convertOut( uint8_t data, uint8_t result ) { return result; }
			inline static uint8_t convertIn( uint8_t data, uint8_t result ) { return result; }
		};
		template<class Pin,class... Tail> struct PinIterator<meta::List<Pin,Tail...>>
		{
			typedef meta::List<Pin,Tail...> Pins;

			inline static uint8_t convertOut( uint8_t data, uint8_t result = 0 )
			{
				typedef typename meta::Filter<IsPinTransparent,Pins>::result transparent_pins;
				if( transparent_pins::size > 0 )
				{
					result |= data & GetPortMask<transparent_pins>::value;
					return PinIterator<typename meta::Filter<IsPinShifted,Pins>::result>::convertOut( data, result );
				}

				const uint8_t serial_count = GetSerialCount<Pins>::value;
				if( serial_count > 1 )
				{
					typedef typename meta::TakeFirst<Pins,serial_count>::result serial_pins;
					result |= Shifter<Pin::type::pos,Pin::index>::shift( data ) & GetPortMask<serial_pins>::value;
					return PinIterator<typename meta::SkipFirst<Pins,serial_count>::result>::convertOut( data, result );
				}

				if( data & (1 << Pin::index) )
					result |= Pin::type::mask;
				return PinIterator<meta::List<Tail...>>::convertOut( data, result );
			}

			inline static uint8_t convertIn( uint8_t data, uint8_t result = 0 )
			{
				typedef typename meta::Filter<IsPinTransparent,Pins>::result transparent_pins;
				if( transparent_pins::size > 0 )
				{
					result |= data & GetPortMask<transparent_pins>::value;
					return PinIterator<typename meta::Filter<IsPinShifted,Pins>::result>::convertIn( data, result );
				}

				const uint8_t serial_count = GetSerialCount<Pins>::value;
				if( serial_count > 1 )
				{
					typedef typename meta::TakeFirst<Pins,serial_count>::result serial_pins;
					result |= Shifter<Pin::type::pos,Pin::index,false>::shift( data ) & GetPortMask<serial_pins>::value;
					return PinIterator<typename meta::SkipFirst<Pins,serial_count>::result>::convertIn( data, result );
				}

				if( data & Pin::type::mask )
					result |= (1 << Pin::index);
				return PinIterator<meta::List<Tail...>>::convertIn( data, result );
			}
		};

		/////////////////////////////////////////////////////////////////////////
		// Port iterator
		/////////////////////////////////////////////////////////////////////////

		template<class Ports,class Pins> struct PortIterator;
		template<class Pins> struct PortIterator<meta::List<>,Pins>
		{
			inline static void setDir( uint8_t data ) { }
			inline static void setOutput( uint8_t data ) { }
			inline static void setInput( uint8_t data ) { }

			inline static void write( uint8_t data ) { }
			inline static void set( uint8_t data ) { }
			inline static void clear( uint8_t data ) { }
			inline static void toggle( uint8_t data ) { }

			inline static uint8_t read() { return 0; }
		};
		template<class Port,class... Tail,class Pins> struct PortIterator<meta::List<Port,Tail...>,Pins>
		{
			// Find pins and calculate mask
			template<typename Pin> struct PortHasPin { static const bool value = meta::IsEqual<Port,typename Pin::type::port>::value; };
			typedef typename meta::Filter<PortHasPin,Pins>::result pins;
			static const uint8_t mask = GetPortMask<pins>::value;

			// Set direction
			inline static void setDir( uint8_t data )
			{
				Port::maskedSetDir( mask, PinIterator<pins>::convertOut( data ) );
				PortIterator<meta::List<Tail...>,Pins>::setDir( data );
			}

			// Set output
			inline static void setOutput( uint8_t data )
			{
				Port::setOutput( PinIterator<pins>::convertOut( data ) );
				PortIterator<meta::List<Tail...>,Pins>::setOutput( data );
			}

			// Set input
			inline static void setInput( uint8_t data )
			{
				Port::setInput( PinIterator<pins>::convertOut( data ) );
				PortIterator<meta::List<Tail...>,Pins>::setInput( data );
			}

			// Write
			inline static void write( uint8_t data )
			{
				Port::maskedSet( mask, PinIterator<pins>::convertOut( data ) );
				PortIterator<meta::List<Tail...>,Pins>::write( data );
			}

			// Set
			inline static void set( uint8_t data )
			{
				Port::set( PinIterator<pins>::convertOut( data ) );
				PortIterator<meta::List<Tail...>,Pins>::set( data );
			}

			// Clear
			inline static void clear( uint8_t data )
			{
				Port::clear( PinIterator<pins>::convertOut( data ) );
				PortIterator<meta::List<Tail...>,Pins>::clear( data );
			}

			// Toggle
			inline static void toggle( uint8_t data )
			{
				Port::toggle( PinIterator<pins>::convertOut( data ) );
				PortIterator<meta::List<Tail...>,Pins>::toggle( data );
			}

			// Read
			inline static uint8_t read()
			{
				uint8_t result = PinIterator<pins>::convertIn( Port::read() );
				return result | PortIterator<meta::List<Tail...>,Pins>::read();
			}
		};
	}

	/////////////////////////////////////////////////////////////////////////
	// Pin list
	/////////////////////////////////////////////////////////////////////////

	template<typename... Pins> class PinList
	{
		// Indexed pin list
		typedef typename meta::MakeIndexedList<Pins...>::result pins;

		// Port list
		template<typename T> struct GetPort { typedef typename T::type::port result; };
		typedef typename meta::Unique<
		typename meta::Transform<GetPort,pins>::result
		>::result ports;

	public:
		inline static void setDir( uint8_t data )    { internal::PortIterator<ports,pins>::setDir( data );    }
		inline static void setOutput( uint8_t data ) { internal::PortIterator<ports,pins>::setOutput( data ); }
		inline static void setInput( uint8_t data )  { internal::PortIterator<ports,pins>::setInput( data );  }

		inline static void write( uint8_t data )  { internal::PortIterator<ports,pins>::write( data ); }
		inline static void set( uint8_t data )    { internal::PortIterator<ports,pins>::set( data ); }
		inline static void clear( uint8_t data )  { internal::PortIterator<ports,pins>::clear( data ); }
		inline static void toggle( uint8_t data ) { internal::PortIterator<ports,pins>::toggle( data ); }

		inline static uint8_t read() { return internal::PortIterator<ports,pins>::read(); }
	};

}
