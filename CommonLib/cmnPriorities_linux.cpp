/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

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
   mTsPrint           = Ris::Threads::Priority(-1, 20);
   mSerialString      = Ris::Threads::Priority(-1, 80);
   mCommLong          = Ris::Threads::Priority(-1, 60);
   mCommShort         = Ris::Threads::Priority(-1, 70);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
