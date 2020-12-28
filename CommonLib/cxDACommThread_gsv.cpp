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

#include "cxDACommThread.h"

namespace CX
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a request message to the slave, wait for the response message and
// process it. Return true if successful. This is called by the process
// loop qcall function, based on the state.

bool DACommThread::doProcess_gsv()
{
   // Encode a request message.
   mTxMsgEncoder.encodeMsg(SX::cMsgId_gsv);

   // Transmit the request message.
   sendString(mTxMsgEncoder.mTxBuffer);

   // Wait for the receive response message notification.
   // Throw an exception if there's a timeout. 
   mNotify.wait(cRxMsgTimeout);

   // Test the received response message.
   if (!mRxMsgDecoder.mRxValid)
   {
      Prn::print(mPF1, "DA  Proc gsv software version ERROR");
      throw cProcExitError;
   }
   Prn::print(mPF1, "DA  Proc gsv software version");

   // Done.
   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace