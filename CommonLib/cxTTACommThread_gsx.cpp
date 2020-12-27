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
   mLoopExitCode = cLoopExitNormal;

   try
   {
      // Test for a notification exception.
      mNotify.testException();

      // Set the thread notification mask.
      mNotify.setMaskOne("CmdAck", cRxMsgNotifyCode);

      // Encode a request message.
      SM::gShare->mSuperWantsTTA.mCount++;
      char tPayload[200];
      SuperWantsTTA_copyTo(&SM::gShare->mSuperWantsTTA, tPayload);
      SuperWantsTTA_clearFlags(&SM::gShare->mSuperWantsTTA);
      mTxMsgEncoder.encodeMsg(SX::cMsgId_gsx, tPayload);

      // Transmit the request message.
      sendString(mTxMsgEncoder.mTxBuffer);

      // Wait for the receive response message notification.
      mNotify.wait(cRxMsgTimeout);

      // Decode and validate the received response message.
      if (!mRxMsgDecoder.mRxValid) throw cLoopExitError;

      // Copy the response message payload into the super state.
      SuperStateTTA_copyFrom(&SM::gShare->mSuperStateTTA, mRxMsgDecoder.mRxPayload);
   }
   catch(int aException)
   {
      mLoopExitCode = cLoopExitAborted;
      Prn::print(0, "EXCEPTION TTACommThread::doProcess_gcs %d %s", aException, mNotify.mException);
   }

   // Done.
   return mLoopExitCode == cLoopExitNormal;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace