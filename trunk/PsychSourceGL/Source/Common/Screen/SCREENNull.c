/*

  Psychtoolbox3/Source/Common/SCREENNull.c		

  AUTHORS:

  Allen.Ingling@nyu.edu		awi 

  PLATFORMS:	

  This file should build on any platform. 

  HISTORY:

  11/26/02  awi		Created. 

  DESCRIPTION:

  Arbitrary code to be executed for debugging/testing/hacking PTB. This subfunction is not
  documented or mentioned in official docs and only here for the purpose of internal testing
  and as a container for quick & dirty hacks and tests by the core developers.

  Therefore its syntax and semantics can change in any way, anytime without warning.

*/

#include "Screen.h"

#if PSYCH_SYSTEM == PSYCH_OSX
#include <IOKit/IOKitLib.h>
#endif

static char useString[] = "[[value1], [value2] ..]=SCREEN('Null',[value1],[value2],...);";
static char synopsisString[] = 
	"Special test function for Psychtoolbox testing and developments. Normal users: DO NOT USE!";
static char seeAlsoString[] = "";

PsychError SCREENNull(void) 
{
#define RADEON_D1CRTC_INTERRUPT_CONTROL 0x60DC

#define RADEON_R500_GEN_INT_CNTL   0x100
#define RADEON_R500_GEN_INT_STATUS 0x104
//#define RADEON_R500_GEN_INT_CNTL   0x040
//#define RADEON_R500_GEN_INT_STATUS 0x044
//#define RADEON_R500_GEN_INT_CNTL   0x200
//#define RADEON_R500_GEN_INT_STATUS 0x204

	const double defaultMatrix[] = {1.1, 1.2, 1.3, 1.4, 2.1, 2.2, 2.3, 2.4};
	const double defaultM=2, defaultN=4; 
	double tempValue; 
	double *array;
	int i, m,n, p, numInArgs, numOutArgs, numNamedOutArgs;
	char *str;
	PsychArgFormatType format;
	const char defaultString[] = "I am the default string\n";

	//all sub functions should have these two lines
	PsychPushHelp(useString, synopsisString, seeAlsoString);
	if(PsychIsGiveHelp()){PsychGiveHelp();return(PsychError_none);};

	#if PSYCH_SYSTEM == PSYCH_WINDOWS
		PsychWindowRecordType *windowRecord;
		PsychAllocInWindowRecordArg(1, TRUE, &windowRecord);
		PsychOSFlipWindowBuffers(windowRecord);
		return(PsychError_none);
	#endif

	#if PSYCH_SYSTEM == PSYCH_OSX
	char pbuf[1024];
	int residualLength = 1024;
	
	//WORKS:	io_registry_entry_t myentry = IORegistryEntryFromPath(kIOMasterPortDefault, kIOServicePlane ":/AppleACPIPlatformExpert/PCI0@0/AppleACPIPCI/PEGP@1/IOPCI2PCIBridge/display@0/ATY,Wormy@0/ATY_Wormy/display0/AppleBacklightDisplay");
	//WORKS:	io_registry_entry_t myentry = IORegistryEntryFromPath(kIOMasterPortDefault, kIOServicePlane ":/AppleACPIPlatformExpert/PCI0@0/AppleACPIPCI/PEGP@1/IOPCI2PCIBridge/display@0/ATY,Wormy@0/ATY_Wormy/display0");
	io_registry_entry_t myentry = IORegistryEntryFromPath(kIOMasterPortDefault, kIOServicePlane ":/AppleACPIPlatformExpert/PCI0@0/AppleACPIPCI/PEGP@1/IOPCI2PCIBridge/display@0");
	if (myentry == MACH_PORT_NULL) PsychErrorExitMsg(PsychError_system, "PTB-DEBUG: FAILED TO FIND IORegistryEntry!\n");
	printf("GOT IORegistryEntry! >> ");

	if (1) {
		CFDataRef myprop = IORegistryEntryCreateCFProperty(myentry, CFSTR("AAPL00,Dither"), kCFAllocatorDefault, 0);
		if (myprop == NULL) PsychErrorExitMsg(PsychError_system, "PTB-DEBUG: FAILED TO FIND AAPL00,Dither key!!\n");
		m = CFDataGetLength(myprop);
		printf("Amount of data stored in key 'AAPL00,Dither' is %i bytes.\nData: ", m);
		psych_uint8* mypropdata = CFDataGetMutableBytePtr(myprop);
		if (mypropdata == NULL) PsychErrorExitMsg(PsychError_system, "PTB-DEBUG: FAILED TO GET MUTABLE DATA PTR OF AAPL00,Dither key!!\n");
		for (i=0; i < m; i++) printf("%x ", mypropdata[i]);
		
		mypropdata[0] = 0x1;
		//	FAILS with kIOReturnUnsupported error:	printf("\n COMMIT! RC is %x [kIOReturnUnsupported = %x] \n", (int) IORegistryEntrySetCFProperty(myentry, CFSTR("AAPL00,Dither"), myprop), kIOReturnUnsupported);
		io_connect_t myconnect;
		if (0) {
			// FAILS with kIOReturnUnsupported error:
			if (kIOReturnSuccess == IOServiceOpen(CGDisplayIOServicePort(CGMainDisplayID()), mach_task_self(), 0, &myconnect)) {
				printf("\n COMMIT-IOSERVICE2! RC is %x [kIOReturnUnsupported = %x] \n", (int) IOConnectSetCFProperty(myconnect, CFSTR("AAPL00,Dither"), myprop), kIOReturnUnsupported);
				IOServiceClose(myconnect);
			}
			else {
				printf("\nNOCON\n");
			}
		}

		if (1) {
			kern_return_t	kernResult; 
			io_service_t	service;
			io_connect_t	connect;
			io_iterator_t 	iterator;
			CFDictionaryRef	classToMatch;
			unsigned int				i;
			
			// Setup matching criterion to find our driver in the IORegistry device tree:
//WORKS but useless for our purpose:			
classToMatch = IOServiceNameMatching("ATIRadeonX1000");
//WORKS but fails to open:			classToMatch = IOServiceNameMatching("ATY,Wormy");
//WORKS, IS WHAT WE NEED but fails to open:			classToMatch = IOServiceNameMatching("display");
			if (classToMatch == NULL) {
				printf("PTB-DEBUG: IOServiceMatching() for Psychtoolbox kernel support driver returned a NULL dictionary. Kernel driver support disabled.\n");
				PsychErrorExitMsg(PsychError_system, "FAIL!");
			}
			
			// This creates an io_iterator_t of all instances of our driver that exist in the I/O Registry. Each installed graphics card
			// will get its own instance of a driver. The iterator allows to iterate over all instances:
			kernResult = IOServiceGetMatchingServices(kIOMasterPortDefault, classToMatch, &iterator);
			if (kernResult != KERN_SUCCESS) {
				printf("PTB-DEBUG: IOServiceGetMatchingServices for Psychtoolbox kernel support driver returned 0x%08x. Kernel driver support disabled.\n", kernResult);
				PsychErrorExitMsg(PsychError_system, "FAIL!");
			}
			
			// In a polished final version we would want to handle the case where more than one gfx-card is attached.
			// The iterator would return multiple instances of our driver and we need to decide which one to connect to.
			// For now, we do not handle this case but instead just get the first item from the iterator.
			service = IOIteratorNext(iterator);
			
			// Release the io_iterator_t now that we're done with it.
			IOObjectRelease(iterator);
			
			if (service == IO_OBJECT_NULL) {
				printf("PTB-INFO: Couldn't find requested IOService.\n");
				PsychErrorExitMsg(PsychError_system, "FAIL!");
			}
			
			// Try in a loop to open with different connect flags i = 0 to 0xffff:
			for(i=0; i< 0xffff; i++) {
				kernResult = IOServiceOpen(service, mach_task_self(), i, &myconnect);
				if (kIOReturnSuccess == kernResult) {
					printf("\nCOOLIO!\n");
									printf("\n COMMIT-IOSERVICE3! RC is %x [kIOReturnUnsupported = %x] \n", (int) IOConnectSetCFProperty(myconnect, CFSTR("AAPL00,Dither"), myprop), kIOReturnUnsupported);
					IOServiceClose(myconnect);
					break;
				}
				else {
					if (kernResult != kIOReturnUnsupported) printf("\nNOCON5(%i): %x\n", i, kernResult);
				}
			}
			if (kIOReturnSuccess != kernResult) printf("PTB-INFO: Couldn't open requested IOService.\n");	
		}
		
		CFRelease(myprop);
	}
	
	if (0) {
		CFStringRef myprop = IORegistryEntryCreateCFProperty(myentry, CFSTR("name"), kCFAllocatorDefault, 0);
		CFStringGetCString(myprop, pbuf, 1024, kCFStringEncodingASCII);
		printf("Value of 'name' is %s .\n", (char*) pbuf);
		CFRelease(myprop);
		//	IORegistryEntrySetCFProperty(myentry, CFSTR("name"), CFSTR("funkydisplay"));
		//	IORegistryEntrySetCFProperty(r, CFSTR("IORequestIdle"), kCFBooleanTrue);
	}

	// Release registry entry:
	IOObjectRelease(myentry);

	#endif

	return(PsychError_none);
	
	#if PSYCH_SYSTEM == PSYCH_OSX
		printf("PTB-DEBUG: Shutting down connection to PsychtoolboxKernelDriver, if any...\n");
		PsychOSShutdownPsychtoolboxKernelDriverInterface();
	#endif
	
	#if PSYCH_SYSTEM == PSYCH_LINUX
		printf("PTB-DEBUG: PreRADEON_R500_GEN_INT_CNTL: %x\n", PsychOSKDReadRegister(0, 0x040, NULL));
		PsychOSKDWriteRegister(0, 0x040, PsychOSKDReadRegister(0, 0x040, NULL) | 0x1, NULL);
		PsychWaitIntervalSeconds(0.1);
		printf("PTB-DEBUG: PostRADEON_R500_GEN_INT_CNTL: %x\n", PsychOSKDReadRegister(0, 0x040, NULL));

		printf("PTB-DEBUG: RADEON_R500_GEN_INT_STATUS: %x\n", PsychOSKDReadRegister(0, 0x044, NULL));
		printf("PTB-DEBUG: RADEON_R500_GEN_INT_CNTL: %x\n", PsychOSKDReadRegister(0, RADEON_R500_GEN_INT_CNTL, NULL));
		printf("PTB-DEBUG: RADEON_R500_GEN_INT_STATUS: %x\n", PsychOSKDReadRegister(0, RADEON_R500_GEN_INT_STATUS, NULL));
		printf("PTB-DEBUG: RADEON_D1CRTC_INTERRUPT_CONTROL: %x\n", PsychOSKDReadRegister(0, RADEON_D1CRTC_INTERRUPT_CONTROL, NULL));
	#endif

	return(PsychError_none);

	//demonstrate how we find the function and subfunction names
	//printf("Psychtoolbox function: %s, subfunction %s\n", PsychGetModuleName(), PsychGetFunctionName() );

	//copy all the input argument to their outputs if we have doubles, if not error.  
	numInArgs = PsychGetNumInputArgs();
	numOutArgs = PsychGetNumOutputArgs();
	numNamedOutArgs = PsychGetNumNamedOutputArgs();
	PsychErrorExit(PsychCapNumOutputArgs(numInArgs));

	/*
	printf("number of input arguments: %d\n", numInArgs);
	printf("number of output arguments: %d\n", numOutArgs);
	printf("number of named output arguments: %d\n", numNamedOutArgs);
	*/

	//iterate over each of the supplied inputs.  If the input is a two-dimensional array 
	//of doubles or a character array, then copy it to the output.  
	for(i=1;i<=numInArgs;i++){

		format = PsychGetArgType(i);

		switch(format){

			case PsychArgType_double:

				if(PsychGetArgM(i)==1 && PsychGetArgN(i)==1){

					tempValue=i;  //if 1x1 double then the default return value is the arg position.

					PsychCopyInDoubleArg(i, FALSE, &tempValue);

					PsychCopyOutDoubleArg(i, FALSE, tempValue);

				}else{

					PsychAllocInDoubleMatArg(i, FALSE, &m, &n, &p, &array);

					PsychCopyOutDoubleMatArg(i, FALSE, m, n, p, array);

				}

				break;

			case PsychArgType_char:

				str=NULL; //This tells PsychGetCharArg() to use its own (volatile) memory. 

				PsychAllocInCharArg(i, FALSE, &str); 

				PsychCopyOutCharArg(i, FALSE, str);

				break;

			case PsychArgType_default:

				PsychCopyOutCharArg(i, FALSE, defaultString);

				break;

		}

	}

	return(PsychError_none);
}
