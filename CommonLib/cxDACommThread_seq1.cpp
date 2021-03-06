/*==============================================================================
Detestion:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "cmnPriorities.h"
#include "cxCProcParms.h"
#include "cxStatus.h"
#include "sxMsgDefs.h"

#include "cxDACommThread.h"

namespace CX
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Execute thread specific code at the beginning of seq1.

void DACommThread::doSeq1Init()
{
   // TTA reboot state.
   // 0 : no reboot. this is set when the da comm thread seq1 starts.
   gStatus.mTTARebootState = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Execute thread specific code at the beginning of seq1.

void DACommThread::doSeq1Exit()
{
}


//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace