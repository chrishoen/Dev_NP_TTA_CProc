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
// Send a request to the slave, wait for the response and process it.
// Return true if successful. This is called by the run loop qcall
// function, based on the state.

bool TTACommThread::doProcess_gsx()
{
   mLoopExitCode = cLoopExitNormal;

   try
   {
      // Test for a notification exception.
      mNotify.testException();

      // Set the thread notification mask.
      mNotify.setMaskOne("CmdAck", cCmdAckNotifyCode);

      // Build a request message.
      SM::gShare->mSuperWantsTTA.mCount++;
      char tPayload[200];
      SuperWantsTTA_copyTo(&SM::gShare->mSuperWantsTTA, tPayload);
      SuperWantsTTA_clearFlags(&SM::gShare->mSuperWantsTTA);
      mTxMsgProc.buildMsg(SX::cMsgId_gsx, tPayload);

      // Send the request message.
      sendString(mTxMsgProc.mTxBuffer);

      // Wait for the receive response message notification.
      mNotify.wait(cCmdAckTimeout);

      // Guard. The receive message proc has saved and validated
      // the response message.
      if (!mRxMsgProc.mRxValid) throw cLoopExitError;
      // Copy the response message payload into the super state.
//    Prn::print(Prn::Show2, "RXPAYLOAD %s", mRxMsgProc.mRxPayload);
      SuperStateTTA_copyFrom(&SM::gShare->mSuperStateTTA, mRxMsgProc.mRxPayload);
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