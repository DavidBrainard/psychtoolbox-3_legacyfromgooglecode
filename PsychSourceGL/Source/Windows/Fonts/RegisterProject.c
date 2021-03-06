/*
  PsychToolbox2/Source/OSX/Fonts/RegisterProject.c		
  
  PROJECTS: Screen only.  
  
  AUTHORS:
  Allen.Ingling@nyu.edu		awi 
  
  PLATFORMS:	Mac OS X
    

  HISTORY:
  10/23/03  	awi		Created. 
 
  
  DESCRIPTION: 
  
  NOTES:
  
  Fonts uses PsychFontGlue to enumerate fonts and store information in a native font list.  However, PsychFontGlue does not share the same
  list between modules.  So the font list maintained by Screen and that maintained by Fonts will in fact be different lists, though 
  they should be identical lists and immutable.  This is wasteful but it should work until we consolidate the two by sharing
  memory between Psychtoolbox modules.
  
  */ 

#include "RegisterProject.h"


// PsychModuleInit is in Screen.cpp
// PsychProjectExit is in Screen.cpp
// PsychProjectSelectFunction is in Screen.cpp


PsychError PsychModuleInit(void)
{
	//register the project exit function
	PsychErrorExitMsg(PsychRegisterExit(&FontsExitFunction), NULL); 
	
	//register the project function which is called when the module
	//is invoked with no arguments:
	PsychErrorExitMsg(PsychRegister(NULL,  &PsychDisplayATSSynopsis), NULL);

	//register the module name
	PsychErrorExitMsg(PsychRegister("Fonts", NULL),NULL);
	
	//register named subfunctions
	PsychErrorExit(PsychRegister("Fonts",  &FONTSFonts));
	PsychErrorExit(PsychRegister("NumFonts",  &FONTSNumFonts));
	PsychErrorExit(PsychRegister("Version",  &MODULEVersion));
        
	InitializeSynopsis();
	PsychSetModuleAuthorByInitials("awi");
        
	return(PsychError_none);

}




