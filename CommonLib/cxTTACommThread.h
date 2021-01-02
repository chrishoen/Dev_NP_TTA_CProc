#pragma once

/*==============================================================================
tta serial communications thread class.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "cxBaseCommThread.h"

namespace CX
{
//******************************************************************************
//******************************************************************************
//******************************************************************************
// tta serial comm thread. It inherits from the base comm thread and
// provides some tta specific overrides. See the comments for the 
// base comm thread.

class TTACommThread : public BaseCommThread
{
public:
   typedef BaseCommThread BaseClass;

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
   bool doProcess_gbc() override;
   bool doProcess_gsv() override;
   bool doProcess_gft() override;
   bool doProcess_gsx() override;
   bool doProcess_gcs() override;
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
