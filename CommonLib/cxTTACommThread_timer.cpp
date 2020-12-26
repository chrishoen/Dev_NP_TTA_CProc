/*==============================================================================
Detestion:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "cxCProcParms.h"

#include "cxTTACommThread.h"

namespace CX
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is called periodically by the base class threadRunFunction.

void TTACommThread::executeOnTimer(int aTimerCount)
{
   // Guard.
   if (!mConnectionFlag) return;

   doProcess_gcs(aTimerCount);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace