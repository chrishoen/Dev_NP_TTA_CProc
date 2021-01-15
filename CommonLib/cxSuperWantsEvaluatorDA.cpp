/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "smShare.h"
#include "SuperStateDA.h"
#include "SuperStateDefs.h"
#include "CProcInfo.h"
#include "cxStatus.h"

#include "cxTTACommThread.h"

#define  _CXSUPERWANTSEVALUATORDA_CPP_
#include "cxSuperWantsEvaluatorDA.h"

namespace CX
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor

SuperWantsEvaluatorDA::SuperWantsEvaluatorDA()
{
   reset();
}

void SuperWantsEvaluatorDA::reset()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Evaluate superwants. The superwants are obtained from shared memory.

void SuperWantsEvaluatorDA::doEvaluate(bool aFirstFlag)
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Do this first.

   // Nickname.
   SuperWantsDA& tDAW = SM::gShare->mSuperWantsDA;

   // If this is the first update.
   if (aFirstFlag)
   {
      Prn::print(Prn::DA1, "DA  Wants        first **********************");
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Evaluate the tta reboot state.

   // Test the tta reboot state for the initial state.
   if (gStatus.mTTARebootState == 0)
   {
      // If the superwants tta reboot code is set.
      if (tDAW.mRebootCodeFlag == 1)
      {
         // Advance the tta reboot state.
         Prn::print(Prn::DA1, "TTA tta reboot state************************** 1");
         gStatus.mTTARebootState = 1;

         // Reset the superwants reboot code. This will be set later.
         tDAW.mRebootCodeFlag = 0;

         // Abort the tta comm thread.
         gTTACommThread->mAbortQCall();
      }
   }

   //***************************************************************************

   // Test the tta reboot state. This is set when the tta comm thread 
   // has been aborted.
   else if (gStatus.mTTARebootState == 2)
   {
      // Advance the tta reboot state.
      Prn::print(Prn::DA1, "DA  tta reboot state************************** 3");
      gStatus.mTTARebootState = 3;

      // Set the superwants reboot code.
      tDAW.mRebootCodeFlag = 1;
   }

   //***************************************************************************

   // Test the tta reboot state.
   else if (gStatus.mTTARebootState == 3)
   {
      // Advance the tta reboot state.
      Prn::print(Prn::DA1, "DA  tta reboot state************************** 4");
      gStatus.mTTARebootState = 4;
   }

   //***************************************************************************

   // Test the tta reboot state.
   else if (gStatus.mTTARebootState == 4)
   {
      // Advance the tta reboot state.
      Prn::print(Prn::DA1, "DA  tta reboot state************************** 5");
      gStatus.mTTARebootState = 5;
   }

   //***************************************************************************

   // Test the tta reboot state.
   else if (gStatus.mTTARebootState == 5)
   {
      // Advance the tta reboot state.
      Prn::print(Prn::DA1, "DA  tta reboot state************************** 6");
      gStatus.mTTARebootState = 6;
   }

   //***************************************************************************

   // Test the tta reboot state.
   else if (gStatus.mTTARebootState == 6)
   {
      // Advance the tta reboot state.
      Prn::print(Prn::DA1, "DA  tta reboot state************************** 7");
      gStatus.mTTARebootState = 7;
   }

   //***************************************************************************

   // Test the tta reboot state.
   else if (gStatus.mTTARebootState == 7)
   {
      // Advance the tta reboot state.
      Prn::print(Prn::DA1, "DA  tta reboot state************************** 8");
      gStatus.mTTARebootState = 8;

      // Start the tta comm thread for seq1.
      gTTACommThread->mRunSeq1QCall();
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Do this first.

}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
