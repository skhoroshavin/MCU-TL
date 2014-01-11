
#include "gpio.h"
#include "PinList.h"

#include "mcucpp/ioports.h"
#include "mcucpp/iopins.h"
#include "mcucpp/pinlist.h"

template<class Port,uint8_t Pos> class Pin
{
public:
	typedef Port port;
	static constexpr uint8_t pos  = Pos;
	static constexpr uint8_t mask = (1 << Pos);

	inline static void set() { Port::set( mask ); }
	inline static void clear() { Port::clear( mask ); }
	inline static void write( bool value ) { if( value ) set(); else clear(); }
};

typedef Pin<PortA,0> Pa0;
typedef Pin<PortA,1> Pa1;
typedef Pin<PortA,2> Pa2;
typedef Pin<PortA,3> Pa3;

typedef Pin<PortB,0> Pb0;
typedef Pin<PortB,1> Pb1;
typedef Pin<PortB,2> Pb2;
typedef Pin<PortB,3> Pb3;
typedef Pin<PortB,4> Pb4;
typedef Pin<PortB,5> Pb5;
typedef Pin<PortB,6> Pb6;
typedef Pin<PortB,7> Pb7;

typedef gpio::PinList<Pb1,Pb3,Pb5,Pb7> p1;

using namespace Mcucpp;
typedef IO::PinList<IO::Pb1,IO::Pb3,IO::Pb5,IO::Pb7> p2;

int main()
{
	//p1::write( PORTD );
	p2::Write( PORTD );

	//p1::write( 0 );
	//p2::Write( 0 );
	while(1);
	return 0;
}
