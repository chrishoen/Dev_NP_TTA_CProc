/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "sxCRC.h"
#include "sxMsgDefs.h"
#include "sxRxMsgDecoder.h"

namespace SX
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

RxMsgDecoder::RxMsgDecoder()
{
   resetVars();
   mPF1 = 0;
}

void RxMsgDecoder::resetVars()
{
   mRxBuffer[0] = 0;
   mRxPayload[0] = 0;
   mRxValid = false;
   mRxMsgId = 0;
   mRxValMarker = false;
   char* mRxPayload = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Copy the receive message string to the receive buffer and process the 
// receive buffer. Decode and validate the message and set the result member
// variables accordingly. Return true if the message is valid.

bool RxMsgDecoder::decodeMsg(const char* aRxString)
{
   Prn::print(mPF1, "RX ************************");

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Do this first.

   // Reset the result variables.
   resetVars();

   // Store the recevied message.
   strcpy(mRxBuffer, aRxString);

   // Temp buffer index.
   int tIndex = 0;

   Prn::print(mPF1, "RX start %s", &mRxBuffer[0]);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Validate the header1 and extract the val/data marker.

   if (strncmp(&mRxBuffer[0], ";01VAL", 6) == 0)
   {
      mRxValMarker = true;
      tIndex = 6;
   }
   else if (strncmp(&mRxBuffer[0], ";01DATA", 7) == 0)
   {
      mRxValMarker = false;
      tIndex = 7;
   }
   else
   {
      Prn::print(mPF1, "RX header1 FAIL");
      return false;
   }
   Prn::print(mPF1, "RX header1 PASS %d %s", tIndex, my_string_from_bool(mRxValMarker));

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Validate the header2 and extract the message ident.

   char tMsgIdString[40];
   strncpy(tMsgIdString, &mRxBuffer[tIndex], 3);

   Prn::print(mPF1, "RX header2 TEST1 %d %s", tIndex, mRxBuffer);
   Prn::print(mPF1, "RX header2 TEST2 %d %s", tIndex, tMsgIdString);

   mRxMsgId = get_MsgId_asInt(tMsgIdString);

   if (mRxMsgId == 0)
   {
      Prn::print(mPF1, "RX header2 FAIL %d %s", tIndex, tMsgIdString);
      return false;
   }

   tIndex += 3;
   Prn::print(mPF1, "RX header2 PASS %d %s", tIndex, get_MsgId_asString(mRxMsgId));

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Validate the payload crc.

   if (doValidateCRC(&mRxBuffer[tIndex]))
   {
      Prn::print(mPF1, "RX crc PASS");
   }
   else
   {
      Prn::print(mPF1, "RX crc FAIL");
      return false;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Store the payload and strip off the crc.

   strcpy(mRxPayload, &mRxBuffer[tIndex]);
   mRxPayload[strlen(mRxPayload) - 4] = 0;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Done.

   Prn::print(mPF1, "RX done PASS");
   mRxValid = true;
   return true;
}
//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace