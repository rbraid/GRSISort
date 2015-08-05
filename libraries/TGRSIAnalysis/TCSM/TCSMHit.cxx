
#include "TCSMHit.h"

////////////////////////////////////////////////////////////////
//
// TCSMHit
//
// This class contains individual CSM Hits.
// Data such as hit energy and position is here.
//
////////////////////////////////////////////////////////////////

ClassImp(TCSMHit)

TCSMHit::TCSMHit()	{	
   Class()->IgnoreTObjectStreamer(true);
   Clear();
}

TCSMHit::~TCSMHit()	{	}

void TCSMHit::SetIsotope(Int_t Mass, TString Element)
{
//Sets isotope mass and element.
  
  //Element.ToLower(); //This is just to standardize things.
  
  if( isotope_mass != -1 || isotope_element != "default")
  {
    if( isotope_mass != Mass || isotope_element != Element)
      std::cerr<<"  Warning: Istope redefinition.  Overwriting."<<std::endl;
  }
  
  Element.ToLower();
  
  isotope_mass = Mass;
  isotope_element = Element;
}

void TCSMHit::SetIsotope(TString Info)
{
  TString Mass;
  TString Element;
  
  if(TString(Info(0,2)).IsDigit())
  {
    Mass=Info(0,2);
    Element=Info(2,4);
  }
  else if(TString(Info(2,4)).IsDigit())
  {
    Mass=Info(2,4);
    Element=Info(0,2);
  }

  Element.ToLower();

  if( isotope_mass != -1 || isotope_element != "default")
  {
    if( isotope_mass != Mass || isotope_element != Element)
      std::cerr<<"  Warning: Istope redefinition.  Overwriting."<<std::endl;
  }

  std::cout<<"Testing SetIsotope, input: "<<Info<<", Parsed Mass: "<<Mass<<" , Parsed Element: "<<Element<<std::endl;
  isotope_mass = Mass.Atof();
  isotope_element = Element;
}

TString TCSMHit::GetIsotope()
{
  TString returnedString;

  returnedString.Clear();

  returnedString = Form("%d", isotope_mass) + isotope_element;

  if(returnedString == "-1default")
    returnedString = "default";  //I like to drop the -1 for usability reasons.

  return returnedString;
}

Double_t TCSMHit::GetCorrectedEnergyMeV()
{
  bool debugCE = 0;
  if(!IsotopeSet())
  {
    std::cerr<<" Caution: Correcting Energy when Isotope isn't set!"<<std::endl;
    return(GetEnergyMeV());
  }
  
  Double_t E = GetEnergyMeV();  //Go from keV to MeV
  
  if(debugCE) std::cout<<"My initial energy is: "<<E<<" MeV"<<std::endl;
  double effthick = 2.4/1000.; //This function uses mm not um.
  effthick = effthick/2; //We assume we only go through half on average.
  if(debugCE) std::cout<<"Half of the thickness of the Target is: "<<effthick<<" mm"<<std::endl;
  effthick = effthick/cos(GetTheta()); //This takes into account the angle effect, the minimum is perpindicular
  if(debugCE) std::cout<<"The effective thickness is: "<<effthick<<" mm, due to an angle of: "<<GetThetaDeg()<<" degrees"<<std::endl;
  
  Double_t elost = -10.;
  
  if(GetIsotope()=="11be")
    elost = 1080.85*effthick - 33.4462*pow(effthick,2) + 0.92379*pow(effthick,3) - 0.0160147*pow(effthick,4) + 0.0001199256*pow(effthick,5);
  else if(GetIsotope()=="12be")
    elost = 1208.8*effthick - 41.8894*pow(effthick,2) + 1.25691*pow(effthick,3) - 0.0223553*pow(effthick,4) + 0.0001661186*pow(effthick,5);
  else if(GetIsotope()=="4he")
    elost = 166.114*effthick - 6.2685*pow(effthick,2) + 0.168201*pow(effthick,3) - 0.002540775*pow(effthick,4) + 0.00001618362*pow(effthick,5);
  else
    elost = -100;
  
  if(debugCE) std::cout<<"My energy lost is: "<<elost<<" MeV"<<std::endl;
  
  return((E+elost));
}

bool TCSMHit::IsotopeSet()
{
  if(GetIsotope() == "default")
    return 0;
  else
    return 1;
}

void TCSMHit::Clear(Option_t *options)	{
//Sets all private variables to their defaults.
//NOTE:  defaults aren't always 0!
   hor_d_strip 	= -1;
   hor_d_charge = 0;
   hor_d_cfd    = 0.0;
	
   ver_d_strip  = -1;
   ver_d_charge	= 0;
   ver_d_cfd	= 0.0;      
	
   hor_e_strip  = -1;
   hor_e_charge = 0;
   hor_e_cfd    = 0.0;
	
   ver_e_strip  = -1;
   ver_e_charge = 0;
   ver_e_cfd    = 0.0;      

   hor_d_energy = 0.0;   
   ver_d_energy = 0.0;   
   hor_d_time   = 0.0;   
   ver_d_time   = 0.0;   
   d_position.SetXYZ(0,0,1);
	
   hor_e_energy = 0.0;
   ver_e_energy = 0.0;
   hor_e_time   = 0.0;
   ver_e_time   = 0.0;
   e_position.SetXYZ(0,0,1);

   isotope_mass = -1;
   isotope_element = "default";

   detectornumber = 0;	//
}

Double_t TCSMHit::GetDthickness() const
{
  double CSMthick[2][16][16]={
    38.35,38.75,36.35,36.05,35.85,35.45,35.15,35.2,34.5,34.65,35.05,35.05,35.55,34.65,34.45,32.05,
    39.25,39.65,38.85,38.15,37.65,36.55,35.95,35.25,34.35,34.75,35.35,35.25,36.45,34.75,33.55,31.95,
    38.85,39.05,38.45,38.15,37.45,36.45,35.15,34.25,33.45,33.75,34.15,34.25,35.25,34.65,32.85,31.25,
    38.15,38.05,37.55,36.85,35.85,35.55,34.55,33.45,32.95,33.25,33.45,33.65,34.15,34.25,32.95,31.55,
    36.75,36.95,35.95,35.85,34.65,34.75,34.85,33.65,33.15,33.15,33.15,33.45,33.65,33.85,33.25,31.95,
    36.25,36.45,35.75,35.35,34.35,34.75,34.65,34.35,33.55,33.05,33.15,33.05,33.35,33.65,33.95,32.75,
    36.75,36.45,35.75,35.45,34.95,34.65,34.95,35.15,34.65,33.55,33.55,32.95,33.55,33.15,33.85,32.65,
    37.05,36.35,35.55,35.45,35.05,34.45,34.75,37.15,37.05,34.15,33.85,33.15,33.35,32.35,32.95,31.65,
    37.45,36.35,35.25,35.45,34.85,34.65,34.95,36.65,36.45,33.65,33.25,33.05,32.95,32.05,32.35,31.05,
    37.05,36.05,35.05,35.35,35.05,35.05,34.75,34.35,33.45,32.85,33.25,33.55,33.05,32.15,32.55,30.95,
    37.25,36.85,35.15,35.15,34.95,34.95,34.55,33.75,32.95,32.75,33.55,33.95,33.55,32.85,33.25,31.25,
    36.95,36.05,35.05,35.25,35.35,35.35,35.15,34.65,33.75,33.75,34.65,35.35,34.85,34.05,34.75,32.55,
    37.05,37.05,36.05,35.95,36.15,35.95,35.95,35.95,35.75,35.25,35.15,35.65,35.55,35.25,35.35,33.95,
    37.95,38.35,37.35,37.05,36.65,36.55,36.75,36.75,36.45,36.45,35.35,35.45,36.05,35.55,35.85,34.25,
    38.65,38.75,37.75,37.05,36.25,36.55,36.55,36.95,36.65,36.55,36.05,35.05,35.75,35.55,35.95,34.45,
    39.35,39.25,38.35,37.65,36.55,36.85,36.85,37.35,36.85,36.75,35.85,35.15,35.05,35.75,35.25,34.05,
    
    38.35,38.75,36.35,36.05,35.85,35.45,35.15,35.2,34.5,34.65,35.05,35.05,35.55,34.65,34.45,32.05,
    39.25,39.65,38.85,38.15,37.65,36.55,35.95,35.25,34.35,34.75,35.35,35.25,36.45,34.75,33.55,31.95,
    38.85,39.05,38.45,38.15,37.45,36.45,35.15,34.25,33.45,33.75,34.15,34.25,35.25,34.65,32.85,31.25,
    38.15,38.05,37.55,36.85,35.85,35.55,34.55,33.45,32.95,33.25,33.45,33.65,34.15,34.25,32.95,31.55,
    36.75,36.95,35.95,35.85,34.65,34.75,34.85,33.65,33.15,33.15,33.15,33.45,33.65,33.85,33.25,31.95,
    36.25,36.45,35.75,35.35,34.35,34.75,34.65,34.35,33.55,33.05,33.15,33.05,33.35,33.65,33.95,32.75,
    36.75,36.45,35.75,35.45,34.95,34.65,34.95,35.15,34.65,33.55,33.55,32.95,33.55,33.15,33.85,32.65,
    37.05,36.35,35.55,35.45,35.05,34.45,34.75,37.15,37.05,34.15,33.85,33.15,33.35,32.35,32.95,31.65,
    37.45,36.35,35.25,35.45,34.85,34.65,34.95,36.65,36.45,33.65,33.25,33.05,32.95,32.05,32.35,31.05,
    37.05,36.05,35.05,35.35,35.05,35.05,34.75,34.35,33.45,32.85,33.25,33.55,33.05,32.15,32.55,30.95,
    37.25,36.85,35.15,35.15,34.95,34.95,34.55,33.75,32.95,32.75,33.55,33.95,33.55,32.85,33.25,31.25,
    36.95,36.05,35.05,35.25,35.35,35.35,35.15,34.65,33.75,33.75,34.65,35.35,34.85,34.05,34.75,32.55,
    37.05,37.05,36.05,35.95,36.15,35.95,35.95,35.95,35.75,35.25,35.15,35.65,35.55,35.25,35.35,33.95,
    37.95,38.35,37.35,37.05,36.65,36.55,36.75,36.75,36.45,36.45,35.35,35.45,36.05,35.55,35.85,34.25,
    38.65,38.75,37.75,37.05,36.25,36.55,36.55,36.95,36.65,36.55,36.05,35.05,35.75,35.55,35.95,34.45,
    39.35,39.25,38.35,37.65,36.55,36.85,36.85,37.35,36.85,36.75,35.85,35.15,35.05,35.75,35.25,34.05};
    
  return CSMthick[GetDetectorNumber()-1][GetDVerticalStrip()][GetDHorizontalStrip()];
}


Double_t TCSMHit::GetDEnergy() const
{
  bool debug = 0;
  bool trustVertical = 1;
  bool trustHoriztonal = 1;
  
  switch(GetDetectorNumber())
  {
    case 1:
    switch(GetDHorizontalStrip())
    {
      case 6:
      case 9:
      trustHoriztonal = 0;
      break;
    }
    break;

    case 2:
    switch(GetDHorizontalStrip())
    {
      //case 8:
      case 9:
      case 10:
      trustHoriztonal = 0;
      break;
    }
    switch(GetDVerticalStrip())
    {
      case 5:
      trustVertical = 0;
      break;
    }
    break;

    case 3:
      switch(GetDHorizontalStrip())
      {
	case 12:
	case 15:
	  trustHoriztonal = 0;
	  break;
      }
    break;

    case 4:

    break;

    default:
    std::cerr<<"  ERROR, Trying to get energy from a D detector that doesn't exist!"<<std::endl;
  }

  if((!trustVertical || !trustHoriztonal) && debug)
  {
    std::cout<<std::endl<<"  GetDEnergy() output: (V,H)"<<std::endl;
    std::cout<<"  Detector: "<<GetDetectorNumber()<<std::endl;
    std::cout<<"  Strip: "<<GetDVerticalStrip()<<" "<<GetDHorizontalStrip()<<std::endl;
    std::cout<<"  Trustworthy: "<<trustVertical<<" "<<trustHoriztonal<<std::endl;
    std::cout<<"  Energy: "<<GetDVerticalEnergy()<<" "<<GetDHorizontalEnergy()<<std::endl;
  }
  

  if(trustVertical && trustHoriztonal)
  {
    //if(debug) std::cout<<"  Returning: "<<(GetDVerticalEnergy() + GetDHorizontalEnergy())/2.<<std::endl;
    return((GetDVerticalEnergy() + GetDHorizontalEnergy())/2.);
  }
  else if(trustVertical && !trustHoriztonal)
  {
    if(debug) std::cout<<"**Returning: "<<GetDVerticalEnergy()<<std::endl;
    return(GetDVerticalEnergy());
  }
  else if(!trustVertical && trustHoriztonal)
  {
    if(debug) std::cout<<"**Returning: "<<GetDHorizontalEnergy()<<std::endl;
    return(GetDHorizontalEnergy());
  }
  else if(!trustVertical && !trustVertical) //Are these correct??? RD
    return(0.);
  else{
    std::cerr<<"  ERROR, I don't know who to trust in GetDEnergy()"<<std::endl;
    return -1; //I added this here so that there is guaranteed a return at the end of the function RD 
  }
}

Double_t TCSMHit::GetEEnergy() const
{
  bool trustVertical = 1;
  bool trustHoriztonal = 1;
  
  switch(GetDetectorNumber())
  {
    case 1:

      break;

    case 2:
      switch(GetEHorizontalStrip())
      {
	case 0:
	case 2:
	case 3:
	case 6:
	//case 8:
	case 12:
	//case 15:
	trustHoriztonal = 0;
	break;
      }
      switch(GetEVerticalStrip())
      {
	case 0:
	trustVertical = 0;
	break;
      }
      break;

    case 3:
      break;

    case 4:
      break;

    default:
      std::cerr<<"  ERROR, Trying to get energy from a E detector that doesn't exist!"<<std::endl;
  }
  
  if(trustVertical && trustHoriztonal)
    return((GetEVerticalEnergy() + GetEHorizontalEnergy())/2.);
  else if(trustVertical && !trustHoriztonal)
    return(GetEVerticalEnergy());
  else if(!trustVertical && trustHoriztonal)
    return(GetEHorizontalEnergy());
  else if(!trustVertical && !trustVertical) //Are these correct? RD
    return(0.);
  else{
    return -1; //I added this here so that there is guaranteed a return at the end of the function RD 
    std::cerr<<"  ERROR, I don't know who to trust in GetEEnergy()"<<std::endl;
  }

}

void TCSMHit::Print(Option_t *options) const	{
  std::cout<<"Printing TCSMHit:  Horizontal    Vertical"<<std::endl;
  std::cout<<"Detector number: "<<GetDetectorNumber()<<std::endl;
  //std::cout<<"Detector position: "<<GetDetectorPosition()<<std::endl;
  std::cout<<"D"<<std::endl;
  std::cout<<"Strip: "<<GetDHorizontalStrip()<<" "<<GetDVerticalStrip()<<std::endl;
  
  std::cout<<"Charge: "<<GetDHorizontalCharge()<<" "<<GetDVerticalCharge()<<std::endl;

  std::cout<<"CFD: "<<GetDHorizontalCFD()<<" "<<GetDVerticalCFD()<<std::endl;
  
  std::cout<<"Energy: "<<GetDHorizontalEnergy()<<" "<<GetDVerticalEnergy()<<std::endl;
  
  std::cout<<"Theta: "<<GetDPosition().Theta()*180./3.14159<<" Phi: "<<GetDPosition().Phi()*180./3.14159<<std::endl;

  std::cout<<"E"<<std::endl;
  std::cout<<"Strip: "<<GetEHorizontalStrip()<<" "<<GetEVerticalStrip()<<std::endl;
  
  std::cout<<"Charge: "<<GetEHorizontalCharge()<<" "<<GetEVerticalCharge()<<std::endl;
  
  std::cout<<"CFD: "<<GetEHorizontalCFD()<<" "<<GetEVerticalCFD()<<std::endl;
  
  std::cout<<"Energy: "<<GetEHorizontalEnergy()<<" "<<GetEVerticalEnergy()<<std::endl;
  
  std::cout<<"Theta: "<<GetEPosition().Theta()*180./3.14159<<" Phi: "<<GetEPosition().Phi()*180./3.14159<<std::endl;
  
  std::cout<<std::endl;
}

bool TCSMHit::IsEmpty()
{
  bool isempty=0;
  if(hor_d_strip == -1)
    if(hor_d_charge == 0)
      if(hor_d_cfd == 0.0)
	if(ver_d_strip == -1)
	  if(ver_d_charge == 0)
	    if(ver_d_cfd == 0.0)
	      if(hor_e_strip == -1)
		if(hor_e_charge == 0)
		  if( hor_e_cfd == 0.0)
		    if(ver_e_strip == -1)
		      if(ver_e_charge == 0)
			if(ver_e_cfd == 0.0)
			  if(hor_d_energy == 0.0)
			    if(ver_d_energy == 0.0)
			      if(hor_d_time == 0.0)
				if(ver_d_time == 0.0)
				  if(hor_e_energy == 0.0)
				    if(ver_e_energy == 0.0)
				      if(hor_e_time == 0.0)
					if(ver_e_time == 0.0)
					  if(detectornumber == 0)
					    isempty = 1;
  return isempty;
}
