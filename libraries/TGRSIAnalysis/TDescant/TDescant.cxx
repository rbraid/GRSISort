
#include <iostream>
#include "TDescant.h"
#include <TRandom.h>
#include <TMath.h>

////////////////////////////////////////////////////////////
//                    
// TDescant
//
// The TDescant class defines the observables and algorithms used
// when analyzing GRIFFIN data. It includes detector positions,
// etc. 
//
////////////////////////////////////////////////////////////

ClassImp(TDescant)


bool TDescant::fSetWave = true;

TVector3 TDescant::gPosition[21] = { 
   //Sceptar positions from Evan; Thanks Evan.
   TVector3(0,0,1),
   TVector3(14.3025, 4.6472, 22.8096),  
   TVector3(0, 15.0386, 22.8096), 
   TVector3(-14.3025, 4.6472, 22.8096),  
   TVector3(-8.8395, -12.1665, 22.8096),  
   TVector3(8.8395, -12.1665, 22.8096),     
   TVector3(19.7051, 6.4026, 6.2123),  
   TVector3(0, 20.7192, 6.2123),  
   TVector3(-19.7051, 6.4026, 6.2123),  
   TVector3(-12.1784, -16.7622, 6.2123),  
   TVector3(12.1784, -16.7622, 6.2123),     
   TVector3(19.7051, 6.4026, -6.2123),  
   TVector3(0, 20.7192, -6.2123),  
   TVector3(-19.7051, 6.4026, -6.2123),  
   TVector3(-12.1784, -16.7622, -6.2123),  
   TVector3(12.1784, -16.7622, -6.2123),
   TVector3(14.3025, 4.6472, -22.8096),  
   TVector3(0, 15.0386, -22.8096),  
   TVector3(-14.3025, 4.6472, -22.8096),  
   TVector3(-8.8395, -12.1665, -22.8096),  
   TVector3(8.8395, -12.1665, -22.8096)
};


TDescant::TDescant() : descantdata(0)	{
   //Default Constructor
   //Class()->IgnoreTObjectStreamer(true);
   //Class()->AddRule("TDescant descant_hits attributes=NotOwner");
   //Class()->AddRule("TDescant descantdata attributes=NotOwner");
   Clear();
}

TDescant::~TDescant()	{
   //Default Destructor
   if(descantdata) delete descantdata;
}

void TDescant::Clear(Option_t *opt)	{
//Clears all of the hits and data
   if(descantdata) descantdata->Clear();

	descant_hits.clear();
}



TDescant& TDescant::operator=(const TDescant& rhs) {
     descantdata     = 0;
     descant_hits = rhs.descant_hits;
     fSetWave = rhs.fSetWave;

     return *this;
}


void TDescant::Print(Option_t *opt)	{
  //Prints out TDescant members, currently does nothing.
  printf("not yet written...\n");
  return;
}


void TDescant::FillData(TFragment *frag, TChannel *channel, MNEMONIC *mnemonic) {
//Fills the "Data" structure for a specific channel with TFragment frag.
   if(!frag || !channel || !mnemonic)
      return;

   if(!descantdata)   
      descantdata = new TDescantData();

   descantdata->SetDet(frag,channel,mnemonic);
   TDescantData::Set();
}


void TDescant::BuildHits(TGRSIDetectorData *data,Option_t *opt)	{
//Builds the GRIFFIN Hits from the "data" structure. Basically, loops through the data for and event and sets observables. 
//This is done for both GRIFFIN and it's suppressors.
   TDescantData *gdata = (TDescantData*)data;
   if(gdata==0)
      gdata = (this->descantdata);

   if(!gdata)
      return;

   descant_hits.clear();
   TDescant::SetHit(false);
   
   for(int i=0;i<gdata->GetMultiplicity();i++)	{
      TDescantHit dethit;

      dethit.SetDetectorNumber(gdata->GetDetNumber(i));
   
      dethit.SetAddress(gdata->GetDetAddress(i));
      
      dethit.SetEnergy(gdata->GetDetEnergy(i));
      dethit.SetCharge(gdata->GetDetCharge(i));

      dethit.SetTime(gdata->GetDetTime(i));
      dethit.SetCfd(gdata->GetDetCFD(i));

      if(TDescant::SetWave()){
         if(gdata->GetDetWave(i).size() == 0) {
            //printf("Warning, TDescant::SetWave() set, but data waveform size is zero!\n");
         }
         dethit.SetWaveform(gdata->GetDetWave(i));
         if(dethit.GetWaveform().size() > 0) {
            //printf("Analyzing waveform, current cfd = %d, psd = %d\n",dethit.GetCfd(),dethit.GetPsd());
            bool analyzed = dethit.AnalyzeWaveform();
            //printf("%s analyzed waveform, cfd = %d, psd = %d\n",analyzed ? "successfully":"unsuccessfully",dethit.GetCfd(),dethit.GetPsd());
         }
      }
		
      dethit.SetPosition(TDescant::GetPosition(gdata->GetDetNumber(i)));

      descant_hits.push_back(dethit);
      TDescant::SetHit();
   }
}
