#pragma once

/*==============================================================================
Main timer thread class.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risThreadsTimerThread.h"

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

class MainTimerThread : public Ris::Threads::BaseTimerThread
{
public:

   typedef Ris::Threads::BaseTimerThread BaseClass;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // If true then this is the first periodic update.
   bool mFirstFlag;

   // Last alarm enable from the superwants cu.
   bool mLastAlarmEnable;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   MainTimerThread();

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


   // Initialize the event table for aux alarms.
   void initialize_AuxAlarms();

   // Thread timer execution. This is called periodically.
   void executeOnTimer_Events(int aTimeCount);
   void executeOnTimer_AuxAlarms(int aTimeCount);

};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global singular instance.

#ifdef _CXMAINTIMERTHREAD_CPP_
         MainTimerThread* gMainTimerThread = 0;
#else
extern   MainTimerThread* gMainTimerThread;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
