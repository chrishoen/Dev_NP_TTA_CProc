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

#include "cxTTACommThread.h"

namespace CX
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a request message to the slave, wait for the response message and
// process it. Return true if successful. This is called by the process
// loop qcall function, based on the state.

bool TTACommThread::doProcess_gsx()
{
   // Encode a request message.
   SM::gShare->mSuperWantsTTA.mCount++;
   char tPayload[200];
   SuperWantsTTA_copyTo(&SM::gShare->mSuperWantsTTA, tPayload);
   SuperWantsTTA_clearFlags(&SM::gShare->mSuperWantsTTA);
   mTxMsgEncoder.encodeMsg(SX::cMsgId_gsx, tPayload);

   // Transmit the request message.
   sendString(mTxMsgEncoder.mTxBuffer);

   // Throw an exception if there's a timeout. 
   mNotify.wait(cRxMsgTimeout);

   // Test the received response message.
   if (!mRxMsgDecoder.mRxValid)
   {
      Prn::print(mPF1, "TTA Proc gsx superstate ERROR");
      throw cProcExitError;
   }
   Prn::print(mPF1, "TTA Proc gsx superstate %d", mTxCount);

   // Copy the response message payload into the super state.
   SuperStateTTA_copyFrom(&SM::gShare->mSuperStateTTA, mRxMsgDecoder.mRxPayload);

   // Done.
   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace