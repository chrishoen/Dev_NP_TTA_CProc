/*==============================================================================
Detestion:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "sxMsgDefs.h"

#include "cxTTACommThread.h"

namespace CX
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Receive string qcall function. This is invoked by the serial child
// thread when a string is received. It executes in the context of the
// short thread. It decodes and validates the received message and
// then notifies the long thread.

void TTACommThread::executeRxString(std::string* aString)
{
   // Metrics.
   mRxCount++;

   Prn::print(mPF1, "<<<< %s", aString->c_str());

   // Decode and validate the receive message.
   mRxMsgDecoder.decodeMsg(aString->c_str());

   // Show.
   Prn::print(mPF1, "Rx Valid  %s", my_string_from_bool(mRxMsgDecoder.mRxValid));
   Prn::print(mPF1, "Rx MsgId  %s", SX::get_MsgId_asString(mRxMsgDecoder.mRxMsgId));

   // Notify the long thread that an acknowledgement was received.
   mNotify.notify(cRxMsgNotifyCode);

   // Delete the string.
   delete aString;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace