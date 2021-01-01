/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "smShare.h"
#include "SuperStateTTA.h"
#include "SuperStateDefs.h"
#include "CProcInfo.h"

#define  _CXSUPERWANTSEVALUATORTTA_CPP_
#include "cxSuperWantsEvaluatorTTA.h"

namespace CX
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor

SuperWantsEvaluatorTTA::SuperWantsEvaluatorTTA()
{
   reset();
}

void SuperWantsEvaluatorTTA::reset()
{
   mFirstFlag = true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Evaluate superwants. The superwants are obtained from shared memory.

void SuperWantsEvaluatorTTA::doEvaluate()
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Do this first.

   // Nickname.
   SuperWantsTTA& tTTAW = SM::gShare->mSuperWantsTTA;

   // If this is the first update.
   if (mFirstFlag)
   {
      mFirstFlag = false;
      Prn::print(Prn::TTA1, "TTA Wants        first **********************");

      // Set the preferred rf path from the stored value.
      tTTAW.mPreferRFPath = gCProcInfo.mTTAPreferRFPath;
      tTTAW.mPreferRFPathFlag = 1;
      return;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Evaluate the preferred path.

   // If the preferred rf path is to be set then store it.
   if (tTTAW.mPreferRFPathFlag == 1)
   {
      Prn::print(Prn::TTA1, "TTA Prefer RF Path store **************** %s",
         get_TTA_RFPath_asString(tTTAW.mPreferRFPath));

      gCProcInfo.doReadModifyWriteBegin();
      gCProcInfo.mTTAPreferRFPath = tTTAW.mPreferRFPath;
      gCProcInfo.doReadModifyWriteEnd();
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
