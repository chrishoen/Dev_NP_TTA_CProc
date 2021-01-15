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
#include "cxStatus.h"

#include "cxTTACommThread.h"

namespace CX
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a request message to the slave, wait for the response message and
// process it. Return true if successful. This is called by the process
// sequence qcall function, based on the state.

bool TTACommThread::doProcess_tst()
{
   // Encode a request message.
   mTxMsgEncoder.encodeMsg(SX::cMsgId_tst);

   // Transmit the request message.
   sendString(mTxMsgEncoder.mTxBuffer);

   // Wait for the receive response message notification.
   // Throw an exception if there's a timeout. 
   mNotify.wait(cRxMsgTimeout);

   // Test the received response message.
   if (!mRxMsgDecoder.mRxValid)
   {
      Prn::print(mPF1, "TTA Proc    tst  test message ERROR");
      throw cSeqExitError;
   }
   Prn::print(mPF1, "TTA Proc    tst  test message");

   // Test the tta reboot state.
   if (gStatus.mTTARebootState == 8)
   {
      // Advance the tta reboot state.
      Prn::print(Prn::DA1, "TTA tta reboot state************************** 0");
      gStatus.mTTARebootState = 0;
   }

   // Done.
   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace