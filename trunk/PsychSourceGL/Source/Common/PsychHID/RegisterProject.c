/*
  PsychToolbox3/Source/Common/PsychHID/RegisterProject.c		
  
  PROJECTS: PsychHID
  
  PLATFORMS:  OSX
  
  AUTHORS:
  Allen.Ingling@nyu.edu		awi 
      
  HISTORY:
  4/16/03  awi		Created. 
  4/15/05  dgp		Added Get/SetReport. 
  8/23/07  rpw      Added PsychHIDKbQueue suite and PsychHIDKbTriggerWait

*/

#include "Psych.h"
#include "PsychHID.h"

PsychError PsychModuleInit(void)
{
	InitializeSynopsis();  //first initialize the list of synopsis strings.

	//register the project function which is called when the module
	//is invoked with no arguments:
	PsychRegister(NULL,  &PsychDisplayPsychHIDSynopsis);

	//register the module name
	PsychErrorExit(PsychRegister("PsychHID", NULL));

	//register the module exit function
	PsychRegisterExit(&PsychHIDCleanup);

	//register module subfunctions
	PsychErrorExit(PsychRegister("Version",  &MODULEVersion));
	PsychErrorExit(PsychRegister("NumDevices",  &PSYCHHIDGetNumDevices));
	PsychErrorExit(PsychRegister("NumElements",  &PSYCHHIDGetNumElements));
	PsychErrorExit(PsychRegister("Devices",  &PSYCHHIDGetDevices));
	PsychErrorExit(PsychRegister("Elements",  &PSYCHHIDGetElements));
	PsychErrorExit(PsychRegister("RawState",  &PSYCHHIDGetRawState));
	PsychErrorExit(PsychRegister("NumCollections",  &PSYCHHIDGetNumCollections));
	PsychErrorExit(PsychRegister("Collections",  &PSYCHHIDGetCollections));
	PsychErrorExit(PsychRegister("CalibratedState",  &PSYCHHIDGetCalibratedState));
	PsychErrorExit(PsychRegister("KbCheck",  &PSYCHHIDKbCheck));
	PsychErrorExit(PsychRegister("KbWait",  &PSYCHHIDKbWait));
	PsychErrorExit(PsychRegister("KbTriggerWait", &PSYCHHIDKbTriggerWait));
	PsychErrorExit(PsychRegister("KbQueueCreate", &PSYCHHIDKbQueueCreate));
	PsychErrorExit(PsychRegister("KbQueueStart", &PSYCHHIDKbQueueStart));
	PsychErrorExit(PsychRegister("KbQueueStop", &PSYCHHIDKbQueueStop));
	PsychErrorExit(PsychRegister("KbQueueCheck", &PSYCHHIDKbQueueCheck));
	PsychErrorExit(PsychRegister("KbQueueFlush", &PSYCHHIDKbQueueFlush));
	PsychErrorExit(PsychRegister("KbQueueRelease", &PSYCHHIDKbQueueRelease));
	PsychErrorExit(PsychRegister("GetReport",  &PSYCHHIDGetReport));
	PsychErrorExit(PsychRegister("ReceiveReports",  &PSYCHHIDReceiveReports));
	PsychErrorExit(PsychRegister("ReceiveReportsStop",  &PSYCHHIDReceiveReportsStop));
	PsychErrorExit(PsychRegister("GiveMeReports",  &PSYCHHIDGiveMeReports));
	PsychErrorExit(PsychRegister("SetReport",  &PSYCHHIDSetReport));
	PsychErrorExit(PsychRegister("ColorCal2", &PSYCHHIDColorCal2));

	PsychSetModuleAuthorByInitials("awi");
	PsychSetModuleAuthorByInitials("dgp");
	PsychSetModuleAuthorByInitials("mk");
	PsychSetModuleAuthorByInitials("rpw");
	PsychSetModuleAuthorByInitials("cgb");

	return(PsychError_none);
}