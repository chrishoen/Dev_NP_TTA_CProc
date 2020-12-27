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

#include "cxBaseCommThread.h"

namespace CX
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Process loop qcall function. Execute an infinite loop that sends a request
// to the slave, waits for the response, and processes it. It calls one
// of the process subroutines, based on the state. It executes in the
// context of the long thread. The purpose of this is to provide long
// thread execution context for message processing. It is only executed
// once, at thhread initialization.

void BaseCommThread::executeProcessLoop()
{
   Prn::print(0, "%sCommThread::executeProcessLoop BEGIN", mLabel);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Do this first.

   // Initialize the synchronization objects.
   mLoopWaitable.initialize(cSlowLoopPeriod);
   mLoopWaitableSlow = true;
   mNotify.clearFlags();

   try
   {
      // Loop to transmit and receive messages.
      while (true)
      {
         //*********************************************************************
         //*********************************************************************
         //*********************************************************************
         // Wait.

         // Wait for timer or abort.
         mLoopWaitable.waitForTimerOrSemaphore();

         // Test for an abort
         if (mLoopWaitable.wasSemaphore()) throw 668;

         // Guard.
         if (!mConnectionFlag) continue;

         //*********************************************************************
         //*********************************************************************
         //*********************************************************************
         // Send a request to the slave, wait for the response and process it.

         try
         {
            // Initialize. This will be set by the specific subfunction.
            mProcExitCode = cProcExitNormal;

            // Test for a notification exception.
            // This can throw an execption if there's an abort.
            mNotify.testException();

            // Set the thread notification mask.
            mNotify.setMaskOne("RxMsg", cRxMsgNotifyCode);

            // Execute a specific subfunction based on the state.
            doProcess();
         }
         catch (int aException)
         {
            if (aException == 667)
            {
               Prn::print(0, "EXCEPTION %sCommThread::doProcess TIMEOUT %s %d",
                  mLabel,
                  SX::get_MsgId_asString(mLoopState),
                  aException);
            }
            else if (aException == cProcExitError)
            {
               Prn::print(0, "EXCEPTION %sCommThread::doProcess ERROR %s %d",
                  mLabel,
                  SX::get_MsgId_asString(mLoopState),
                  aException);
            }
            else
            {
               Prn::print(0, "EXCEPTION %sCommThread::doProcess %s %d %s",
                  mLabel,
                  SX::get_MsgId_asString(mLoopState),
                  aException,
                  mNotify.mException);
            }
         }
      }
   }
   catch (int aException)
   {
      if (aException == 668)
      {
         Prn::print(0, "EXCEPTION %sCommThread::executeProcessLoop %d %s", 
            mLabel, 
            aException, mNotify.mException);
      }
      else
      {
         Prn::print(0, "EXCEPTION %sCommThread::executeProcessLoop %d",
            mLabel,
            aException);
      }
   }

   // Finalize the synchronization objects.
   mLoopWaitable.finalize();
   mNotify.clearFlags();

   Prn::print(0, "%sCommThread::executeProcessLoop END", mLabel);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a request message to the slave, wait for the response message and
// process it. Return true if successful. This is called by the process
// loop qcall function, based on the state.

bool BaseCommThread::doProcess()
{
   if (mLoopState == SX::cMsgId_tst)
   {
      if (doProcess_tst())
      {
         mLoopState = SX::cMsgId_gbc;
         setLoopWaitableSlow();
         return true;
      }
      else
      {
         return false;
      }
   }
   else if (mLoopState == SX::cMsgId_gbc)
   {
      if (doProcess_gbc())
      {
         mLoopState = SX::cMsgId_gft;
         setLoopWaitableSlow();
         return true;
      }
      else
      {
         return false;
      }
   }
   else if (mLoopState == SX::cMsgId_gft)
   {
      if (doProcess_gft())
      {
         mLoopState = SX::cMsgId_gsx;
         setLoopWaitableFast();
         return true;
      }
      else
      {
         return false;
      }
   }
   else if (mLoopState == SX::cMsgId_gsx)
   {
      return doProcess_gsx();
   }
   else if (mLoopState == SX::cMsgId_gcs)
   {
      return doProcess_gcs();
   }
   return false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace