/*==============================================================================
Detestion:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "smShare.h"
#include "SuperStateExtraTTA.h"

#include "sxMsgDefs.h"
#include "cxCProcParms.h"
#include "cxOverrides.h"
#include "cxSuperStateEvaluatorTTA.h"
#include "cxSuperWantsEvaluatorTTA.h"

#include "cxTTACommThread.h"

namespace CX
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a request message to the slave, wait for the response message and
// process it. Return true if successful. This is called by the process
// sequence qcall function, based on the state.

bool TTACommThread::doProcess_gsx()
{
   // Evaluate the super wants.
   gSuperWantsEvaluatorTTA.doEvaluate(mFirstFlag_gsx);

   // Encode a request message.
   SM::gShare->mSuperWantsTTA.mCount++;
   char tPayload[200];
   SuperWantsTTA_copyTo(&SM::gShare->mSuperWantsTTA, tPayload);
   SuperWantsTTA_clearFlags(&SM::gShare->mSuperWantsTTA);
   mTxMsgEncoder.encodeMsg(SX::cMsgId_gsx, tPayload);

   // Transmit the request message.
   sendString(mTxMsgEncoder.mTxBuffer);

   // Wait for the response message.
   // Throw an exception if there's a timeout. 
   mNotify.wait(cRxMsgTimeout);

   // Test the received response message.
   // Throw an exception if there's an error. 
   if (!mRxMsgDecoder.mRxValid)
   {
      Prn::print(mPF1, "TTA Proc    gsx  superstate ERROR");
      throw cSeqExitError;
   }
   Prn::print(mPF1, "TTA Proc    gsx  superstate %d", mTxCount);

   // Copy the response message payload into the super state.
   SuperStateTTA_copyFrom(&SM::gShare->mSuperStateTTA, mRxMsgDecoder.mRxPayload);

   // Apply test overrides.
   gOverrides.doOverride(SM::gShare->mSuperStateTTA);

   // Show.
   SuperWantsTTA_show2(&SM::gShare->mSuperWantsTTA, mPF2);
   SuperStateTTA_show2(&SM::gShare->mSuperStateTTA, mPF2);

   // Evaluate the super state.
   gSuperStateEvaluatorTTA.doEvaluate(mFirstFlag_gsx);

   // Done.
   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace