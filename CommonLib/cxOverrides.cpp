/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "SuperStateTTA.h"
#include "SuperStateDA.h"

#define  _CXOVERRIDES_CPP_
#include "cxOverrides.h"

namespace CX
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor

Overrides::Overrides()
{
   reset();
}

void Overrides::reset()
{
   mTTATemperature = -1;
   mTTAMainVoltage = -1;
   mTTAMainCurrent = -1;

   mDATemperature = -1;
   mDAMainInputVoltage = -1;
   mDAMainInputCurrent = -1;
   mDATowerVoltage = -1;
   mDATowerCurrent = -1;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Show.

void Overrides::show(int aPF)
{
   if (!Prn::getFilter(aPF)) return;
   Prn::print(aPF, "");

   Prn::print(aPF, " 1 mTTATemperature          %.1f", mTTATemperature);
   Prn::print(aPF, " 2 mTTAMainVoltage          %.1f", mTTAMainVoltage);
   Prn::print(aPF, " 3 mTTAMainCurrent          %.1f", mTTAMainCurrent);
   Prn::print(aPF, " 4 mDATemperature           %.1f", mDATemperature);
   Prn::print(aPF, " 5 mDAMainInputVoltage      %.1f", mDAMainInputVoltage);
   Prn::print(aPF, " 6 mDAMainInputCurrent      %.1f", mDAMainInputCurrent);
   Prn::print(aPF, " 7 mDATowerVoltage          %.1f", mDATowerVoltage);
   Prn::print(aPF, " 8 mDATowerCurrent          %.1f", mDATowerCurrent);
}

void Overrides::doOverride(SuperStateTTA& aTTA)
{
   if (mTTATemperature >= 0) aTTA.mTemperature = mTTATemperature;
   if (mTTAMainVoltage >= 0) aTTA.mMainVoltage = mTTAMainVoltage;
   if (mTTAMainCurrent >= 0) aTTA.mMainCurrent = mTTAMainCurrent;

}
void Overrides::doOverride(SuperStateDA& aDA)
{
   if (mDATemperature >= 0)      aDA.mTemperature      = mDATemperature;
   if (mDAMainInputVoltage >= 0) aDA.mMainInputVoltage = mDAMainInputVoltage;
   if (mDAMainInputCurrent >= 0) aDA.mMainInputCurrent = mDAMainInputCurrent;
   if (mDATowerVoltage >= 0)     aDA.mTowerVoltage     = mDATowerVoltage;
   if (mDATowerCurrent >= 0)     aDA.mTowerCurrent     = mDATowerCurrent;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
