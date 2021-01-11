/*==============================================================================
Detestion:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "smShare.h"
#include "SuperStateExtraDA.h"

#include "sxMsgDefs.h"
#include "cxCProcParms.h"
#include "cxOverrides.h"
#include "cxSuperStateEvaluatorDA.h"
#include "cxSuperWantsEvaluatorDA.h"

#include "cxDACommThread.h"

namespace CX
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a request message to the slave, wait for the response message and
// process it. Return true if successful. This is called by the process
// sequence qcall function, based on the state.

bool DACommThread::doProcess_gsx()
{
   // Evaluate the super wants.
   gSuperWantsEvaluatorDA.doEvaluate(mFirstFlag_gsx);

   // Encode a request message.
   SM::gShare->mSuperWantsDA.mCount++;
   char tPayload[200];
   SuperWantsDA_copyTo(&SM::gShare->mSuperWantsDA, tPayload);
   SuperWantsDA_clearFlags(&SM::gShare->mSuperWantsDA);
   mTxMsgEncoder.encodeMsg(SX::cMsgId_gsx, tPayload);

   // Transmit the request message.
   sendString(mTxMsgEncoder.mTxBuffer);

   // Throw an exception if there's a timeout. 
   mNotify.wait(cRxMsgTimeout);

   // Test the received response message.
   if (!mRxMsgDecoder.mRxValid)
   {
      Prn::print(mPF1, "DA  Proc    gsx  superstate ERROR");
      throw cSeqExitError;
   }
   Prn::print(mPF1, "DA  Proc    gsx  superstate %d", mTxCount);

   // Copy the response message payload into the super state.
   SuperStateDA_copyFrom(&SM::gShare->mSuperStateDA, mRxMsgDecoder.mRxPayload);

   // Apply test overrides.
   gOverrides.doOverride(SM::gShare->mSuperStateDA);

   // Show.
   SuperStateDA_show2(&SM::gShare->mSuperStateDA, mPF2);

   // if a tta reboot is not in progress, then evaluate the super state.
   if (gSuperWantsEvaluatorDA.mRebootTTACountZero == 0)
   {
      gSuperStateEvaluatorDA.doEvaluate(mFirstFlag_gsx);
   }

   // Done.
   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace