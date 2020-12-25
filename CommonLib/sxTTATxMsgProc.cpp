/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "sxCRC.h"
#include "sxMsgDefs.h"

#include "sxTTATxMsgProc.h"

namespace SX
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

TTATxMsgProc::TTATxMsgProc()
{
   mTxBuffer[0] = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Build a transmit message string. Fill the transmit buffer with the
// full string for the given message ident. This can then be transmitted. 
// Return a pointer to the buffer.

const char* TTATxMsgProc::buildMsg(int aMsgId, const char* aPayload)
{
   // Build the header.
   strcpy(mTxBuffer, ";10");
   strncat(mTxBuffer, get_MsgId_asString(aMsgId), cMaxStringSize - 1);

   // Concatenate the payload.
   if (aPayload)
   {
      // Concatenate the payload.
      strncat(mTxBuffer, aPayload, cMaxStringSize - 1);

      // Concatenate the payload crc.
      doConcatenateCRC(&mTxBuffer[6]);
   }

   return mTxBuffer;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace