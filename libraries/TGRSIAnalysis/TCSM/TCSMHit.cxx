
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
  if(!IsotopeSet())
  {
    std::cerr<<" Caution: Correcting Energy when Isotope isn't set!"<<std::endl;
    return(GetEnergyMeV());
  }

  return( GetCorrectedEnergyMeV(GetIsotope()));
}

Double_t TCSMHit::GetCorrectedEnergyMeV(TString tempIsotope)
{
  bool debugCE = 0;
  
  Double_t E = GetEnergyMeV();  //Go from keV to MeV
  
  if(debugCE) std::cout<<"My initial energy is: "<<E<<" MeV"<<std::endl;
  double effthick = 2.4/1000.; //This function uses mm not um.
  effthick = effthick/2.; //We assume we only go through half on average.
  if(debugCE) std::cout<<"Half of the thickness of the Target is: "<<effthick<<" mm"<<std::endl;
  effthick = effthick/cos(GetTheta()); //This takes into account the angle effect, the minimum is perpindicular
  if(debugCE) std::cout<<"The effective thickness is: "<<effthick<<" mm, due to an angle of: "<<GetThetaDeg()<<" degrees"<<std::endl;
  
  Double_t elost = 0.;

  tempIsotope.ToLower();

  if(debugCE) std::cout<<"tempIsotope is: "<<tempIsotope<<std::endl;

  double Energy[56]={0.5	,0.55	,0.6	,0.65	,0.7	,0.8	,0.9	,1	,1.1	,1.2	,1.3	,1.4	,1.5	,1.6	,1.7	,1.8	,2	,2.25	,2.5	,2.75	,3	,3.25	,3.5	,3.75	,4	,4.5	,5	,5.5	,6	,6.5	,7	,8	,9	,10	,11	,12	,13	,14	,15	,16	,17	,18	,20	,22.5	,25	,27.5	,30	,32.5	,35	,37.5	,40	,45	,50	,55	,60	,65};

  double StoppingPower[56];
  
  
  if(tempIsotope == "12be")
  {
    double tmpArray[] = {512.028	,533.013	,552.56	,570.855	,588.089	,619.747	,647.792	,672.902	,695.36	,715.354	,733.176	,749.022	,763.086	,775.566	,786.658	,796.362	,812.395	,826.925	,836.586	,842.37	,845.172	,845.488	,843.915	,840.851	,836.4942	,825.1983	,811.4202	,796.2553	,780.3004	,764.0534	,747.7126	,715.8453	,685.392	,656.7487	,630.0127	,605.1823	,582.0563	,560.5338	,540.6141	,521.8967	,504.5812	,488.2674	,458.7436	,426.6194	,398.8998	,371.9836	,346.7699	,326.2582	,308.048	,291.7392	,277.1314	,252.0182	,231.1076	,213.39875	,198.29132	,185.18498};

    for(int i=0; i<56;i++)
      StoppingPower[i] = tmpArray[i];
  }

  else if(tempIsotope ==  "11be")
  {
    double tmpArray[] = {531.108	,552.523	,572.496	,591.213	,608.667	,640.557	,668.629	,693.46	,715.335	,734.644	,751.68	,766.637	,779.812	,791.2	,801.201	,809.812	,823.458	,835.2	,842.072	,845.165	,845.474	,843.496	,839.729	,834.6697	,828.5174	,814.0288	,797.6567	,780.2967	,762.5461	,744.7026	,727.265	,693.4029	,661.7537	,632.3137	,605.0805	,579.9525	,556.7285	,535.3077	,515.3895	,496.9735	,479.7592	,463.8464	,434.8245	,403.7022	,374.3841	,346.7691	,324.4565	,304.8457	,287.5364	,272.1282	,258.321	,234.5089	,214.79908	,198.19095	,183.98411	,171.77826};

    for(int i=0; i<56;i++)
      StoppingPower[i] = tmpArray[i];
  }

  else if(tempIsotope ==  "10be")
  {
    double tmpArray[] = {552.679	,574.526	,594.924	,613.865	,631.538	,663.361	,691.26	,715.513	,736.607	,755.032	,770.881	,784.65	,796.534	,806.632	,815.141	,822.359	,833.318	,841.573	,845.156	,845.358	,842.775	,838.303	,832.2416	,825.0874	,817.0394	,799.1583	,780.0922	,760.5373	,741.0909	,721.8512	,703.2167	,667.6598	,634.9148	,604.8782	,577.3478	,552.1222	,529.0002	,507.6812	,488.0646	,469.8499	,453.0369	,437.4252	,409.2051	,377.1847	,346.5682	,322.1545	,301.043	,282.6331	,266.3246	,251.8171	,238.8105	,216.49946	,198.09048	,182.58306	,169.3768	,158.07146};

    for(int i=0; i<56;i++)
      StoppingPower[i] = tmpArray[i];
  }

  else if(tempIsotope ==  "9be")
  {
    double tmpArray[] = {576.847	,599.224	,619.85	,638.913	,656.607	,688.263	,715.489	,738.764	,758.677	,775.717	,790.18	,802.261	,812.456	,820.863	,827.78	,833.306	,840.877	,845.146	,845.139	,841.95	,836.3748	,829.2101	,820.6539	,811.3047	,801.2611	,780.0875	,758.4275	,736.7777	,715.5356	,695.0995	,675.3683	,638.4167	,604.8758	,574.4426	,546.8151	,521.6918	,498.772	,477.7547	,458.5397	,440.8264	,424.4145	,409.2039	,380.7857	,346.5673	,319.6523	,296.7399	,276.9294	,259.6205	,244.4128	,230.906	,218.8	,198.09002	,181.08189	,166.77516	,154.6695	,144.26466};

    for(int i=0; i<56;i++)
      StoppingPower[i] = tmpArray[i];
  }

  else if(tempIsotope ==  "4he")
  {
    double tmpArray[] = {334.9847	,333.9397	,332.0014	,329.3686	,326.24	,319.0928	,311.1552	,302.9246	,294.6991	,286.5775	,278.759	,271.143	,263.9289	,256.9165	,250.3054	,243.9955	,232.1783	,219.061	,207.3468	,196.8351	,187.5252	,179.0167	,171.4093	,164.4029	,158.09719	,146.8876	,137.37981	,129.07335	,121.86791	,115.56325	,109.95922	,100.35259	,92.04735	,84.3531	,78.32958	,73.18662	,68.73408	,64.84189	,61.40998	,58.35829	,55.62679	,53.15545	,48.87314	,44.48081	,40.86893	,37.84736	,35.28605	,33.06493	,31.13396	,29.44311	,27.93237	,25.38111	,23.2801	,21.539261	,20.058556	,18.787954};

    for(int i=0; i<56;i++)
      StoppingPower[i] = tmpArray[i];
  }

  else if(tempIsotope ==  "6he")
  {
    double tmpArray[] = {327.3591	,330.7931	,333.1372	,334.4891	,335.0473	,334.478	,332.1229	,328.478	,324.1406	,319.2089	,313.9818	,308.6582	,303.1375	,297.6192	,292.2029	,286.8883	,276.4632	,264.1376	,252.7167	,242.1994	,232.4848	,223.4723	,215.2615	,207.552	,200.4436	,187.7295	,176.618	,166.9085	,158.3004	,150.69357	,143.78762	,131.87783	,122.07009	,113.76381	,106.65861	,100.45423	,94.98048	,89.46724	,84.51441	,80.38191	,76.6697	,73.31771	,67.5143	,61.53085	,56.60806	,52.48575	,48.9738	,45.93214	,43.29071	,40.94945	,38.87835	,35.35649	,32.46499	,30.04374	,27.9927	,26.22181};

    for(int i=0; i<56;i++)
      StoppingPower[i] = tmpArray[i];
  }

  else if(tempIsotope ==  "13c")
  {
    double tmpArray[] = {796.23	,830.35	,861.89	,891.04	,918.164	,967.008	,1009.741	,1047.111	,1080.582	,1110.133	,1136.745	,1160.407	,1182.109	,1200.845	,1218.608	,1234.395	,1261.028	,1288.651	,1309.344	,1325.087	,1336.869	,1345.682	,1351.519	,1355.376	,1356.249	,1356.035	,1349.86	,1340.715	,1329.591	,1315.486	,1301.394	,1270.242	,1237.122	,1204.024	,1170.9427	,1138.8739	,1107.815	,1078.764	,1049.7193	,1022.6798	,996.5446	,971.7131	,925.3589	,873.1039	,826.4592	,784.4221	,740.8908	,699.8641	,665.8409	,635.1207	,607.3028	,558.4727	,517.1483	,481.828	,451.111	,424.2964};

    for(int i=0; i<56;i++)
      StoppingPower[i] = tmpArray[i];
  }

  else
  {
    std::cerr<<"Unsupported Isotope in Energy Correction: "<<tempIsotope<<std::endl;
    return(GetEnergyMeV());
  }


  //TGraph *stGraph = new TGraph(56, Energy, StoppingPower);
  TSpline3 *spline = new TSpline3("myspline", Energy, StoppingPower, 56);

  if(debugCE) std::cout<<"Linear Approx is: "<<spline->Eval(E)*effthick<<" MeV"<<std::endl;

  const double stepsize = effthick/100.;

  if(debugCE) std::cout<<"Entering Loop"<<std::endl;
  while(effthick > 0.)
  {
    if(debugCE) std::cout<<"effthick is: "<<effthick<<std::endl;
    if(debugCE) std::cout<<"adding "<<spline->Eval(E+elost)*stepsize<<" to elost"<<std::endl;
    
    elost += spline->Eval(E+elost)*stepsize;

    if(debugCE) std::cout<<"elost is at "<<elost<<std::endl;
    
    effthick -= stepsize;
  }
    
  if(debugCE) std::cout<<"My energy lost is: "<<elost<<" MeV"<<std::endl<<std::endl;
  
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