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

#define  _TTACOMMTHREAD_CPP_
#include "cxTTACommThread.h"

namespace CX
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

TTACommThread::TTACommThread()
   : mRxMsgNotify(&mNotify, cRxMsgNotifyCode)
{
   using namespace std::placeholders;

   // Set base class thread variables.
   BaseClass::mShortThread->setThreadName("TTACommShort");
   BaseClass::mShortThread->setThreadPriority(Cmn::gPriorities.mCommShort);
   BaseClass::mShortThread->setThreadPrintLevel(3);

   BaseClass::mLongThread->setThreadName("TTACommLong");
   BaseClass::mLongThread->setThreadPriority(Cmn::gPriorities.mCommLong);
   BaseClass::mLongThread->setThreadPrintLevel(3);

   // Set base class call pointers.
   BaseClass::mShortThread->mThreadInitCallPointer           = std::bind(&TTACommThread::threadInitFunction, this);
   BaseClass::mShortThread->mThreadExitCallPointer           = std::bind(&TTACommThread::threadExitFunction, this);
   BaseClass::mShortThread->mThreadExecuteOnTimerCallPointer = std::bind(&TTACommThread::executeOnTimer, this, _1);

   // Set qcalls.
   mProcessLoopQCall.bind(this->mLongThread, this, &TTACommThread::executeProcessLoop);
   mSessionQCall.bind(this->mShortThread, this, &TTACommThread::executeSession);
   mRxStringQCall.bind(this->mShortThread, this, &TTACommThread::executeRxString);

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

void TTACommThread::setLoopWaitableSlow()
{
   if (mLoopWaitableSlow) return;
   mLoopWaitable.initialize(cSlowLoopPeriod);
   mLoopWaitableSlow = true;
}
void TTACommThread::setLoopWaitableFast()
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

void TTACommThread::threadInitFunction()
{
   using namespace std::placeholders;

   // Instance of serial port settings.
   Ris::SerialSettings tSerialSettings;

   tSerialSettings.setPortDevice("/dev/ttyO2");
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

void TTACommThread::threadExitFunction()
{
   // Shutdown the child thread
   mSerialStringThread->shutdownThread();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread shutdown function. This shuts down the two threads.

void TTACommThread::shutdownThreads()
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

void TTACommThread::executeSession(bool aConnected)
{
   if (aConnected)
   {
      Prn::print(0, "TTACommThread serial port open  CONNECTED");
   }
   else
   {
      Prn::print(0, "TTACommThread serial port error DISCONNECTED");
   }

   mConnectionFlag = aConnected;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace