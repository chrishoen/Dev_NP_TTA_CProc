/*==============================================================================
Detestion:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "cmnPriorities.h"
#include "cxCProcParms.h"
#include "sxMsgDefs.h"

#include "cxBaseCommThread.h"

namespace CX
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor. True is tta, false is da.

BaseCommThread::BaseCommThread(int aTTAFlag)
   : mRxMsgNotify(&mNotify, cRxMsgNotifyCode)
{
   using namespace std::placeholders;

   // Set flag.
   mTTAFlag = aTTAFlag;

   // Set base class thread variables.
   if (mTTAFlag)
   {
      BaseClass::mShortThread->setThreadName("TTACommShort");
      BaseClass::mLongThread->setThreadName("TTACommLong");
   }
   else
   {
      BaseClass::mShortThread->setThreadName("DACommShort");
      BaseClass::mLongThread->setThreadName("DACommLong");
   }

   BaseClass::mShortThread->setThreadPriority(Cmn::gPriorities.mCommShort);
   BaseClass::mShortThread->setThreadPrintLevel(3);

   BaseClass::mLongThread->setThreadPriority(Cmn::gPriorities.mCommLong);
   BaseClass::mLongThread->setThreadPrintLevel(3);

   // Set base class call pointers.
   BaseClass::mShortThread->mThreadInitCallPointer = std::bind(&BaseCommThread::threadInitFunction, this);
   BaseClass::mShortThread->mThreadExitCallPointer = std::bind(&BaseCommThread::threadExitFunction, this);
   BaseClass::mShortThread->mThreadExecuteOnTimerCallPointer = std::bind(&BaseCommThread::executeOnTimer, this, _1);

   // Set qcalls.
   mProcessLoopQCall.bind(this->mLongThread, this, &BaseCommThread::executeProcessLoop);
   mSessionQCall.bind(this->mShortThread, this, &BaseCommThread::executeSession);
   mRxStringQCall.bind(this->mShortThread, this, &BaseCommThread::executeRxString);

   // Set member variables.
   mProcExitCode = 0;
   mTxCount = 0;
   mRxCount = 0;
   mLoopState = SX::cMsgId_tst;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Set the loop waitable timer slow or fast.

void BaseCommThread::setLoopWaitableSlow()
{
   if (mLoopWaitableSlow) return;
   mLoopWaitable.initialize(cSlowLoopPeriod);
   mLoopWaitableSlow = true;
}
void BaseCommThread::setLoopWaitableFast()
{
   if (!mLoopWaitableSlow) return;
   mLoopWaitable.initialize(cFastLoopPeriod);
   mLoopWaitableSlow = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function. This is called by the base class immediately 
// before the thread starts running.

void BaseCommThread::threadInitFunction()
{
   using namespace std::placeholders;

   // Instance of serial port settings.
   Ris::SerialSettings tSerialSettings;

   if (mTTAFlag)
   {
      tSerialSettings.setPortDevice("/dev/ttyO2");
   }
   else
   {
      tSerialSettings.setPortDevice("/dev/ttyO4");
   }

   tSerialSettings.setPortSetup("38400,N,8,1");
   tSerialSettings.mRxTimeout = 0;
   tSerialSettings.mTermMode = Ris::cSerialTermMode_CRLF;
   tSerialSettings.mThreadPriority = Cmn::gPriorities.mSerialString;
   tSerialSettings.mPrintLevel = 0;
   tSerialSettings.mSessionQCall = mSessionQCall;
   tSerialSettings.mRxStringQCall = mRxStringQCall;
   tSerialSettings.m485Flag = true;

   // Create the child thread.
   mSerialStringThread = new Ris::SerialStringThread(tSerialSettings);

   // Launch the child thread.
   mSerialStringThread->launchThread();

   // Launch the loop qcall.
   mProcessLoopQCall();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function. This is called by the base class immediately 
// after  the thread starts running.

void BaseCommThread::threadExitFunction()
{
   // Shutdown the child thread
   mSerialStringThread->shutdownThread();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread shutdown function. This shuts down the two threads.

void BaseCommThread::shutdownThreads()
{
   // Abort the long thread.
   BaseClass::mNotify.abort();
   mLoopWaitable.postSemaphore();

   // Shutdown the two threads.
   BaseClass::shutdownThreads();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Serial session qcall function. This is invoked by the serial child
// thread when the serial port is opened or closed because of an error
// or when it is reopened correctly.

void BaseCommThread::executeSession(bool aConnected)
{
   if (aConnected)
   {
      if (mTTAFlag)
      {
         Prn::print(0, "TTA serial port open  CONNECTED");
      }
      else
      {
         Prn::print(0, "TTA serial port open  CONNECTED");
      }

   }
   else
   {
      if (mTTAFlag)
      {
         Prn::print(0, "DA  serial port error DISCONNECTED");
      }
      else
      {
         Prn::print(0, "DA  serial port error DISCONNECTED");
      }
   }

   mConnectionFlag = aConnected;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace