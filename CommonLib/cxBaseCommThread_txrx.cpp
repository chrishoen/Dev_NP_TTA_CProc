/*==============================================================================
Detestion:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "sxMsgDefs.h"

#include "cxBaseCommThread.h"

namespace CX
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a null terminated string via the serial port. A newline terminator
// is appended to the string before transmission. This executes in the
// context of the The calling thread.

void BaseCommThread::sendString(const char* aString)
{
   // Guard.
   if (!mConnectionFlag)
   {
      Prn::print(mPF1, ">>>> NOT CONNECTED");
      return;
   }

   // Metrics.
   mTxCount++;

   // Print the string.
   Prn::print(mPF1, ">>>> %s", aString);

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

void BaseCommThread::sendString(std::string* aString)
{
   // Guard.
   if (!mConnectionFlag)
   {
      Prn::print(mPF1, ">>>> NOT CONNECTED");
      delete aString;
      return;
   }

   // Metrics.
   mTxCount++;

   // Print the string.
   Prn::print(mPF1, ">>>> %s", aString->c_str());

   // Send the string.
   mSerialStringThread->sendString(aString);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace