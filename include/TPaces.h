// Author: Peter C. Bender

#ifndef TPACES_H
#define TPACES_H

#include "Globals.h"

#include <vector>
#include <cstdio>

#include "TPacesHit.h"
#ifndef __CINT__
#include "TPacesData.h"
#else
class TPacesData;
#endif
#include "TVector3.h" 

#include "TGRSIDetector.h" 

class TPaces : public TGRSIDetector {

  public:
     TPaces();
     TPaces(const TPaces&);
     virtual ~TPaces();

  public: 
     void BuildHits(TGRSIDetectorData *data =0,Option_t *opt = ""); //!

     TPacesHit *GetPacesHit(int i)           {	return &paces_hits[i];   }	//!
     Short_t GetMultiplicity() const	      {	return paces_hits.size();}	//!

     void FillData(TFragment*,TChannel*,MNEMONIC*); //!

     TPaces& operator=(const TPaces&);  //! 

   private: 
     TPacesData *pacesdata;                  //!  Used to build PACES Hits
     std::vector <TPacesHit> paces_hits;     //   The set of crystal hits
	
     static bool fSetCoreWave;		         //!  Flag for Waveforms ON/OFF
 
     static long fCycleStart;                //!  The start of the cycle
     static long fLastPPG;                   //!  value of the last ppg

     long fCycleStartTime;                   //   The start of the cycle as it's saved to the tree
     bool ftapemove;                         //   flag set during tape move portion of cycle
     bool fbackground;                       //   flag set during background portion of cycle
     bool fbeamon;                           //   flag set during beam on portion of cycle
     bool fdecay;                            //   flag set during decay portion of cycle

   public:
     static bool SetCoreWave()      { return fSetCoreWave;  }	//!

     void SetTapeMove()     { ftapemove = kTRUE; }//!
     void SetBackground()   { fbackground = kTRUE;}//!
     void SetBeamOn()       { fbeamon = kTRUE;}//!
     void SetDecay()        { fdecay = kTRUE;}//!

     bool GetTapeMove()   const { return ftapemove;  }//!
     bool GetBackground() const { return fbackground;}//!
     bool GetBeamOn()     const { return fbeamon;    }//!
     bool GetDecay()      const { return fdecay;     }//!

     static int GetCycleTimeInMilliSeconds(long time) { return (int)((time-fCycleStart)/1e5); }//!

   private:
     void ClearStatus() { ftapemove = kFALSE; fbackground = kFALSE; fbeamon = kFALSE; fdecay = kFALSE;}//!     

   public:         
     virtual void Clear(Option_t *opt = "");		      //!
     virtual void Print(Option_t *opt = "");		//!

   ClassDef(TPaces,1)  // Paces Physics structure for 


};

#endif


