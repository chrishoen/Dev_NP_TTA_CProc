#pragma once

/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risThreadsTwoThread.h"
#include "risThreadsWaitable.h"
#include "risSerialStringThread.h"

#include "sxTTATxMsgEncoder.h"
#include "sxTTARxMsgDecoder.h"

namespace CX
{
//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is a master thread that sends commands to a slave thread
// and waits for responses.

class TTACommThread : public Ris::Threads::BaseTwoThread
{
public:
   typedef Ris::Threads::BaseTwoThread BaseClass;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Constants:

   // Timer periods.
   static const int cSlowLoopPeriod = 4000;
   static const int cFastLoopPeriod = 2000;

   // Wait timeouts.
   static const int cRxMsgTimeout = 2000;

   // Notification codes.
   static const int cRxMsgNotifyCode = 11;

   // Loop exit status codes.
   static const int cProcExitNormal = 0;
   static const int cProcExitAborted = 1;
   static const int cProcExitError = 2;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Serial  string port child thread. It provides the thread execution
   // context for a serial string port and uses it to provide string
   // communication.
   Ris::SerialStringThread* mSerialStringThread;

   // If true then the serial port is open.
   bool mConnectionFlag;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Notifications.
   Ris::Threads::NotifyWrapper mRxMsgNotify;

   // Waitable timer.
   Ris::Threads::Waitable mLoopWaitable;

   // True if the waitable timer is slow, false if it is fast.
   bool mLoopWaitableSlow;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Process loop exit code.
   int mProcExitCode;

   // Tx message code.
   int mLoopState;

   // Message encoder/decoder.
   SX::TTATxMsgEncoder mTxMsgEncoder;
   SX::TTARxMsgDecoder mRxMsgDecoder;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Metrics.
   int mTxCount;
   int mRxCount;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   TTACommThread();

   // Set the loop waitable timer slow or fast.
   void setLoopWaitableSlow();
   void setLoopWaitableFast();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. Thread base class overloads.

   // Thread init function. This is called by the base class immediately 
   // before the thread starts running.
   void threadInitFunction() override;

   // Thread exit function. This is called by the base class immediately 
   // after the thread starts running.
   void threadExitFunction() override;

   // Thread shutdown function. This shuts down the two threads.
   void shutdownThreads() override;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. 

   // Serial session callback qcall.
   Ris::Threads::QCall1<bool> mSessionQCall;

   // Serial session qcall function. This is invoked by the serial child
   // thread when the serial port is opened or closed because of an error
   // or when it is reopened correctly.
   void executeSession(bool aConnected);

   // Serial receive string callback qcall.
   Ris::Threads::QCall1<std::string*> mRxStringQCall;

   // Receive string qcall function. This is invoked by the serial child
   // thread when a string is received. It executes in the context of the
   // short thread. It decodes and validates the received message and
   // then notifies the long thread.
   void executeRxString(std::string* aString);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. qcalls.

   // Process loop qcall. 
   Ris::Threads::QCall0 mProcessLoopQCall;

   // Process loop qcall function. Execute an infinite loop that sends a request
   // to the slave, waits for the response, and processes it. It calls one
   // of the process subroutines, based on the state. It executes in the
   // context of the long thread. The purpose of this is to provide long
   // thread execution context for message processing. It is only executed
   // once, at thhread initialization.
   void executeProcessLoop();

   // Send a request message to the slave, wait for the response message and
   // process it. Return true if successful. This is called by the process
   // loop qcall function, based on the state.
   bool doProcess();
   bool doProcess_tst();
   bool doProcess_gcs();
   bool doProcess_gbc();
   bool doProcess_gft();
   bool doProcess_gsx();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Send a null terminated string via the serial port. A newline terminator
   // is appended to the string before transmission. This executes in the
   // context of the The calling thread.
   void sendString(const char* aString);

   // Send a null terminated string via the serial port. A newline terminator
   // is appended to the string before transmission. This executes in the
   // context of the The calling thread. The string is deleted after
   // transmission.
   void sendString(std::string* aString);
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
