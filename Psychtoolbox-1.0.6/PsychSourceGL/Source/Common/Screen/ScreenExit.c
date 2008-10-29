/*
	PsychToolbox2/Source/Common/Screen/ScreenExit.cpp		

	AUTHORS:
	
		Allen.Ingling@nyu.edu		awi 

	PLATFORMS:	
	
		Only OS X fow now.  


	HISTORY:
	
		12/20/01	awi		Created.
		1/25/04		awi		Added update provided by mk. It makes the ScreenCloseAllWindows call.  
                7/22/05         mk              Added call to CloseWindowBank() to free dynamic window bank array.
	DESCRIPTION:
	
		ScreenExitFunction is called before the Screen module is flushed.

	T0 DO:
		
		


*/

#include "Screen.h"

PsychError ScreenExitFunction(void)
{

	//The timing array holds time values set by Screen internal diagnostics.  It allocates memory with 
	//malloc to hold the array of times.  This call frees the memory prior to unloading Screen
	ClearTimingArray();
  
	// Close all open onscreen windows and release their resources,
	// -> Perform exactly the same cleanup that Screen('CloseAll') would do.
	ScreenCloseAllWindows();
	CloseWindowBank();
	return(PsychError_none);
}



