#pragma once

/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "cxBaseCommThread.h"

#include "sxTTATxMsgEncoder.h"
#include "sxTTARxMsgDecoder.h"

namespace CX
{
//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is a master thread that sends commands to a slave thread
// and waits for responses.

class TTACommThread : public BaseCommThread
{
public:
   typedef BaseCommThread BaseClass;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Message encoder/decoder.
   SX::TTATxMsgEncoder mTxMsgEncoder;
   SX::TTARxMsgDecoder mRxMsgDecoder;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   TTACommThread();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. 

   // Receive string qcall function. This is invoked by the serial child
   // thread when a string is received. It executes in the context of the
   // short thread. It decodes and validates the received message and
   // then notifies the long thread.
   void executeRxString(std::string* aString) override;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Send a request message to the slave, wait for the response message and
   // process it. Return true if successful. This is called by the process
   // loop qcall function, based on the state.
   bool doProcess_tst() override;
   bool doProcess_gcs() override;
   bool doProcess_gbc() override;
   bool doProcess_gft() override;
   bool doProcess_gsx() override;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global instance.

#ifdef    _TTACOMMTHREAD_CPP_
          TTACommThread* gTTACommThread = 0;
#else
   extern TTACommThread* gTTACommThread;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
