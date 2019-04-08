
#include "TTigressHit.h"
#include <TClass.h>

ClassImp(TTigressHit)

TTigressHit::TTigressHit()	{	
   //Class()->IgnoreTObjectStreamer(true);
	Clear();
}

TTigressHit::~TTigressHit()	{	}


void TTigressHit::Clear(Option_t *opt)	{

	detector = -1;
	crystal  = -1;
	first_segment = 0;
	first_segment_charge = 0.0;

	core.Clear();
	for(int x=0;x<segment.size();x++) { 
		segment[x].Clear();
	}
	segment.clear();
	for(int x=0;x<bgo.size();x++)	{
		bgo[x].Clear();
	}
	bgo.clear();

	lasthit.SetXYZ(0,0,0);
   lastenergy = 0.0;
}


void TTigressHit::Print(Option_t *opt)	{
  std::cout<<"TigressHit GetDetectorNumber: "<<GetDetectorNumber()<<std::endl;
  std::cout<<"TigressHit GetCrystalNumber: "<<GetCrystalNumber()<<std::endl;
  std::cout<<"TigressHit GetInitialHit: "<<GetInitialHit()<<std::endl;
  std::cout<<"TigressHit GetCharge: "<<GetCharge()<<std::endl;
  std::cout<<"TigressHit GetEnergy: "<<GetEnergy()<<std::endl;
  
}


bool TTigressHit::Compare(TTigressHit lhs, TTigressHit rhs)	{
	if (lhs.GetDetectorNumber() == rhs.GetDetectorNumber())	{
		return(lhs.GetCrystalNumber() < rhs.GetCrystalNumber());
	}
	else	{
		return (lhs.GetDetectorNumber() < rhs.GetDetectorNumber()); 
	}
}


bool TTigressHit::CompareEnergy(TTigressHit lhs, TTigressHit rhs)	{
		return(lhs.GetEnergy()) > rhs.GetEnergy();
}


void TTigressHit::CheckFirstHit(int charge,int segment)	{
   //printf(DYELLOW "charge = %i  | first_segment_charge = %i \n", charge,first_segment_charge);
   if(abs(charge) > first_segment_charge)	{
 		SetInitialHit(segment);
      first_segment_charge = charge;
	}
	return;				
}

void TTigressHit::Add(TTigressHit *hit)	{
	if(this == hit)	{
		lasthit    = position;
      lastenergy = GetEnergy();
		lastpos    = std::make_tuple(GetDetectorNumber(),GetCrystalNumber(),GetInitialHit());
      SetSuppress(hit->Suppress());
		return;
	}
   this->core.SetEnergy(this->GetEnergy() + hit->GetEnergy());

   if(hit->Suppress()) { this->SetSuppress(true); }  // if any of the add back compents is 
                                                     // suppressed, suppress the entire hit.
   if(lastenergy > hit->GetEnergy()) {
     this->lastenergy = hit->GetEnergy();
     this->lasthit    = hit->GetPosition();
     this->lastpos    = std::make_tuple(hit->GetDetectorNumber(),hit->GetCrystalNumber(),hit->GetInitialHit());
   } else {
     this->GetCore()->SetCfd(hit->GetTimeCFD());    
     this->GetCore()->SetTime(hit->GetTime());
     this->SetPosition(hit->GetPosition());
   }
}



