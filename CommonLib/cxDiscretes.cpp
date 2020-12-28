/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#define  _CXDISCRETES_CPP_
#include "cxDiscretes.h"

namespace CX
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor

Discretes::Discretes()
   :   mAuxAlarm0In(66),   // p8.07
       mAuxAlarm1In(67),   // p8.08
       mAuxAlarm2In(69),   // p8.09
       mAuxAlarm3In(68),   // p8.10
       mAuxAlarm4In(45),   // p8.11
       mModeLed(47),       // p8.15
       mStatusLed(46),     // p8.16
       mBuzzer(51),        // p9.16
       mTTA0Out(27),       // p8.17
       mTTA1Out(65)        // p8.18
{
   initialize();
}

void Discretes::initialize()
{
   // Store pointers.
   mAuxAlarmIn[0] = &mAuxAlarm0In;
   mAuxAlarmIn[1] = &mAuxAlarm1In;
   mAuxAlarmIn[2] = &mAuxAlarm2In;
   mAuxAlarmIn[3] = &mAuxAlarm3In;
   mAuxAlarmIn[4] = &mAuxAlarm4In;

   // Initialize the gpio.
   mAuxAlarm0In.streamOpen();
   mAuxAlarm0In.setDirection(INPUT);

   mAuxAlarm1In.streamOpen();
   mAuxAlarm1In.setDirection(INPUT);
      
   mAuxAlarm2In.streamOpen();
   mAuxAlarm2In.setDirection(INPUT);

   mAuxAlarm3In.streamOpen();
   mAuxAlarm3In.setDirection(INPUT);

   mAuxAlarm4In.streamOpen();
   mAuxAlarm4In.setDirection(INPUT);

   mModeLed.streamOpen();
   mModeLed.setDirection(OUTPUT);
      
   mStatusLed.streamOpen();
   mStatusLed.setDirection(OUTPUT);

   mBuzzer.streamOpen();
   mBuzzer.setDirection(OUTPUT);

   mTTA0Out.streamOpen();
   mTTA0Out.setDirection(OUTPUT);

   mTTA1Out.streamOpen();
   mTTA1Out.setDirection(OUTPUT);
}

void Discretes::finalize()
{
   mAuxAlarm0In.streamClose();
   mAuxAlarm1In.streamClose();
   mAuxAlarm2In.streamClose();
   mAuxAlarm3In.streamClose();
   mAuxAlarm4In.streamClose();
   mModeLed.streamClose();
   mStatusLed.streamClose();
   mBuzzer.streamClose();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
