/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <time.h>

#include "risPortableCalls.h"
#include "my_functions.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return a string.

std::string my_string_from_int(int aX)
{
   char tBuffer[20];
   sprintf(tBuffer, "%d", aX);
   return std::string(tBuffer);
}

std::string my_string_from_double(double aX, int aPrecision)
{
   char tBuffer[40];

   if (aPrecision == 1)
   {
      sprintf(tBuffer, "%.1f", aX);
   }
   else if (aPrecision == 2)
   {
      sprintf(tBuffer, "%.2f", aX);
   }
   else if (aPrecision == 3)
   {
      sprintf(tBuffer, "%.3f", aX);
   }
   else if (aPrecision == 4)
   {
      sprintf(tBuffer, "%.4f", aX);
   }
   else if (aPrecision == 5)
   {
      sprintf(tBuffer, "%.5f", aX);
   }

   return std::string(tBuffer);
}
