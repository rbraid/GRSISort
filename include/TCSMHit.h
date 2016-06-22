//Author: Ryan Braid
#ifndef TCSMHIT_H
#define TCSMHIT_H

#include "Globals.h"

#include <stdio.h>
#include <utility>
#include <iostream>

#include "TMath.h"
#include "TSpline.h"

#include "TChannel.h"
#include "TVector3.h" 

#include "TGRSIDetectorHit.h"

class TCSMHit : public TGRSIDetectorHit 	{
	public:
		TCSMHit();
		~TCSMHit();

	private:

		Short_t			hor_d_strip;	//
		Int_t      	   hor_d_charge;	//	
		Int_t 			hor_d_cfd;		//	

		Short_t			ver_d_strip;		//
		Int_t      	   ver_d_charge;	//
		Int_t  			ver_d_cfd;		// 		

		Short_t			hor_e_strip;	//
		Int_t         	hor_e_charge;	//	
		Int_t 			hor_e_cfd;		//	

		Short_t			ver_e_strip;		//
		Int_t	         ver_e_charge;	//
		Int_t 			ver_e_cfd;		//

		Int_t 		isotope_mass;
		TString		isotope_element;

		Int_t		triggerid;

	  	Double_t hor_d_energy;		//		
	  	Double_t ver_d_energy;		//		
  		Double_t hor_d_time;			//		
  		Double_t ver_d_time;			//		
		TVector3 d_position;	//

  		Double_t hor_e_energy;		//			
  		Double_t ver_e_energy;		//			
  		Double_t hor_e_time;			//		
  		Double_t ver_e_time;			//		

		TVector3 e_position;	//

		//double energy;	//
		//double time;	//

		UShort_t   detectornumber;	//

		//TVector3 position;  //

	public:
	  bool IsEmpty();

		virtual void Clear(Option_t* = "");	//!
		virtual void Print(Option_t* = "") const;	//!

		//static bool Compare(TCSMHit *lhs,TCSMHit *rhs); //!

		Double_t GetEHorizontalEnergy() const	{	return hor_e_energy;	};	//!
		Double_t GetEVerticalEnergy() const	   {	return ver_e_energy;	};	//!
		Double_t GetEEnergy() const ;
		Double_t GetETime() const		         {	return hor_e_time;		}	//!
		
		Double_t GetDHorizontalEnergy() const	{	return hor_d_energy;	};	//!
		Double_t GetDVerticalEnergy() const    {	return ver_d_energy;	};	//!
		Double_t GetDEnergy() const;
		Double_t GetDTime() const		         {	return ver_d_time;		}	//!

		UShort_t GetDetectorNumber() const	   { return detectornumber;	} //!

		Int_t GetEHorizontalCharge() const	   {	return	hor_e_charge;}	//!
		Int_t GetEVerticalCharge()	const  	   {	return	ver_e_charge;	}	//!

		Int_t GetDHorizontalCharge() const	   {	return	hor_d_charge;}	//!
		Int_t GetDVerticalCharge()	const  	   {	return	ver_d_charge;	}	//!


		Double_t GetEHorizontalTime() const	   {	return hor_e_time;	}	//!
		Double_t GetEVerticalTime() const		{	return ver_e_time;	}	//!

		Double_t GetDHorizontalTime() const	   {	return hor_d_time;	}	//!
		Double_t GetDVerticalTime() const		{	return ver_d_time;	}	//!

		Int_t GetTriggerID() const		{ return triggerid; }

		Int_t GetEHorizontalStrip() const     {	return	hor_e_strip;	}	//!
		Int_t GetEVerticalStrip() const   	  {	return	ver_e_strip;	}	//!
		
		Int_t GetDHorizontalStrip() const     {	return	hor_d_strip;	}	//!
		Int_t GetDVerticalStrip() const       {	return	ver_d_strip;	}	//!
	
		Int_t GetEHorizontalCFD() const       {	return hor_e_cfd;	}	//!
		Int_t GetEVerticalCFD() const         {	return ver_e_cfd;	}	//!

		Int_t GetDHorizontalCFD() const    	  {	return hor_d_cfd;	}	//!
		Int_t GetDVerticalCFD()	const         {	return ver_d_cfd;	}	//!

		TVector3 GetEPosition() const     	  {	return e_position;	}	//!
		TVector3 GetDPosition() const         {	return d_position;	}	//!

		Double_t GetDdE_dx() const 		{return GetDEnergy()/GetDthickness();}
		Double_t GetDthickness() const;

		TVector3 GetPosition() const	{	return d_position;	 }	//!
		Double_t GetEnergy() const		{return GetDEnergy() + GetEEnergy();} //!
		Double_t GetTime() const		{return ver_d_time;} //!

		Double_t GetEnergyMeV() const		{return GetEnergy()/1000.;}
		Double_t GetThetaDeg() const 		{return GetDPosition().Theta()*180/TMath::Pi();}
		Double_t GetTheta() const 		{return GetDPosition().Theta();}
		TString GetIsotope() const;
		Double_t GetCorrectedEnergyMeV();
		Double_t GetCorrectedEnergyMeV(TString);
		Double_t GetCorrectedEnergyMeVTarget(double);
		Double_t GetCorrectedEnergyMeVTarget(double,TString);
		Double_t GetCorrectedEnergyMeVDetector();
		Double_t GetCorrectedEnergyMeVDetector(TString);
		Double_t GetCorrectedEnergy()		{return GetCorrectedEnergyMeV()*1000.;}
		Double_t GetCorrectedEnergy(const TString tmp)		{return GetCorrectedEnergyMeV(tmp)*1000.;}
		bool IsotopeSet();
		Double_t GetMassMeV() const;

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

		inline void  SetDetectorNumber(const Int_t &tempnum)			{ detectornumber = tempnum;	} //!

		inline void SetEHorizontalCharge(const Int_t temp) 	{hor_e_charge = temp;}	//!
		inline void SetEVerticalCharge(const Int_t temp)	{ver_e_charge  = temp;}	//!

		inline void SetDHorizontalCharge(const Int_t temp) 	{hor_d_charge = temp;}	//!
		inline void SetDVerticalCharge(const Int_t temp)	{ver_d_charge  = temp;}	//!

		inline void SetEHorizontalStrip(const Int_t temp)	{hor_e_strip  = temp;}	//!
		inline void SetEVerticalStrip(const Int_t temp)		{ver_e_strip   = temp;}	//!

		inline void SetDHorizontalStrip(const Int_t temp)	{hor_d_strip  = temp;}	//!
		inline void SetDVerticalStrip(const Int_t temp)		{ver_d_strip   = temp;}	//!
	
		inline void SetEHorizontalCFD(const Int_t tempd)	{ hor_e_cfd = tempd;	}	//!
		inline void SetEVerticalCFD(const Int_t tempd)	{ ver_e_cfd  = tempd;	}	//!

		inline void SetDHorizontalCFD(const Int_t tempd)	{ hor_d_cfd = tempd;	}	//!
		inline void SetDVerticalCFD(const Int_t tempd)	{ ver_d_cfd  = tempd;	}	//!


		inline void SetEHorizontalTime(const Int_t tempd)	{ hor_e_time = tempd;	}	//!
		inline void SetEVerticalTime(const Int_t tempd)	  { ver_e_time  = tempd;	}	//!

		inline void SetDHorizontalTime(const Int_t tempd)	{ hor_d_time = tempd;	}	//!
		inline void SetDVerticalTime(const Int_t tempd)	  { ver_d_time  = tempd;	}	//!



		inline void SetEPosition(TVector3 tempp)	{ e_position = tempp;	}	//!
		inline void SetDPosition(TVector3 tempp)	{ d_position = tempp;	}	//!


		inline void SetTriggerID(const Int_t tempi)	{ triggerid = tempi; }


		inline void SetDHorizontalEnergy(const Double_t tempd) { hor_d_energy = tempd; }
		inline void SetDVerticalEnergy(const Double_t tempd) { ver_d_energy = tempd; }

		inline void SetEHorizontalEnergy(const Double_t tempd) { hor_e_energy = tempd; }
		inline void SetEVerticalEnergy(const Double_t tempd) { ver_e_energy = tempd; }

		//void SetHorizontalWave(std::vector<int> &wave)	{ hor_strip_wave = wave;	} 
		//void SetVerticalWave(std::vector<int> &wave)	{ ver_strip_wave = wave;	} 
		//void SetPadWave(std::vector<int> &wave)	{ pad_wave = wave;	}

		void SetIsotope(Int_t Mass, TString Element);
		void SetIsotope(TString);

		
	ClassDef(TCSMHit,3)  //CSM Hits
};




#endif
