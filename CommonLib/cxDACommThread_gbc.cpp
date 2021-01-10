/*==============================================================================
Detestion:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "smShare.h"
#include "SysInfo.h"

#include "sxMsgDefs.h"
#include "cxCProcParms.h"
#include "BirthCertificate.h"

#include "cxDACommThread.h"

namespace CX
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a request message to the slave, wait for the response message and
// process it. Return true if successful. This is called by the process
// sequence qcall function, based on the state.

bool DACommThread::doProcess_gbc()
{
   // Encode a request message.
   mTxMsgEncoder.encodeMsg(SX::cMsgId_gbc);

   // Transmit the request message.
   sendString(mTxMsgEncoder.mTxBuffer);

   // Wait for the receive response message notification.
   // Throw an exception if there's a timeout. 
   mNotify.wait(cRxMsgTimeout);

   // Test the received response message.
   if (!mRxMsgDecoder.mRxValid)
   {
      Prn::print(mPF1, "DA  Proc   gbc  birth certificate ERROR");
      throw cSeqExitError;
   }
   Prn::print(mPF1, "DA  Proc    gbc  birth certificate");

   // Update the birth certificate json file.
   Prn::print(Prn::DA1, "DA  Update       birth certificate");
   BirthCertificateDA tBirthCertificate;
   tBirthCertificate.fillWithCommandData(mRxMsgDecoder.mRxPayload);
   tBirthCertificate.doWriteToJsonFile();

   // Update the sysinfo json file.
   Prn::print(Prn::DA1, "DA  Update       sysinfo with birth certificate");
   gSysInfo.doReadModifyWriteBegin();
   gSysInfo.readFrom(&tBirthCertificate);
   gSysInfo.doReadModifyWriteEnd();

   // Done.
   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace