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

bool TTACommThread::doProcess_gcs()
{
   mLoopExitCode = cLoopExitNormal;

   try
   {
      // Test for a notification exception.
      mNotify.testException();

      // Set the thread notification mask.
      mNotify.setMaskOne("CmdAck", cCmdAckNotifyCode);

      // Build a message.
      mTxMsgProc.buildMsg(SX::cMsgId_gcs);

      // Send the message.
      sendString(mTxMsgProc.mTxBuffer);

      // Wait for the acknowledgement notification.
      mNotify.wait(cCmdAckTimeout);
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