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

   // Timer periods..
   static const int cSlowLoopPeriod = 2000;
   static const int cFastLoopPeriod = 2000;

   // Wait timeouts.
   static const int cCmdAckTimeout = 2000;

   // Notification codes.
   static const int cCmdAckNotifyCode = 11;

   static const int cFlushCmdAckNotifyCode = 17;

   // Loop exit status codes.
   static const int cLoopExitNormal = 0;
   static const int cLoopExitAborted = 1;
   static const int cLoopExitError = 2;

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
   Ris::Threads::NotifyWrapper mCmdAckNotify;

   // Waitable timer.
   Ris::Threads::Waitable mLoopWaitable;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Run test exit code.
   int mLoopExitCode;

   // Tx message code.
   int mTxCode;

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

   // Session callback qcall.
   Ris::Threads::QCall1<bool> mSessionQCall;

   // Session qcall function. This is invoked by the child thread when 
   // the serial port is closed because of an error or when it is reopened
   // correctly.
   void executeSession(bool aConnected);

   // Receive string callback qcall.
   Ris::Threads::QCall1<std::string*> mRxStringQCall;

   // Receive string qcall function. This is invoked by the child thread 
   // when a string is received and it processes the received string
   // in the context of the short thread.
   void executeRxString(std::string* aString);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. qcalls.

   // Run loop qcall. It is invoked by the command line executive.
   Ris::Threads::QCall0 mRunLoopQCall;

   // Run loop qcall function. Execute an infinite loop that sends a request
   // to the slave, waits for the response, and processes it. It calls one
   // of the process subroutines, based on the state. It executes in the
   // context of the short thread.
   void executeRunLoop();

   // Send a request to the slave, wait for the response and process it.
   // Return true if successful. This is called by the run loop qcall
   // function, based on the state.
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
