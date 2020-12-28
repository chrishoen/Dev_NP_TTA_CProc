#pragma once

/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "SuperStateTTA.h"
#include "SuperStateDA.h"
#include "SuperStateCU.h"

namespace CX
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This class implements evaluation of superstates. It does calculations
// based on superstate values and generates corresponding events.

class SuperStateEvaluator
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Constants:

   // Guard thresholds.
   static constexpr float cAD_Guard_ThreshLo = 0.02;

   // TTA thresholds.
   static constexpr float cTTA_Temperature_ThreshHi = 40.0;
   static constexpr float cTTA_MainVoltage_ThreshLo    = 7.0;
   static constexpr float cTTA_MainCurrent_ThreshLo    = 0.30;
   static constexpr float cTTA_LineResistance_ThreshHi = 4.0;

   // DA thresholds.
   static constexpr float cDA_Temperature_ThreshHi = 40.0;
   static constexpr float cDA_MainVoltage_ThreshLo = 7.0;
   static constexpr float cDA_MainCurrent_ThreshLo = 0.30;
   static constexpr float cDA_TowerVoltage_ThreshLo = 7.0;
   static constexpr float cDA_TowerCurrent_ThreshLo = 0.30;


   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members:

   // Copies of received superstates.
   SuperStateTTA mSuperStateTTA;
   SuperStateDA  mSuperStateDA;

   // Copies of previous received superstates.
   SuperStateTTA mLastSuperStateTTA;
   SuperStateDA  mLastSuperStateDA;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members:

   // Valid flags.
   bool mValidFlagTTA;
   bool mValidFlagDA;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods:

   // Constructor.
   SuperStateEvaluator();
   void reset();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Evaluate superstates. The superstates are obtained from shared memory.
   void doEvaluateTTA();
   void doEvaluateDA();
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global instance

#ifdef _CXSUPERSTATEEVALUATOR_CPP_
          SuperStateEvaluator gSuperStateEvaluator;
#else
   extern SuperStateEvaluator gSuperStateEvaluator;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

