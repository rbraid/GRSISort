

#include "TCrystalHit.h"
#include <TClass.h>


ClassImp(TCrystalHit)


TCrystalHit::TCrystalHit(): suppress(false)	{
   Class()->IgnoreTObjectStreamer(true);
	Clear();
}

TCrystalHit::~TCrystalHit()	{	}

void TCrystalHit::Clear(Option_t *opt)	{
	segment = -1;
	charge  = 0xffffffff;
	
	energy = 0.0;
	time = 0.0;
	cfd = 0.0;

	wave.clear();
}


void TCrystalHit::Print(Option_t *opt)	{
   std::cout<<"Get Segment: "<<GetSegment()      <<std::endl;
   std::cout<<"Get Charge: "<<GetCharge()        <<std::endl;
   std::cout<<"Get Energy: "<<GetEnergy()        <<std::endl;
   std::cout<<"Get Time: "<<GetTime()            <<std::endl;
   std::cout<<"Get Cfd: "<<GetCfd()              <<std::endl;
   std::cout<<"Get Efficiency: "<<GetEfficiency()<<std::endl;
   std::cout<<"Is Suppressed: "<<Suppress()<<std::endl;
}

