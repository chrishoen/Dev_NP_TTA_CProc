#pragma once

/*==============================================================================
cproc global status variables
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace CX
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This class provides gpio discrete i/o.

class Status
{
public:
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members:

   // TTA reboot state.
   // 0 : no reboot. this is set when the da comm thread seq1 starts.
   // 1 : the da  comm thread intercepts a superwants tta reboot code and
   //     resets it. it sends an abort qcall to the tta comm thread.
   // 2 : the tta comm thread sets this when it is aborted.
   // 3 : the da  comm thread sets the superwants tta reboot code.
   // 4 : the da  comm thread gets a superstate message. the da reboot
   //     relay should be set.
   // 5 : the da  comm thread gets a superstate message. the da reboot
   //     relay should be not set. the tta should reboot.
   // 6 : the da  comm thread gets a superstate message. waits.
   // 7 : the da  comm thread gets a superstate message. waits.
   // 8 : the da  comm thread sends an seq1 qcall to the tta comm thread.
   // 0 : the tta comm thread sets this when it receives a tst messags.

   int mTTARebootState;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods:

   // Constructor.
   Status();
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global instance

#ifdef _CXSTATUS_CPP_
          Status gStatus;
#else
   extern Status gStatus;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

