/*==============================================================================
Detestion:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "smShare.h"
#include "sxMsgDefs.h"

#include "cxCProcParms.h"

#include "cxTTACommThread.h"

namespace CX
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Run loop qcall function. Execute an infinite loop that sends a request
// to the slave, waits for the response, and processes it. It calls one
// of the process subroutines, based on the state. It executes in the
// context of the short thread.

void TTACommThread::executeRunLoop()
{
   Prn::print(0, "TTACommThread::executeRunLoop BEGIN");

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Do this first.

   // Initialize the synchronization objects.
   mLoopWaitable.initialize(cSlowLoopPeriod);
   mNotify.clearFlags();

   try
   {
      // Loop to transmit and receive.
      while (true)
      {
         //*********************************************************************
         //*********************************************************************
         //*********************************************************************
         // Wait.

         // Wait for timer or abort.
         mLoopWaitable.waitForTimerOrSemaphore();
         if (mLoopWaitable.wasSemaphore())
         {
            // The waitable semahore was posted for an abort.
            throw 668;
         }

         // Guard.
         if (!mConnectionFlag) continue;

         //*********************************************************************
         //*********************************************************************
         //*********************************************************************
         // Send a request to the slave, wait for the response and process it.

         if (mTxCode == SX::cMsgId_gcs)
         {
            doProcess_gcs();
         }
         else if (mTxCode == SX::cMsgId_gsx)
         {
            doProcess_gsx();
         }

      }
   }
   catch (int aException)
   {
      if (aException == 668)
      {
         Prn::print(0, "EXCEPTION TTACommThread::executeRunLoop %d %s", aException, mNotify.mException);
      }
   }

   // Finalize the synchronization objects.
   mLoopWaitable.finalize();
   mNotify.clearFlags();

   Prn::print(0, "CommSeqThread::doProcessAcquire END");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace