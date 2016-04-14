
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
  
  Element.ToLower(); //This is just to standardize things.
  
  if( isotope_mass != -1 || isotope_element != "default")
  {
    if( isotope_mass != Mass || isotope_element != Element)
    {
      std::cerr<<"  Warning: Istope redefinition.  Overwriting."<<std::endl;
      std::cerr<<"  "<<isotope_mass<<isotope_element<<" --> "<<Mass<<Element<<std::endl;
    }
  }
    
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
    if( isotope_mass != Mass.Atof() || isotope_element != Element)
    {
      std::cerr<<"  Warning: Istope redefinition.  Overwriting."<<std::endl;
      std::cerr<<"  "<<isotope_mass<<isotope_element<<" --> "<<Mass.Atof()<<Element<<std::endl;
    }
  }

  //std::cout<<"Testing SetIsotope, input: "<<Info<<", Parsed Mass: "<<Mass<<" , Parsed Element: "<<Element<<std::endl;
  isotope_mass = Mass.Atof();
  isotope_element = Element;
}

TString TCSMHit::GetIsotope() const
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

Double_t TCSMHit::GetCorrectedEnergyMeV(TString tempIsotope)
{
  bool debugCE = 0;
  
  Double_t E = GetEnergyMeV();  //Go from keV to MeV
  
  if(debugCE) std::cout<<"My initial energy is: "<<E<<" MeV"<<std::endl;
  double effthick = 2.4/1000.; //This function uses mm not um.
  effthick = effthick/2; //We assume we only go through half on average.
  if(debugCE) std::cout<<"Half of the thickness of the Target is: "<<effthick<<" mm"<<std::endl;
  effthick = effthick/cos(GetTheta()); //This takes into account the angle effect, the minimum is perpindicular
  if(debugCE) std::cout<<"The effective thickness is: "<<effthick<<" mm, due to an angle of: "<<GetThetaDeg()<<" degrees"<<std::endl;
  
  Double_t elost = -10.;

  tempIsotope.ToLower();

  if(debugCE) std::cout<<"tempIsotope is: "<<tempIsotope<<std::endl;
  
  if(tempIsotope=="11be")
    elost = 1080.85*effthick - 33.4462*pow(effthick,2) + 0.92379*pow(effthick,3) - 0.0160147*pow(effthick,4) + 0.0001199256*pow(effthick,5);
  else if(tempIsotope=="12be")
    elost = 1208.8*effthick - 41.8894*pow(effthick,2) + 1.25691*pow(effthick,3) - 0.0223553*pow(effthick,4) + 0.0001661186*pow(effthick,5);
  else if(tempIsotope=="4he")
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

   triggerid = -1;

   detectornumber = 0;	//
}

Double_t TCSMHit::GetDthickness() const
{
  double CSMthick[2][16][16]={
    36.45,36.35,35.85,35.85,35.65,35.45,35.05,35.15,34.85,35.35,35.05,34.95,35.65,34.55,34.35,31.95,
    39.25,39.65,38.85,38.25,37.75,36.55,35.85,35.25,34.35,34.75,35.35,35.25,36.45,34.75,33.55,28.85,
    38.85,39.05,38.45,38.25,37.45,36.45,35.15,34.25,33.45,33.75,34.05,34.25,35.25,34.65,32.85,31.25,
    38.15,38.05,37.55,36.85,35.85,35.55,34.55,33.45,32.95,33.25,33.45,33.65,34.15,34.15,32.95,31.45,
    36.75,36.95,35.95,35.85,34.65,34.75,34.85,33.65,33.15,33.15,33.15,33.45,33.65,33.85,33.25,31.95,
    36.25,36.45,35.75,35.35,34.35,34.75,34.65,34.35,33.55,33.05,33.15,33.05,33.35,33.65,33.95,32.75,
    36.75,36.45,35.75,35.45,34.95,34.65,34.95,35.15,34.65,33.55,33.55,32.95,33.55,33.15,33.85,32.75,
    37.05,36.35,35.55,35.45,35.05,34.45,34.75,37.15,37.05,34.15,33.85,33.15,33.35,32.35,32.95,31.65,
    37.45,36.35,35.25,35.45,34.85,34.65,34.95,36.65,36.45,33.65,33.25,33.05,32.95,32.05,32.35,31.05,
    37.05,36.05,35.05,35.35,35.05,35.05,34.75,34.35,33.55,32.85,33.25,33.55,33.05,32.15,32.55,31.65,
    37.25,36.95,35.15,35.15,34.95,34.95,34.55,33.75,32.95,32.75,33.55,34.05,33.55,32.85,33.25,32.35,
    36.95,36.15,35.05,35.25,35.35,35.35,35.15,34.65,33.75,33.75,34.65,35.35,34.85,34.05,34.75,32.55,
    37.05,37.05,36.05,35.95,36.15,35.95,35.95,35.95,35.75,35.25,35.15,35.65,35.55,35.25,35.35,28.75,
    37.95,38.35,37.35,36.95,36.65,36.55,36.75,36.75,36.45,36.45,35.45,35.45,36.15,35.55,35.85,30.95,
    38.65,38.75,37.65,36.95,36.25,36.55,36.55,36.95,36.65,36.55,36.05,35.05,35.75,35.55,35.95,28.65,
    39.35,39.25,38.25,37.75,36.55,36.75,26.55,30.15,29.45,25.45,29.75,30.35,30.75,34.95,31.55,28.85,
    
    44.45,44.05,43.35,43.75,38.55,38.25,41.95,37.15,37.45,35.85,37.75,39.15,38.35,38.35,46.25,47.75,
    43.05,43.35,43.05,43.25,44.35,42.75,41.95,42.35,42.05,34.55,42.25,43.35,43.65,44.15,45.25,46.55,
    28.95,40.35,39.55,38.75,37.95,36.85,36.85,36.85,36.05,35.45,37.45,36.95,36.35,37.45,38.05,38.35,
    41.55,41.35,41.75,41.65,41.65,42.85,42.75,42.25,41.45,42.55,41.55,41.65,41.85,42.85,43.85,44.75,
    41.55,41.15,41.25,41.45,41.15,42.15,42.55,42.05,39.85,40.95,41.25,41.05,41.35,42.65,43.15,43.75,
    42.25,41.55,40.85,41.45,40.95,41.15,42.25,40.25,39.75,0,0,41.15,41.25,42.05,42.35,43.15,
    42.25,41.65,40.45,40.75,40.55,40.05,40.85,41.15,39.85,41.55,39.95,40.85,41.25,41.65,41.75,41.95,
    42.35,42.05,40.95,40.55,40.85,40.15,39.75,41.75,40.95,25.15,39.75,41.15,40.95,40.85,40.95,41.45,
    42.65,42.15,41.45,40.75,40.45,40.65,39.95,40.55,40.55,40.35,39.65,40.55,40.05,40.35,40.65,41.05,
    43.05,42.25,41.75,41.35,40.35,40.55,40.05,39.75,40.15,39.95,39.35,39.25,39.85,40.25,40.15,40.55,
    43.75,43.15,41.55,41.45,40.45,39.95,39.95,39.55,39.85,39.85,40.35,39.35,39.35,40.35,40.35,40.55,
    44.55,44.25,42.05,42.05,40.85,40.15,40.25,40.15,40.25,40.35,40.85,40.15,40.15,41.45,41.65,41.75,
    44.55,44.25,43.35,42.55,41.55,40.55,40.65,40.65,40.15,40.35,41.55,40.75,40.75,41.85,42.25,42.35,
    45.15,44.65,44.05,42.85,42.15,41.45,41.25,40.95,40.65,41.15,41.75,41.85,41.15,42.05,42.95,43.35,
    45.55,45.05,44.65,44.05,42.55,42.25,41.95,41.45,41.45,42.05,42.15,43.15,42.35,42.25,43.25,44.25,
    42.25,42.65,46.35,43.45,41.95,41.75,41.75,40.95,41.05,41.65,41.35,43.15,42.55,42.45,43.25,44.55,
  };
    
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
  std::cout<<"TriggerID: "<<GetTriggerID()<<std::endl;
  std::cout<<"Isotope: "<<GetIsotope()<<std::endl;
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

Double_t TCSMHit::GetMassMeV() const
{
  double mass = -1.;

  const double MEVpNUC = 931.494061;
  const double MASS_BE8 = 8*MEVpNUC+4.9416;
  const double MASS_BE12 = 12*MEVpNUC+25.0766;
  const double MASS_BE9 = 9*MEVpNUC+11.3484;
  const double MASS_BE11 = 11*MEVpNUC+20.1771;
  const double MASS_HE4 = 4*MEVpNUC+2.4249;
  const double MASS_C12 = 12*MEVpNUC;
  const double MASS_C13 = 13*MEVpNUC+3.1250;
  const double MASS_BE10 = 10*MEVpNUC+12.6074;

  if(GetIsotope()=="default")
    std::cerr<<"Error: GetMassMeV for an unset Isotope, returning -1"<<std::endl;
  else if(GetIsotope()=="8be")
    mass=MASS_BE8;
  else if(GetIsotope()=="12be")
    mass=MASS_BE12;
  else if(GetIsotope()=="9be")
    mass=MASS_BE9;
  else if(GetIsotope()=="11be")
    mass=MASS_BE11;
  else if(GetIsotope()=="4he")
    mass=MASS_HE4;
  else if(GetIsotope()=="c12")
    mass=MASS_C12;
  else if(GetIsotope()=="c13")
    mass=MASS_C13;
  else if(GetIsotope()=="10be")
    mass=MASS_BE10;
  else
    std::cerr<<"Error: GetMassMeV for an unsupported isotope, returning -1"<<std::endl;

  return mass;
}