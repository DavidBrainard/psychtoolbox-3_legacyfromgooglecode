/*
  Psychtoolbox3/Source/Common/SCREENOpenVideoCapture.c		
  
  AUTHORS:
  
  mario.kleiner at tuebingen.mpg.de   mk
  
  PLATFORMS:	
  
  This file should build on any platform. 

  HISTORY:
  
  2/7/06	mk		Created. 
  12/23/07	mk		Extended to allow switching between capture engines on a per-device basis.
 
  DESCRIPTION:
  
  Open a video capture source (framegrabber, webcam, ...), create and initialize a corresponding capture object
  and return a handle to it to MATLAB space.
 
  On OS-X and Windows, video capture is implemented via Apples Sequence-Grabber API which is part of Quicktime.
  On all operating systems, we support Firewire machine vision cameras that comply to the IIDC-1.x specification,
  via use of the open-source free software library libdc1394 V2. The library itself is most powerful and well
  tested/tuned for GNU/Linux systems, but also well working on OS/X. It has experimental limited support for
  MS-Windows as well, but its useability needs to be seen.
  
  TO DO:
  
*/

#include "Screen.h"

static char useString[] = "videoPtr = Screen('OpenVideoCapture', windowPtr [, deviceIndex] [,roirectangle] [, pixeldepth] [, numbuffers] [, allowfallback] [, targetmoviename] [, recordingflags] [, captureEngineType]);";
static char synopsisString[] = 
"Try to open the video source 'deviceIndex' for video capture into onscreen window 'windowPtr' and "
"return a handle 'videoPtr' on success. If 'deviceIndex' is left out, it defaults to zero - use the "
"first capture device attached to your machine. 'roirectangle' if specified, defines the requested size "
"for captured images. The default is to return the maximum size image provided by the capture device. "
"The real ROI (region of interest) may differ from the requested one, depending on the capabilities of "
"your capture device. 'pixeldepth' if provided, asks for the number of layers that captured textures "
"should have: 1=Luminance image, 2=Luminance+Alpha image, 3=RGB image, 4=RGB+Alpha image. Default is "
"to take whatever the capture device provides by default. Different devices support different formats "
"so some of these settings may be ignored. 'numbuffers' if provided, specifies the number of internal "
"video buffers to use. It defaults to a value that is optimal for your specific hardware for common use. "
"'allowfallback' if set to 1, will allow Psychtoolbox to use a less efficient mode of operation for video "
"capture if your specific hardware or operating system setup doesn't allow to use the high-performance "
"mode. 'allowfallback' defaults to 1 = Allow fallback path.\n"
"'targetmoviename' If you provide a filename for this argument, PTB will record the captured video to the "
"specified Quicktime movie file on your filesystem. PTB will use a default video codec for encoding the "
"video stream. If you want it to use a specific codec, you can extend the targetmoviename by a string of "
"format :CodecType=xxx , where xxx is the numeric type id of the codec. You can get a list of codecs "
"supported by your system by running PTB's recording engine at a verbosity level of 4 or higher.\n"
"'recordingflags' specify the behaviour of harddisc- "
"recording: 0 (default) = Only record video. 2 = Record audio track as well. The value 1 (or 1+2) asks "
"PTB to first record into system memory, and only write the movie file after capture has been stopped. "
"This allows for higher capture framerates, but is limited in recording time by installed memory. Also, "
"this mode currently can cause hangs and crashes of PTB for unknown reasons, so better avoid!\n"
"\n"
"'captureEngineType' This optional parameter allows selection of the video capture engine to use for this "
"video source. Allowable values are currently 0 and 1. Zero selects Apples Quicktime Sequence-Grabber API "
"as capture engine, which is supported on MacOS/X and MS-Windows (for Windows you'll need to install a "
"Quicktime Video digitizer component VDIG). The Quicktime engine allows movie recording and sound recording "
"as well (see above). A value of 1 selects Firewire video capture via the free software library libdc1394-V2. "
"That engine only supports high performance machine vision cameras that are compliant with the "
"IIDC-1.x standard and that are connected via a Firewire (IEEE1394) bus system. Use of the engine with such "
"cams allows for much higher flexibility and performance than use of video capture via Quicktime, "
"however, video recording to harddisk or sound recording isn't yet supported with firewire capture, ie., "
"the 'targetmoviename' is simply ignored. The firewire capture engine is supported on Linux, MacOS/X and "
"- with quite a few limitations and bugs - on Windows. If you don't specify 'captureEngineType', the global "
"setting from Screen('Preference', 'DefaultVideoCaptureEngine') will be used. If you don't specify that either "
"then engine selection will default to Quicktime for MacOS/X and MS-Windows, and Firewire libdc1394 on Linux.\n\n"
"To summarize: \n"
"Quicktime engine: Supports all cameras/video sources supported by your operating system, allows for video- and "
"audio recording as well. On many setups, only one camera can be used at a time and the 'deviceIndex' parameter "
"is ignored -- the default source is always chosen. Latency, max framerate and reliability is ok, but not stellar.\n"
"Firewire engine: Supports only Firewire machine vision cameras, but allows free selection among all connected "
"cameras, simultaneous operation of many cameras, low latency, high framerates and reliability, precise timestamping "
"and low level access to many special features of such cameras, e.g., gain-, shutter-, exposure-, trigger controls etc.\n";

static char seeAlsoString[] = "CloseVideoCapture StartVideoCapture StopVideoCapture GetCapturedImage";

PsychError SCREENOpenVideoCapture(void) 
{
	PsychWindowRecordType					*windowRecord;
	int                                     deviceIndex;
	int                                     capturehandle = -1;
	double                                  framerate;
	int                                     width;
	int                                     height;
	PsychRectType                           roirectangle;
	Boolean                                 roiassigned;
	int                                     reqdepth = 0;
	int                                     num_dmabuffers = 0;
	int                                     allow_lowperf_fallback = 1;
	char*									moviename;
	int										recordingflags;
	int										engineId;
	
	// All sub functions should have these two lines
	PsychPushHelp(useString, synopsisString, seeAlsoString);
	if(PsychIsGiveHelp()) {PsychGiveHelp(); return(PsychError_none);};

	PsychErrorExit(PsychCapNumInputArgs(9));            // Max. 9 input args.
	PsychErrorExit(PsychRequireNumInputArgs(1));        // Min. 1 input args required.
	PsychErrorExit(PsychCapNumOutputArgs(1));           // Max. 1 output args.
	
	// Get the window record from the window record argument and get info from the window record
	PsychAllocInWindowRecordArg(kPsychUseDefaultArgPosition, TRUE, &windowRecord);
	// Only onscreen windows allowed:
	if(!PsychIsOnscreenWindow(windowRecord)) {
		PsychErrorExitMsg(PsychError_user, "OpenVideoCapture called on something else than an onscreen window.");
	}
	
	// Get the device index. We default to the first device if none is given:
	deviceIndex=0;
	PsychCopyInIntegerArg(2, FALSE, &deviceIndex);
	
	// Get the optional roi rectangle:
	roiassigned = PsychCopyInRectArg(3, FALSE, roirectangle);
	if (IsPsychRectEmpty(roirectangle)) PsychErrorExitMsg(PsychError_user, "OpenVideoCapture called with invalid (empty) roirectangle argument.");
	
	
	// Query (optional) output texture pixel depth: By default, we take whatever we get from the capture device:
	PsychCopyInIntegerArg(4, FALSE, &reqdepth);
	if (reqdepth<0) PsychErrorExitMsg(PsychError_user, "OpenVideoCapture called with invalid (negative) pixeldepth argument.");
	// Query number of ringbuffers to use. Our default is coded in the OS dependent subroutines.
	PsychCopyInIntegerArg(5, FALSE, &num_dmabuffers);
	if (num_dmabuffers<0) PsychErrorExitMsg(PsychError_user, "OpenVideoCapture called with invalid (negative) numbuffers argument.");
	// Query, if use of low-performance fallback code is allowed if high-perf engine fails:
	PsychCopyInIntegerArg(6, FALSE, &allow_lowperf_fallback);
	if (allow_lowperf_fallback<0) PsychErrorExitMsg(PsychError_user, "OpenVideoCapture called with invalid (negative) allowfallback flag.");
	
	// Query optional moviename for recording the grabbed video into a Quicktime movie file:
	moviename = NULL;
	PsychAllocInCharArg(7, FALSE, &moviename);
	
	// Query optional movie recording flags:
	// 0 = Record video, stream to disk immediately (slower, but unlimited recording duration).
	// 1 = Record video, stream to memory, then at end of recording to disk (limited duration by RAM size, but faster).
	// 2 = Record audio as well.
	recordingflags = 0;
	PsychCopyInIntegerArg(8, FALSE, &recordingflags);
	
	// Copy in optional id of video capture engine to use. We default to the one set via the Screen('Preference', 'DefaultVideocaptureEngine');
	// setting, which by itself defaults to LibDC1394 (type 1) on Linux, and Quicktime/SG (type 0) on all other OSs.
	engineId = PsychPrefStateGet_VideoCaptureEngine();
	PsychCopyInIntegerArg(9, FALSE, &engineId);
	if (engineId<0 || engineId>1)  PsychErrorExitMsg(PsychError_user, "OpenVideoCapture called with invalid 'captureEngineType' argument. Valid are zero and one.");

	// Try to open the capture device and create & initialize a corresponding capture object.
	// A MATLAB handle to the video capture object is returned upon successfull operation.
	if (roiassigned) {
		PsychOpenVideoCaptureDevice(engineId, windowRecord, deviceIndex, &capturehandle, roirectangle, reqdepth, num_dmabuffers, allow_lowperf_fallback, moviename, recordingflags);
	}
	else {
		PsychOpenVideoCaptureDevice(engineId, windowRecord, deviceIndex, &capturehandle, NULL, reqdepth, num_dmabuffers, allow_lowperf_fallback, moviename, recordingflags);
	}
	
	// Upon sucessfull completion, we'll have a valid handle in 'capturehandle'. Return it to Matlab-world:
	PsychCopyOutDoubleArg(1, TRUE, (double) capturehandle);
	
	// Ready!
	return(PsychError_none);
}

