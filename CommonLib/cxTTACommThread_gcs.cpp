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
// Return true if successful. This is called by the above execute on
// timer, based on the state.

bool TTACommThread::doProcess_gcs(int aTimerCount)
{
   mProcExitCode = cProcExitNormal;

   try
   {
      // Test for a notification exception.
      mNotify.testException();

      // Set the thread notification mask.
      mNotify.setMaskOne("CmdAck", cCmdAckNotifyCode);

      // Build a message.
      SM::gShare->mSuperWantsTTA.mCount++;
      char tPayload[200];
      SuperWantsTTA_copyTo(&SM::gShare->mSuperWantsTTA, tPayload);
      SuperWantsTTA_clearFlags(&SM::gShare->mSuperWantsTTA);
      mTxMsgProc.buildMsg(SX::cMsgId_gsx, tPayload);

      // Send the message.
      sendString(mTxMsgProc.mTxBuffer);

      // Wait for the acknowledgement notification.
      mNotify.wait(cCmdAckTimeout);
   }
   catch(int aException)
   {
      mProcExitCode = cProcExitAborted;
      Prn::print(0, "EXCEPTION TTACommThread::doProcess_gcs %d %s", aException, mNotify.mException);
   }

   // Done.
   return mProcExitCode == cProcExitNormal;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace