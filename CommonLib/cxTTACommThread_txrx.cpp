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
// Send a null terminated string via the serial port. A newline terminator
// is appended to the string before transmission. This executes in the
// context of the The calling thread.

void TTACommThread::sendString(const char* aString)
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

void TTACommThread::sendString(std::string* aString)
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
// Receive string qcall function. This is invoked by the serial child
// thread when a string is received. It executes in the context of the
// short thread. It decodes and validates the received message and
// then notifies the long thread.

void TTACommThread::executeRxString(std::string* aString)
{
   // Metrics.
   mRxCount++;

   Prn::print(Prn::Show1, "<<<< %s", aString->c_str());

   // Decode and validate the receive message.
   mRxMsgDecoder.decodeMsg(aString->c_str());

   // Show.
   Prn::print(Prn::Show1, "Rx Valid  %s", my_string_from_bool(mRxMsgDecoder.mRxValid));
   Prn::print(Prn::Show1, "Rx MsgId  %s", SX::get_MsgId_asString(mRxMsgDecoder.mRxMsgId));

   // Notify the long thread that an acknowledgement was received.
   mNotify.notify(cRxMsgNotifyCode);

   // Delete the string.
   delete aString;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace