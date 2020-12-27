/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "sxCRC.h"
#include "sxMsgDefs.h"

#include "sxTTATxMsgEncoder.h"

namespace SX
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

TTATxMsgEncoder::TTATxMsgEncoder()
{
   mTxBuffer[0] = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Encode a transmit message string. Fill the transmit buffer with the
// full string for the given message ident. The string content consists
// of header, payload, and crc footer. This can then be transmitted. This
// does not append a crlf at the end of the string, the serial thread does
// that. Return a pointer to the buffer.

const char* TTATxMsgEncoder::encodeMsg(int aMsgId, const char* aPayload)
{
   // Encode the header.
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

   // Done.
   return mTxBuffer;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace