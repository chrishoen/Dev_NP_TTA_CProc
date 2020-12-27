#pragma once

/*==============================================================================
Provides a class definition for the active alarm list record
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <string>

namespace SX
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is a class that implments functions that can be used to validate
// and extract the contents of receive message strings. The strings contain
// the header, data, and  crc footer.

class TTARxMsgProc
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Constants.

   static const int cMaxStringSize = 1000;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Full receive message string to be processed.
   char mRxBuffer[cMaxStringSize];

   // This is true if the receive message is valid.
   bool mRxValid;

   // The receive message identifier.
   int mRxMsgId;

   // If true then marker is val else data.
   bool mRxValMarker;

   // The receive message payload.
   char mRxPayload[cMaxStringSize];

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   TTARxMsgProc();
   void resetVars();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Copy the receive message string to the receive buffer and process
   // the receive buffer. Validate the message, extract message content,
   // and set the message member variables accordingly. Return true if
   // the message is valid.
   bool processMsg(const char* aRxString);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace


