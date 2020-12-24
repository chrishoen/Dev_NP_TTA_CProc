/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <windows.h>

#define  _CMNPRIORITIES_CPP_
#include "cmnPriorities.h"

namespace Cmn
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

Priorities::Priorities()
{
   mTsPrint         = Ris::Threads::Priority(-1, THREAD_PRIORITY_ABOVE_NORMAL);
   mLogFile         = Ris::Threads::Priority(-1, THREAD_PRIORITY_LOWEST);
   mCmdComm         = Ris::Threads::Priority(-1, THREAD_PRIORITY_NORMAL);
   mSerialString    = Ris::Threads::Priority(-1, THREAD_PRIORITY_NORMAL);
   mTestSeqLong     = Ris::Threads::Priority(-1, THREAD_PRIORITY_HIGHEST);
   mTestSeqShort    = Ris::Threads::Priority(-1, THREAD_PRIORITY_NORMAL);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
