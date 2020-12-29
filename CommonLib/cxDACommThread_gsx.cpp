/*==============================================================================
Detestion:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "smShare.h"
#include "sxMsgDefs.h"
#include "cxCProcParms.h"
#include "cxSuperStateEvaluatorDA.h"

#include "cxDACommThread.h"

namespace CX
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a request message to the slave, wait for the response message and
// process it. Return true if successful. This is called by the process
// loop qcall function, based on the state.

bool DACommThread::doProcess_gsx()
{
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
      Prn::print(mPF1, "DA  Proc   gsx superstate ERROR");
      throw cProcExitError;
   }
   Prn::print(mPF1, "DA  Proc   gsx superstate %d", mTxCount);

   // Copy the response message payload into the super state.
   SuperStateDA_copyFrom(&SM::gShare->mSuperStateDA, mRxMsgDecoder.mRxPayload);

   // Evaluate the super state.
   gSuperStateEvaluatorDA.doEvaluate();

   // Update the mode info.
   SM::gShare->doUpdateModeInfoDA();

   // Done.
   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace