/*==============================================================================
Detestion:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "cmnShare.h"
#include "sxMsgDefs.h"

#include "acomTestSeqThread.h"

namespace ACom
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a null terminated string via the serial port. A newline terminator
// is appended to the string before transmission. This executes in the
// context of the The calling thread.

void TestSeqThread::sendString(const char* aString)
{
   // Guard.
   if (!mConnectionFlag)
   {
      Prn::print(Prn::Show1, ">>>> NOT CONNECTED");
      return;
   }

   // Metrics.
   mTxCount++;

   // Print the string.
   Prn::print(Prn::Show1, ">>>> %s", aString);

   // Send the string.
   mSerialStringThread->sendString(aString);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a null terminated string via the serial port. A newline terminator
// is appended to the string before transmission. This executes in the
// context of the The calling thread. The string is deleted after
// transmission.

void TestSeqThread::sendString(std::string* aString)
{
   // Guard.
   if (!mConnectionFlag)
   {
      Prn::print(Prn::Show1, ">>>> NOT CONNECTED");
      delete aString;
      return;
   }

   // Metrics.
   mTxCount++;

   // Print the string.
   Prn::print(Prn::Show1, ">>>> %s", aString->c_str());

   // Send the string.
   mSerialStringThread->sendString(aString);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Receive string qcall function. This is invoked by the child thread 
// when a string is received and it processes the received string
// in the context of the short thread.

void TestSeqThread::executeRxString(std::string* aString)
{
   // Metrics.
   mRxCount++;

   Prn::print(Prn::Show1, "<<<< %s", aString->c_str());

   // Signal the notification that an acknowledgement was received.
   mNotify.notify(cCmdAckNotifyCode);

   // Process the receive message.
   mRxMsgDecoder.decodeMsg(aString->c_str());

   // Show.
   Prn::print(Prn::Show1, "Rx Valid  %s", my_string_from_bool(mRxMsgDecoder.mRxValid));
   Prn::print(Prn::Show1, "Rx MsgId  %s", SX::get_MsgId_asString(mRxMsgDecoder.mRxMsgId));

   // Delete the string.
   delete aString;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace