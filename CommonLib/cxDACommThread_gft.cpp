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

#include "FactoryTestRecordDA.h"

#include "cxDACommThread.h"

namespace CX
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a request message to the slave, wait for the response message and
// process it. Return true if successful. This is called by the process
// loop qcall function, based on the state.

bool DACommThread::doProcess_gft()
{
   // Encode a request message.
   mTxMsgEncoder.encodeMsg(SX::cMsgId_gft);

   // Transmit the request message.
   sendString(mTxMsgEncoder.mTxBuffer);

   // Wait for the receive response message notification.
   // Throw an exception if there's a timeout. 
   mNotify.wait(cRxMsgTimeout);

   // Test the received response message.
   if (!mRxMsgDecoder.mRxValid)
   {
      Prn::print(mPF1, "DA  Proc    gft  factory test record ERROR");
      throw cProcExitError;
   }
   Prn::print(mPF1, "DA  Proc    gft  factory test record");

   // Update the factory test record.
   Prn::print(mPF1, "DA  Update       factory test record");
   FactoryTestRecordDA tFactoryTestRecord;
   tFactoryTestRecord.fillWithCommandData(mRxMsgDecoder.mRxPayload);
   tFactoryTestRecord.doWriteToJsonFile();

   // Done.
   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace