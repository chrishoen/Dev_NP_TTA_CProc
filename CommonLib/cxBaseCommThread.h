#pragma once

/*==============================================================================
base serial comm thread class for tta and da serial comm threads.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risThreadsTwoThread.h"
#include "risThreadsWaitable.h"
#include "risSerialStringThread.h"

#include "sxTxMsgEncoder.h"
#include "sxRxMsgDecoder.h"

namespace CX
{
//******************************************************************************
//******************************************************************************
//******************************************************************************
//
// This is a base class for the tta and da serial communications threads.
// It has only those two threads as inheritors.
//
// The serial communications between cproc and the tta and the da utilize
// two rs485 serial channels. The rs485 scheme is based on a master/slave
// concept.
//
// The following will describe behaviour that is relevant to the tta.
// The behavoiur for the da is exactly the same. The messages are the
// but the message contents are different
// 
// The cproc tta comm thread is the master and the tta is the slave. 
// The tta comm thread enters a sequence that executes a sequence that sends
// a request message to the tta, waits for the response message and then
// processes the response. The tta only sends a message to cproc in
// response to a request message that it receives from cproc.
//    
// The sequence timing of a single send, wait for response, process response
// is controllled by an abort able wait mechanism. This makes the timing
// periodic.
// 
//******************************************************************************
//******************************************************************************
//******************************************************************************
//
// The sequence goes through a set of five messages via a simple state
// machine. There is a sixth message that's used for debug.
//
//    MsgId_tst = 1;    // test message
//    MsgId_gbc = 2;    // birth certificate message
//    MsgId_gsv = 3;    // software version message
//    MsgId_gft = 4;    // factory test record message
//    MsgId_gsx = 5;    // superstate message
//    MsgId_gcs = 6;    // common share debug message
// 
// The sequence is as follows, starting at sequence initialization.
// 
// 1) send a test message request,
//    wait for the response.
// 
// 2) send a birth certificate message request,
//    wait for the response.
//    process the response.
// 
// 3) send a software version message request,
//    wait for the response.
//    process the response.
// 
// 4) send a factory test record message request,
//    wait for the response.
//    process the response.
// 
// 5) send a superstate message request, this contains the superwants
//    wait for the response.
//    process the response.
// 
// 6) repeat step 5)
//
// If there are any timeouts or errors then start again at step 1).
//
//******************************************************************************
//******************************************************************************
//******************************************************************************
//
// The base comm thread has a member that is a ris serial string thread.
// This is used to send strings to the slave and receive strings from
// the slave. The serial thread sends received message strings to this
// thread via a ris qcall. The serial thread manages all of the message
// string crlf terminators.
//
// The base comm thread inherits from the ris base two thread class.
// This means that it has two threads: a long term thread and a short
// term thread. Both are ris qcall threads.
// 
// The long thread executes the sequence qcall and it provides the execution
// context for the message processing sequence. The sequence qcall is executed
// at initialization and contains an infinite sequence that processes the
// messages. The sequence only exits if it is aborted or the thread is
// terminated.
//
// The short thread executes ris qcalls sent by the ris serial receive 
// child thread. It validates received messages and notifies the long
// thread when messages are received. The long thread sends request
// messages and then waits for the notification from the short thread.
//
//******************************************************************************
//******************************************************************************
//******************************************************************************

class BaseCommThread : public Ris::Threads::BaseTwoThread
{
public:
   typedef Ris::Threads::BaseTwoThread BaseClass;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Constants:

   // Timer periods.
   static const int cSlowSeqPeriod = 2000;
   static const int cFastSeqPeriod = 2000;

   // Wait timeouts.
   static const int cRxMsgTimeout = 2000;

   // Notification codes.
   static const int cRxMsgNotifyCode = 11;

   // Seq exit status codes.
   static const int cSeqExitNormal = 0;
   static const int cSeqExitAborted = 1;
   static const int cSeqExitError = 2;

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

   // if true then tta else da.
   bool mTTAFlag;
   char mLabel[4];

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Notifications.
   Ris::Threads::NotifyWrapper mRxMsgNotify;

   // Waitable timer.
   Ris::Threads::Waitable mSeqWaitable;

   // True if the waitable timer is slow, false if it is fast.
   bool mSeqWaitableSlow;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Process sequence exit code.
   int mSeqExitCode;

   // Seq state. Based on message bing processed.
   int mSeqState;

   // Seq execution times.
   double mSeqTime1;
   double mSeqTime2;
   double mSeqDuration;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Message encoder/decoder.
   SX::TxMsgEncoder mTxMsgEncoder;
   SX::RxMsgDecoder mRxMsgDecoder;

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
   // Members.

   // Print filters.
   int mPF1;
   int mPF2;
   int mPF3;
   int mPF4;
   int mPF5;
   int mPF6;
   int mPF7;
   int mPF8;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   bool mBirthCertificateValid;
   bool mSoftwareVersionValid;
   bool mFactoryTestRecordValid;
   bool mSysInfoValid;
   bool mGainCalcValid;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor. True is tta, false is da.
   BaseCommThread(int aTTAFlag);

   // Set the sequence waitable timer slow or fast.
   void setSeqWaitableSlow();
   void setSeqWaitableFast();

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
   virtual void executeRxString(std::string* aString) = 0;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. qcalls.

   // Run sequence qcall. 
   Ris::Threads::QCall0 mRunSeq1QCall;

   // Run sequence qcall function. Execute an infinite loop sequence that
   // sends a request to the slave, waits for the response, and processes it.
   // It calls one of the process subroutines, based on the state. It executes
   // in the context of the long thread. The purpose of this is to provide
   // long thread execution context for message processing.
   void executeRunSeq1();

   // Send a request message to the slave, wait for the response message and
   // process it. Return true if successful. This is called by the process
   // sequence qcall function, based on the state.
   bool doProcess();
   virtual bool doProcess_tst() = 0;
   virtual bool doProcess_gbc() = 0;
   virtual bool doProcess_gsv() = 0;
   virtual bool doProcess_gft() = 0;
   virtual bool doProcess_gsx() = 0;
   virtual bool doProcess_gcs() = 0;

   // Helpers to show process time duration.
   void doProcess_begin();
   void doProcess_end();

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
}//namespace
