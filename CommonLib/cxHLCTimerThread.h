#pragma once

/*==============================================================================
Main timer thread class.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risThreadsTimerThread.h"
#include "dspFilterAlpha.h"
#include "HLC.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace CX
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is a timer thread that polls the shared memory and sends events
// to the event log thread.

class HLCTimerThread : public Ris::Threads::BaseTimerThread
{
public:

   typedef Ris::Threads::BaseTimerThread BaseClass;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // If true then this is the first periodic update.
   bool mFirstFlag;

   // Times of arrival for rising edge and falling edge of pulse detetcions.
   double mTime1;
   double mTime2;
   double mDuration;

   // HLC input. Sensor or override.
   double mRawInput;

   // HLC filtered input.
   double mInput;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // HLC aprogram driver.
   TXRX::HLC mHLC;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   Dsp::Filter::AlphaOne mAlphaFilter;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   HLCTimerThread();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. Thread base class overloads.

   // Thread init function. This is called by the base class immediately 
   // after the thread starts running.
   void threadInitFunction() override;

   // Thread timer execution. This is called periodically.
   void executeOnTimer(int aTimeCount) override;

   // Thread exit function. This is called by the base class immediately
   // before the thread is terminated.
   void threadExitFunction() override;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. 


   // Read the input. Sensor or override.
   void doRead(int aTimeCount);

   // Process the input.
   void doProcess(int aTimeCount);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global singular instance.

#ifdef _CXHLCTIMERTHREAD_CPP_
         HLCTimerThread* gHLCTimerThread = 0;
#else
extern   HLCTimerThread* gHLCTimerThread;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
