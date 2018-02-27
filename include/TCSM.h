#ifndef TCSM_H
#define TCSM_H

#include "Globals.h"

#include <vector>
#include <cstdio>
#include <map>
#include <iostream>
#ifndef __CINT__
#include <tuple>
#include <iterator>
#include <algorithm>
#endif
#include <utility>
#include <set>


#include "TFragment.h"
#include "TChannel.h"

#ifndef __CINT__
#include "TCSMData.h"
#else
class TCSMData;
#endif
#include "TCSMHit.h"

#include "TMath.h"
#include "TVector3.h"
#include "TRandom3.h"

#include "TGRSIDetector.h"

#ifndef PI
#define PI                       (TMath::Pi())
#endif

class TCSM :  public TGRSIDetector 	{
	public:
		TCSM();
		~TCSM();

	public: 
		virtual void Clear(Option_t * = "");		//!
		virtual void Print(Option_t * = "");		//!
		void BuildHits(TGRSIDetectorData *cd = 0,  Option_t * = "");			//!
        void SetHits(std::vector<TCSMHit> tmp);
        
		TCSMHit *GetHit(int i)		{return &csm_hits.at(i);}	//->
		Short_t GetMultiplicity()	{return csm_hits.size();}	//->

	   static TVector3 GetPosition(int detector, char pos, int horizontalstrip, int verticalstrip, double X=0.00, double Y=0.00, double Z=0.00);	//! 
       static TVector3 GetPositionsNew(int detector,char pos, int horizontalstrip, int verticalstrip);
       
      void FillData(TFragment*,TChannel*,MNEMONIC*);

	private: 
		TCSMData *data;                             //!
		std::vector <TCSMHit> csm_hits;
		void BuildVH(vector<int> &,vector<int> &,vector<TCSMHit> &,TCSMData*);
		void BuilddEE(vector<TCSMHit> &,vector<TCSMHit> &,vector<TCSMHit> &);
		void OldBuilddEE(vector<TCSMHit> &,vector<TCSMHit> &,vector<TCSMHit> &);
		void MakedEE(vector<TCSMHit> &DHitVec,vector<TCSMHit> &EHitVec,vector<TCSMHit> &BuiltHits);
		TCSMHit MakeHit(int, int, TCSMData*);
		TCSMHit MakeHit(vector<int> &,vector<int> &, TCSMData*);
		TCSMHit CombineHits(TCSMHit d_hit,TCSMHit e_hit);
		void RecoverHit(char, int, TCSMData *, vector<TCSMHit> &);
		bool AlmostEqual(int, int);
		bool AlmostEqual(double,double);
		
		
		double AlmostEqualWindow;

		//int CombineHits(TCSMHit*,TCSMHit*,int,int);				//!
		//void RemoveHits(std::vector<TCSMHit>*,std::set<int>*);	//!

		static int fCfdBuildDiff; //!   // largest acceptable time difference between events (clock ticks)  (50 ns)

        
   ClassDef(TCSM,4)  // CSM Analysis structure
};


#endif
