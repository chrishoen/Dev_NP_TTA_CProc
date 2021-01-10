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

#include "FactoryTestRecordTTA.h"

#include "cxTTACommThread.h"

namespace CX
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a request message to the slave, wait for the response message and
// process it. Return true if successful. This is called by the process
// sequence qcall function, based on the state.

bool TTACommThread::doProcess_gft()
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
      Prn::print(mPF1, "TTA Proc    gft  factory test record ERROR");
      throw cSeqExitError;
   }
   Prn::print(mPF1, "TTA Proc    gft  factory test record");

   // Update the factory test record.
   Prn::print(mPF1, "TTA Update       factory test record");
   FactoryTestRecordTTA tFactoryTestRecord;
   tFactoryTestRecord.fillWithCommandData(mRxMsgDecoder.mRxPayload);
   tFactoryTestRecord.doWriteToJsonFile();

   // Update the gain calculator json file.
   Prn::print(mPF1, "TTA Update       gain calc with factory test record");
   Calc::GainCalc* tCalc = &SM::gShare->mGainCalc;
   tCalc->doReadModifyWriteBegin();
   tCalc->readFrom(&tFactoryTestRecord);
   tCalc->doReadModifyWriteEnd();

   // Done.
   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace