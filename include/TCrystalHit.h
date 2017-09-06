#ifndef TCRYSTALHIT_H
#define TCRYSTALHIT_H

#include <cstdio>
#include <vector>
#include <iostream>

#include "TGRSIDetectorHit.h"


class TCrystalHit : public TGRSIDetectorHit	{

	public:
		TCrystalHit();
		virtual ~TCrystalHit();

	private: 
		int segment;		      //
		int charge;		         //

		double energy;	         //
		double time;		      //
		double cfd;		         //
		double effweight;

      bool   suppress;        //

		std::vector<int> wave;	//!

	public:
		
		virtual void Clear(Option_t *opt = "");		//!
		virtual void Print(Option_t *opt = "");		//!

		inline int    GetSegment()  {   return segment;}       //!
		inline int    GetCharge()	{	return charge; }	//!		
		inline double GetEnergy()	{	return energy;	}	//!
		inline double GetTime()		{	return time;	}	//!
		inline double GetCfd()		{	return cfd;	}	//!
		inline double GetEfficiency(){   return effweight;} //!

		inline void SetSegmentNumber(const int &seg) { segment = seg;   }       //!	
		inline void SetCharge(const int &chg)	{	charge = chg;	}	//!
		inline void SetEnergy(const double &e)	{	energy = e;	}	//!
		inline void SetTime(const double &t)	{	time = t;	}	//!
		inline void SetCfd(const double &c)	{	cfd = c;	}	//!
		inline void SetEfficiency(const double &w) { effweight = w;} //!

      inline void SetSuppress(const bool flag = true) { suppress = flag;} 
      inline bool Suppress() {return suppress;}

		inline void SetWave(const std::vector<int> &w)	{	wave = w;	} //!
		inline std::vector<int> *GetWave()	{	return &wave;	}	  //!


	ClassDef(TCrystalHit,1)
};

#endif
