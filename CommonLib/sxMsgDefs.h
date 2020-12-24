#pragma once

/*==============================================================================
This file provides a set of constants that are use throughout the event
processing code. It also provides some common helper functions.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <time.h>

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Evt
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// File paths.

static const char* cLogFilePath = "/var/log/np_tta_event.log";
static const char* cAlarmFilePath = "/var/log/np_tta_alarm.txt";

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Msg types. Msgs are defined as one of two types.

// A type one event is a momentary event that does not have a boolean state
// variable associated with it. An example is "the batter hit the ball". It
// is analogous to an impulse or a dirac delta function. A type one event
// can only be set and has a duration of zero.
static const int cEvt_Type1 = 1;

// A type two event is a momentary event that has a boolean state variable
// associated with it. An example is "Bob drank a cup of coffee". It
// is analogous to a pulse. The pulse involves two events: a rising edge and
// a falling edge. A type two event can be set and then cleared. The event is
// set when Bob starts drinking the coffee and is cleared when he is finished.
// The duration can be considered as a pulse width.
static const int cEvt_Type2 = 2;

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Msg level. Associated with each event is a level that defines 
// alarm status. Alarm and Critical are used to classify active alarms.

static const int cEvt_SeverityInfo       = 0;
static const int cEvt_SeverityWarning    = 1;
static const int cEvt_SeveritySevere     = 2;
static const int cEvt_SeverityCritical   = 3;
static const int cEvt_SeverityUseDefault = 4;

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Msg source. Associated with each event is a code that defines the
// source of the event.

static const int cEvt_SourceTest = 0;
static const int cEvt_SourceTTA  = 1;
static const int cEvt_SourceDA   = 2;
static const int cEvt_SourceCU   = 3;
static const int cEvt_SourceHLC  = 4;
static const int cEvt_SourceAux  = 5;

//******************************************************************************
//******************************************************************************
//******************************************************************************
// More.

// Maximum number of event table indices. This defines the size of the event
// table.
static const int cEvt_MaxNumIndex = 100;

// Maximum number of event identifiers. The identifiers are mapped to the
// indices.
static const int cEvt_MaxNumIdent = 1000;

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Msg identifier. This is unique to each event. The identifiers are mapped
// into the event table via the index from identifier mapping array.
// The meaning of the events is not given here. They can be derived from
// inspecting the event table.

// Msg identifiers.
static const int cEvt_Ident_Test0              = 900;
static const int cEvt_Ident_Test1              = 901;
static const int cEvt_Ident_Test2              = 902;
static const int cEvt_Ident_Test3              = 903;

static const int cEvt_Ident_ClearInactiveAlarms = 910;

static const int cEvt_Ident_TTA_CommLost       = 100;
static const int cEvt_Ident_TTA_CommError      = 101;
static const int cEvt_Ident_TTA_Temperature    = 102;
static const int cEvt_Ident_TTA_MainVoltage    = 103;
static const int cEvt_Ident_TTA_MainCurrent    = 104;
static const int cEvt_Ident_TTA_LineResistance = 105;
static const int cEvt_Ident_TTA_AmpACurrent    = 106;
static const int cEvt_Ident_TTA_AmpBCurrent    = 107;
static const int cEvt_Ident_TTA_OpMode         = 108;
static const int cEvt_Ident_TTA_RFPath         = 109;
static const int cEvt_Ident_TTA_PreferRFPath   = 110;

static const int cEvt_Ident_DA_CommLost        = 200;
static const int cEvt_Ident_DA_CommError       = 201;
static const int cEvt_Ident_DA_Temperature     = 202;
static const int cEvt_Ident_DA_MainVoltage     = 203;
static const int cEvt_Ident_DA_MainCurrent     = 204;
static const int cEvt_Ident_DA_TowerVoltage    = 205;
static const int cEvt_Ident_DA_TowerCurrent    = 206;
static const int cEvt_Ident_DA_AmpCurrent      = 207;
static const int cEvt_Ident_DA_OpMode          = 208;
static const int cEvt_Ident_DA_RFPath          = 209;
static const int cEvt_Ident_DA_UserAtten       = 210;

static const int cEvt_Ident_CU_CProcRestart    = 300;

static const int cEvt_Ident_HLC_AlarmEnable    = 400;
static const int cEvt_Ident_HLC_Detect         = 401;

static const int cEvt_Ident_Aux_AuxInput0      = 500;
static const int cEvt_Ident_Aux_AuxInput1      = 501;
static const int cEvt_Ident_Aux_AuxInput2      = 502;
static const int cEvt_Ident_Aux_AuxInput3      = 503;
static const int cEvt_Ident_Aux_AuxInput4      = 504;

static const int cEvt_Ident_Aux_AuxInput[5] = 
{ 
   cEvt_Ident_Aux_AuxInput0,
   cEvt_Ident_Aux_AuxInput1,
   cEvt_Ident_Aux_AuxInput2,
   cEvt_Ident_Aux_AuxInput3,
   cEvt_Ident_Aux_AuxInput4,
};



//******************************************************************************
//******************************************************************************
//******************************************************************************
// More.

// Max table record or record argument size.
static const int cMaxRecordArgSize = 40;

// Max table record or record argument size.
static const int cMaxShowStringSize = 200;

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Helpers.

// Get a constant as a string.
char* get_EvtType_asString(int aType);
char* get_EvtSeverity_asString(int aSeverity);
char* get_EvtSource_asString(int aSource);

// Get a timestamp as a string.
char* get_timespec_asString(timespec aTimeSpec, char* aBuffer);

// Return true if the event can cause an alarm.
bool isAlarmable(int tSeverity);

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

