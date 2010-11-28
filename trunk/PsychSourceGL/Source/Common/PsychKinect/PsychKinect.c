/*
	PsychToolbox3/Source/Common/PsychKinect/PsychKinect.h

	PROJECTS: PsychKinect only.

	AUTHORS:

	mario.kleiner@tuebingen.mpg.de	mk

	PLATFORMS:	All.

	HISTORY:

	24.11.2010  mk		Created.

	DESCRIPTION:

	A Psychtoolbox driver for Microsoft's Kinect 3D-camera, based
	on the free software code of the http://openkinect.org project.

*/

#include "PsychKinect.h"

#include <stdio.h>
#include <string.h>
#include <math.h>

// Just simulate the beast:
// #define EMULATE_LIBENECT 1

#ifdef EMULATE_LIBENECT

	// Simulation: Fake behaviour of libusb and libfreeenect:
	#define FREENECT_FRAME_W 640
	#define FREENECT_FRAME_H 480
	#define FREENECT_FRAME_PIX (FREENECT_FRAME_H*FREENECT_FRAME_W)
	#define FREENECT_RGB_SIZE (FREENECT_FRAME_PIX*3)
	#define FREENECT_BAYER_SIZE (FREENECT_FRAME_PIX)
	#define FREENECT_DEPTH_SIZE (FREENECT_FRAME_PIX*sizeof(freenect_depth))
	#define FREENECT_COUNTS_PER_G 819

	typedef enum {
		FREENECT_FORMAT_RGB = 0,
		FREENECT_FORMAT_BAYER = 1,
	} freenect_rgb_format;

	typedef enum {
		FREENECT_FORMAT_11_BIT = 0,
		FREENECT_FORMAT_10_BIT = 1,
		FREENECT_FORMAT_PACKED_11_BIT = 2,
		FREENECT_FORMAT_PACKED_10_BIT = 3,
	} freenect_depth_format;

	typedef void* freenect_device;
	typedef freenect_device* freenect_context;
	typedef void* freenect_usb_context;
	typedef unsigned char freenect_pixel;

	typedef void (*freenect_depth_cb)(freenect_device *dev, void *depth, unsigned int timestamp);
	typedef void (*freenect_rgb_cb)(freenect_device *dev, freenect_pixel *rgb, unsigned int timestamp);

	static freenect_depth_cb depth_cb;
	static freenect_rgb_cb rgb_cb;

	static usbevent_counter = 0;
	freenect_context devil;
	
	unsigned short depth_frame[640*480];
	unsigned char rgb_frame[640*480*3];

	int freenect_init(freenect_context **ctx, freenect_usb_context *usb_ctx) { *ctx = &devil; return(1); };
	int freenect_shutdown(freenect_context *ctx) { return(1); }

	int freenect_open_device(freenect_context *ctx, freenect_device **dev, int index) { usbevent_counter=0; *dev = (freenect_device*) malloc(sizeof(void*)); *ctx = *dev; return(1); };
	int freenect_close_device(freenect_device *dev) { usbevent_counter = 0 ; return(1); }

	void freenect_set_depth_callback(freenect_device *dev, freenect_depth_cb cb) { depth_cb = cb; };
	void freenect_set_rgb_callback(freenect_device *dev, freenect_rgb_cb cb) { rgb_cb = cb; };

	int freenect_set_rgb_format(freenect_device *dev, freenect_rgb_format fmt) {return(1);};
	int freenect_set_depth_format(freenect_device *dev, freenect_depth_format fmt) {return(1);};

	int freenect_start_depth(freenect_device *dev) {return(1);};
	int freenect_start_rgb(freenect_device *dev) {return(1);};
	int freenect_stop_depth(freenect_device *dev) {return(1);};
	int freenect_stop_rgb(freenect_device *dev) {return(1);};

	void freenect_set_user(freenect_device *dev, void *user) { *dev = user; };
	void *freenect_get_user(freenect_device *dev) { return(*dev); };

	int freenect_process_events(freenect_context *ctx)
	{
		int x,y, pos;

		PsychYieldIntervalSeconds(0.0001);
		usbevent_counter++;

		if (usbevent_counter % 32 == 0) {
			pos = 0;
			for(y=0; y<480; y++)
				for (x=0; x<640; x++) {
					// z channel:
					depth_frame[pos++] = (unsigned short) usbevent_counter % 2048;
				}
			depth_cb(*ctx, depth_frame, usbevent_counter);
		}
		
		if (usbevent_counter % 32 == 16) {
			
			/* Fill our rawimage image array with content: */
			pos = 0;
			for(y=0; y<480; y++)
				for (x=0; x<640; x++) {
					// Red channel:
					rgb_frame[pos++] = (unsigned char) x + usbevent_counter / 4;
					// Green channel:
					rgb_frame[pos++] = (unsigned char) y + usbevent_counter / 4;
					// Blue channel:
					rgb_frame[pos++] = (unsigned char) usbevent_counter / 4;
				}
					
			rgb_cb(*ctx, rgb_frame, usbevent_counter);
		}
		
		return(0);
	}

#else
	// The real thing: Include headers of libusb and libfreeenect to access the real Kinect:
	#include "libfreenect.h"
#endif

// Number of maximum simultaneously open kinect devices:
#define MAX_PSYCH_KINECT_DEVS 10
#define MAX_SYNOPSIS_STRINGS 20  

//declare variables local to this file.  
static const char *synopsisSYNOPSIS[MAX_SYNOPSIS_STRINGS];

// Temporary buffer for conversion of depth images into color coded RGB images:
unsigned char gl_depth_back[640 * 480 * 4];
unsigned short t_gamma[2048];
double	 zmap[640*480*3];

freenect_context *f_ctx = NULL;
freenect_device *f_dev;

typedef struct PsychKNBuffer {
	unsigned short depth[640 * 480];		// 16-bit disparity pixels.
	unsigned char* color;					// RGB8 or raw non-bayer-filtered 1 Byte color pixels.
	double	  cts;							// Cached captured sample onset time from paCallback.
	int				cwidth, cheight;		// color buffer width x height.
	int				dwidth, dheight;		// color buffer width x height.
} PsychKNBuffer;

// Our device record:
typedef struct PsychKNDevice {
	psych_mutex	mutex;					// Mutex lock for the PsychKNDevice struct.
	psych_condition changeSignal;		// Condition variable or event object for change signalling (see above).
	psych_thread captureThread;			// Processing thread.
	freenect_device *dev;				// Handle to USB device representing the Kinect.
	PsychKNBuffer* buffers;				// Bufferchain with capture buffers.
	int	numbuffers;						// Number of allocated buffers.
	int dropframes;						// dropframes = 1: Stall capture if FIFO full, 0 = Overwrite oldest frames.
	int bayerFilterMode;				// 0 = Don't: Fetch unfiltered sensor data. 1 = Let libfreenect do Bayer filtering into RGB. 2 = ...
	double	captureStartTime;			// Time of start of capture.
	volatile unsigned int state;		// Current state: 0=Stopped, 1=Running.
	volatile unsigned int reqstate;		// Requested state of the stream, as opposed to current 'state'. Written by main-thread.
	unsigned int recposition;			// Current record position in frames since start of capture.
	unsigned int readposition;			// Last read-out frame since start of capture.
	unsigned int frame_valid;			// Current readposition valid?
	unsigned int xruns;					// Number of over-/underflows.
	unsigned int paCalls;				// Number of callback invocations.
} PsychKNDevice;

PsychKNDevice kinectdevices[MAX_PSYCH_KINECT_DEVS];
unsigned int  devicecount = 0;
unsigned int  verbosity = 3;
psych_bool initialized = FALSE;

void InitializeSynopsis(void)
{
	int i=0;
	const char **synopsis = synopsisSYNOPSIS;  //abbreviate the long name

	synopsis[i++] = "PsychKinect - A Psychtoolbox driver for the Microsoft Kinect box.\n";
	synopsis[i++] = "This driver allows to control the box and grab color images and depth";
	synopsis[i++] = "images from the Kinect depthcamera.\n";
	synopsis[i++] = "It uses the free software drivers and libraries from the OpenKinect";
	synopsis[i++] = "project (Thanks!): http://openkinect.org\n";
	synopsis[i++] = "Libfreenect is Copyright (C) 2010  Hector Martin \"marcan\" <hector@marcansoft.com>";
	synopsis[i++] = "This code is licensed to you under the terms of the GNU GPL, version 2; see: \n";
	synopsis[i++] = "http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt";
	synopsis[i++] = "\n";

	synopsis[i++] = "Usage:";

	// synopsis[i++] = "devices = Screen('VideoCaptureDevices' [, engineId]);";
	synopsis[i++] = "kinectPtr = PsychKinect('Open' [, deviceIndex=0][, numbuffers=2]][, bayerFilterMode=1]);";
	synopsis[i++] = "PsychKinect('Close', kinectPtr);";
    synopsis[i++] = "[starttime, fps, cwidth, cheight, dwidth, dheight] = PsychKinect('Start', kinectPtr [, dropframes=0]);";
    synopsis[i++] = "PsychKinect('Stop', kinectPtr);";
    synopsis[i++] = "status = PsychKinect('GetStatus', kinectPtr);";
    synopsis[i++] = "[result, cts, age] = PsychKinect('GrabFrame', kinectPtr [, waitMode=1][, mostrecent=0]);";
    synopsis[i++] = "PsychKinect('ReleaseFrame', kinectPtr);";
	synopsis[i++] = "[imageOrPtr, width, height, channels, extType, extFormat] = PsychKinect('GetImage', kinectPtr [, imtype=0][, returnTexturePtr=0]);";
	synopsis[i++] =	"[imageOrPtr, width, height, extType, extFormat] = PsychKinect('GetDepthImage', kinectPtr [, format=0][, returnTexturePtr=0]);";
	synopsis[i++] = NULL;  //this tells PsychKinectDisplaySynopsis where to stop

	if (i > MAX_SYNOPSIS_STRINGS) {
		PrintfExit("%s: increase dimension of synopsis[] from %ld to at least %ld and recompile.", __FILE__, (long)MAX_SYNOPSIS_STRINGS,(long)i);
	}
}

PsychError PsychKinectDisplaySynopsis(void)
{
	int i;
	
	for (i = 0; synopsisSYNOPSIS[i] != NULL; i++)
		printf("%s\n",synopsisSYNOPSIS[i]);
		
	return(PsychError_none);
}

PsychKNBuffer* PsychGetKNBuffer(PsychKNDevice* kinect, int index)
{
	return(&kinect->buffers[index % kinect->numbuffers]);
}

void PsychDepthCB(freenect_device *dev, freenect_depth *depth, uint32_t timestamp)
{
	int i;

	PsychKNDevice *kinect = (PsychKNDevice*) freenect_get_user(dev);
	PsychKNBuffer *buffer = PsychGetKNBuffer(kinect, kinect->recposition);
	memcpy(buffer->depth, depth, FREENECT_FRAME_W * FREENECT_FRAME_H * 2);
	buffer->dwidth = FREENECT_FRAME_W;
	buffer->dheight = FREENECT_FRAME_H;
	
	// Mark depth frame as done:
	kinectdevices[0].paCalls++;
}

void PsychRGBCB(freenect_device *dev, freenect_pixel *rgb, uint32_t timestamp)
{
	int i;
	
	PsychKNDevice *kinect = (PsychKNDevice*) freenect_get_user(dev);
	PsychKNBuffer *buffer = PsychGetKNBuffer(kinect, kinect->recposition);
	memcpy(buffer->color, rgb, ((kinect->bayerFilterMode == 1) ? FREENECT_RGB_SIZE : FREENECT_BAYER_SIZE));
	buffer->cwidth = FREENECT_FRAME_W;
	buffer->cheight = FREENECT_FRAME_H;

	// Timestamp the buffer:
	buffer->cts = (double) timestamp;

	kinectdevices[0].paCalls++;
}

void* PsychKinectThreadMain(volatile void* deviceToCast)
{
	PsychKNDevice *kinect = (PsychKNDevice*) deviceToCast;
	int abort = FALSE;
	int headroom;
	
	// Child protection:
	if ((NULL == kinect) || (NULL == kinect->dev)) return(NULL);

	// Initialize libkinect and attach callbacks, start kinect's iso-streaming:
	freenect_set_depth_callback(kinect->dev, PsychDepthCB);
	freenect_set_rgb_callback(kinect->dev, PsychRGBCB);
	freenect_set_rgb_format(kinect->dev, ((kinect->bayerFilterMode == 1) ? FREENECT_FORMAT_RGB : FREENECT_FORMAT_BAYER));
	freenect_set_depth_format(kinect->dev, FREENECT_FORMAT_11_BIT);

	freenect_start_depth(kinect->dev);
	freenect_start_rgb(kinect->dev);

	// Main processing loop:
	while(!abort) {
		// Check for thread cancellation:
		PsychTestCancelThread(&kinect->captureThread);

		// Check again for cancellation, this time the "soft way":
		PsychLockMutex(&kinect->mutex);
		if (kinect->reqstate != 1) {
			abort = TRUE;
			PsychUnlockMutex(&kinect->mutex);
			continue;
		};

		headroom = kinect->numbuffers - 1 - (kinect->recposition - kinect->readposition);

		PsychUnlockMutex(&kinect->mutex);
		
		// Do something?
		if (!abort && ((headroom > 0) || (kinect->dropframes))) {
			// xrun due to oldest frame gets overwritten?
			if (headroom <= 0) kinect->xruns++;

			// Kick off USB event handling and our callbacks:
			if (freenect_process_events(f_ctx) < 0) {
				// Error condition! We better abort!
				abort = TRUE;
				printf("PTB-CRITICAL: Error during USB data receive from Kinect! Aborting. Prepare for trouble!\n");
				continue;
			}
			
			// Check if we have a completely new video & depth frame:
			if (kinect->paCalls >= 2) {
				// Yes. Reset counter:
				kinect->paCalls = 0;

				// Increment our write pointer to finalize this frame and prepare capture of a new one:
				PsychLockMutex(&kinect->mutex);
				kinect->recposition++;
				
				// Signal good news to main thread:
				PsychSignalCondition(&kinect->changeSignal);
				PsychUnlockMutex(&kinect->mutex);
			}
		}
		else {
			// FIFO full and we should retain the oldest frames. Just idle
			// until capacity becomes available:
			PsychYieldIntervalSeconds(0.002);

			// xrun due to current frame gets dropped:
			kinect->xruns++;
		}

		// Repeat processing loop:
	}
	
	// Stop capture:
	freenect_stop_depth(kinect->dev);
	freenect_stop_rgb(kinect->dev);
	
	// Stop of capture:
	PsychLockMutex(&kinect->mutex);
	kinect->reqstate = 0;
	kinect->state = 0;
	kinect->paCalls = 0;
	PsychUnlockMutex(&kinect->mutex);

	return(NULL);
}

PsychKNDevice* PsychGetKinect(int handle, psych_bool dontfail)
{
	if (handle < 0 || handle >= MAX_PSYCH_KINECT_DEVS || kinectdevices[handle].dev == NULL) {
		if (!dontfail) {
			printf("PTB-ERROR: Invalid Kinect device handle %i passed. No such device open.\n");
			PsychErrorExitMsg(PsychError_user, "Invalid kinect handle.");
		}
		
		return(NULL);
	}

	return(&(kinectdevices[handle]));
}

void PsychKNStop(int handle)
{
	PsychKNDevice* kinect;
	kinect = PsychGetKinect(handle, TRUE);
	if (NULL == kinect) return;

	PsychLockMutex(&kinect->mutex);

	// Stop capture if it is still running:
	if (kinect->state == 1) {
		// Send abort request to thread:
		kinect->reqstate = 0;

		PsychUnlockMutex(&kinect->mutex);
		PsychYieldIntervalSeconds(0.1);

		PsychAbortThread(&kinect->captureThread);

		// Wait for it to die peacefully:
		PsychDeleteThread(&kinect->captureThread);
		kinect->captureThread = 0;
		
		// Mark as dead:
		kinect->state = 0;
	}
	else {
		PsychUnlockMutex(&kinect->mutex);
	}

	return;
}

void PsychKNClose(int handle)
{
	int i;
	PsychKNDevice* kinect;
	kinect = PsychGetKinect(handle, TRUE);
	if (NULL == kinect) return;

	// Stop device:
	PsychKNStop(handle);

	// Capture is stopped. Release mutexes and other resources:
	PsychDestroyCondition(&kinect->changeSignal);
	PsychDestroyMutex(&kinect->mutex);

	if (kinect->buffers) {
		for (i = 0; i < kinect->numbuffers; i++) free(kinect->buffers[i].color);
		free(kinect->buffers);
		kinect->buffers = NULL;
	}

	// Close usb connection:
	freenect_close_device(kinect->dev);
	kinect->dev = NULL;
	
	// Done with this device:
	devicecount--;
	
	// Last device closed? If so, shutdown driver:
	if (devicecount <= 0) {
		devicecount = 0;
		initialized = FALSE;
		freenect_shutdown(f_ctx);
		f_ctx = NULL;
	}
}

void PsychKNInit(void) {
	int handle;
	int i;
	float v;

	for (handle = 0 ; handle < MAX_PSYCH_KINECT_DEVS; handle++) kinectdevices[handle].dev = NULL;
	devicecount = 0;
	initialized = FALSE;
	
	// Build depths gamma table:
	for (i=0; i<2048; i++) {
		v = i/2048.0;
		v = powf(v, 3.0f)* 6;
		t_gamma[i] = v*6*256;
	}
}

PsychError PsychKNShutdown(void) {
	int handle;
	
	if (initialized) {
		for (handle = 0 ; handle < MAX_PSYCH_KINECT_DEVS; handle++) PsychKNClose(handle);
	}
	
	return(PsychError_none);
}

PsychError PSYCHKINECTOpen(void)
{
    static char useString[] = "kinectPtr = PsychKinect('Open' [, deviceIndex=0][, numbuffers=2][, bayerFilterMode=1]);";
    //
    static char synopsisString[] = 
    "Open connection to Kinect box, return a 'kinectPtr' handle to it.\n\n"
    "The call tries to open the Kinect box with index 'deviceIndex', or the "
	"first detected box, if 'deviceIndex' is omitted. It configures the box "
	"for video and depth capture, using a n-buffered pool for internal storage "
	"of up to 'numbuffers' frames. 'numbuffers' defaults to 2 for double-buffering "
	"if omitted.\n"
	"'bayerFilterMode' is optional and selects mode of color reconstruction from "
	"the visual cameras sensor data: A setting of 1 (default) will use a slow and "
	"unoptimized bayer filter algorithm built into the libfreenect driver to return "
	"RGB images via the 'GetImage' function. A setting of 0 will disable filtering and return "
	"a 2D only image with sensor raw data, which you'll need to post-process and filter yourself. "
	"A setting of > 1 would select other filtering methods, but these are not yet implemented. "
	"You would want to use the default value if you need convenient access to RGB images without "
	"any effort on your side. Unfiltered raw data has the advantage that cpu load of kinect "
	"operation is much lower, so you may get higher framerates. Also, raw data only requires "
	"a third of the storage memory if this is of concern, e.g., for high settings of 'numbuffers'.\n"
	"The returned handle can be passed to the other subfunctions to operate the device.\n";

    static char seeAlsoString[] = "";	

	int rc, i, j;
	int deviceIndex = 0;
	int handle = 0;
	freenect_device *dev = NULL;
	int numbuffers = 2;
	int bayerFilterMode = 1;
	
    // All sub functions should have these two lines
    PsychPushHelp(useString, synopsisString,seeAlsoString);
    if(PsychIsGiveHelp()){PsychGiveHelp();return(PsychError_none);};

    //check to see if the user supplied superfluous arguments
    PsychErrorExit(PsychCapNumOutputArgs(1));
    PsychErrorExit(PsychCapNumInputArgs(3));
	
	if (devicecount >= MAX_PSYCH_KINECT_DEVS) PsychErrorExitMsg(PsychError_user, "Maximum number of simultaneously open kinect devices reached.");

	// Find a free device slot:
	for (handle = 0; (handle < MAX_PSYCH_KINECT_DEVS) && kinectdevices[handle].dev; handle++);
	if (kinectdevices[handle].dev) PsychErrorExitMsg(PsychError_internal, "Maximum number of simultaneously open kinect devices reached. This should not happen!");

	// Get optional kinect device index:
    PsychCopyInIntegerArg(1, FALSE, &deviceIndex);
	
	// Don't support anything than a single "default" kinect yet:
	if (deviceIndex != 0) PsychErrorExitMsg(PsychError_user, "No such kinect device with given 'deviceIndex'.");

	// Get optional numbuffers count:
    PsychCopyInIntegerArg(2, FALSE, &numbuffers);
	if (numbuffers < 1) PsychErrorExitMsg(PsychError_user, "Invalid value for 'numbuffers' provided. Must be greater than 1!");

	// Get optional bayerFilterMode:
    PsychCopyInIntegerArg(3, FALSE, &bayerFilterMode);
	if (bayerFilterMode < 0) PsychErrorExitMsg(PsychError_user, "Invalid value for 'bayerFilterMode' provided. Must be >= 0!");

	// Open and initialize the device:

	if (!initialized) {
		// First time init:
		initialized = TRUE;

		// Initialize libusb:
		if (freenect_init(&f_ctx, NULL) < 0) PsychErrorExitMsg(PsychError_system, "Driver initialization of libfreenect failed!");
	}

	// Zero init device structure:
	memset(&kinectdevices[handle], 0, sizeof(PsychKNDevice));

	// Connect with it, get usb connection handle:
	
	if (freenect_open_device(f_ctx, &dev, deviceIndex) < 0) {
		PsychErrorExitMsg(PsychError_user, "Could not connect to kinect device with given 'deviceIndex' for some reason! [libusb_open_device failed]");
	}

	kinectdevices[handle].dev = dev;
	freenect_set_user(kinectdevices[handle].dev, (void*) &kinectdevices[handle]);

	// Have connection open and ready. Initialize mutexes, condition variables and processing thread:
	if (PsychInitMutex(&(kinectdevices[handle].mutex))) {
		printf("PsychKinect: CRITICAL! Failed to initialize Mutex object for handle %i! Prepare for trouble!\n", handle);
		PsychErrorExitMsg(PsychError_system, "Device mutex creation failed!");
	}

	// If we use locking, this will create & init the associated event variable:
	PsychInitCondition(&(kinectdevices[handle].changeSignal), NULL);

	// Allocate buffers:
	kinectdevices[handle].buffers = (PsychKNBuffer*) calloc(numbuffers, sizeof(PsychKNBuffer));
	if (NULL == kinectdevices[handle].buffers) {
		printf("PTB-ERROR: Could not create requested %i kinect image buffers. Prepare for trouble!\n", numbuffers);
		PsychErrorExitMsg(PsychError_outofMemory, "Buffer creation failed!");
	}
	
	for (i = 0; i < numbuffers; i++) {
		kinectdevices[handle].buffers[i].color = (unsigned char*) calloc(1, ((bayerFilterMode == 1) ? FREENECT_RGB_SIZE : FREENECT_BAYER_SIZE));
		if (NULL == kinectdevices[handle].buffers[i].color) {
			for (j = 0; j < i; j++) free(kinectdevices[handle].buffers[j].color);
			free(kinectdevices[handle].buffers);
			kinectdevices[handle].buffers = NULL;
		}
	}

	kinectdevices[handle].numbuffers = numbuffers;
	kinectdevices[handle].bayerFilterMode = bayerFilterMode;

	// Increment count of open devices:
	devicecount++;

	// Return device handle:
    PsychCopyOutDoubleArg(1, FALSE, handle);

    return(PsychError_none);	
}

PsychError PSYCHKINECTClose(void)
{
    static char useString[] = "PsychKinect('Close', kinectPtr);";
    //
    static char synopsisString[] = 
    "Close connection to Kinect box 'kinectPtr'. Do nothing if no such box is open.\n\n";

    static char seeAlsoString[] = "";	

	int handle;

    // All sub functions should have these two lines
    PsychPushHelp(useString, synopsisString,seeAlsoString);
    if(PsychIsGiveHelp()){PsychGiveHelp();return(PsychError_none);};

    //check to see if the user supplied superfluous arguments
    PsychErrorExit(PsychCapNumOutputArgs(0));
    PsychErrorExit(PsychCapNumInputArgs(1));
    
	// Get device handle:
	PsychCopyInIntegerArg(1, TRUE, &handle);
	
	// Close device:
	PsychKNClose(handle);

    return(PsychError_none);	
}

PsychError PSYCHKINECTStart(void)
{
    static char useString[] = "[starttime, fps, cwidth, cheight, dwidth, dheight] = PsychKinect('Start', kinectPtr [, dropframes=0]);";
    //							1		   2	3		4		 5		 6								 1			  2
    static char synopsisString[] = 
    "Start video and depth capture operation of box 'kinectPtr'.\n\n"
    "Starts the capture operation into the internal bufferqueue. Color- and "
    "depths frames can be polled/waited for/dequeued from the queue via a "
	"call to PsychKinect('GrabFrame',...); and then retrieved via the 'GetXXX' "
	"functions. Data is stored in an internal ringbuffer, whose size is "
	"set in the 'Open' function. The optional 'dropframes' flag decides if "
	"the oldest frames should be dropped if the buffer is full (=1) or if "
	"the most recent frame should be dropped if the buffer is full (=0). "
	"\n\n"
	"The function returns the start time of capture and the nominal framerate "
	"'fps', color image width 'cwidth' and height 'cheight', and depth image "
	"'dwidth' and 'dheight'.\n\n";
	
    static char seeAlsoString[] = "";	

	int handle, dropframes;
	int rc;
	
	PsychKNDevice *kinect;

    // All sub functions should have these two lines
    PsychPushHelp(useString, synopsisString,seeAlsoString);
    if(PsychIsGiveHelp()){PsychGiveHelp();return(PsychError_none);};

    //check to see if the user supplied superfluous arguments
    PsychErrorExit(PsychCapNumOutputArgs(6));
    PsychErrorExit(PsychCapNumInputArgs(2));
    
	// Get device handle:
	PsychCopyInIntegerArg(1, TRUE, &handle);
	kinect = PsychGetKinect(handle, FALSE);

	dropframes = 0;
	PsychCopyInIntegerArg(2, FALSE, &dropframes);
	
	PsychLockMutex(&kinect->mutex);
	if (kinect->state > 0) {
		PsychUnlockMutex(&kinect->mutex);
		PsychErrorExitMsg(PsychError_user, "Tried to start an already started capture operation!");
	}

	// Reset device state:
	kinect->recposition = 0;
	kinect->readposition = 0;
	kinect->reqstate = 1;
	kinect->state = 1;
	kinect->xruns = 0;
	kinect->paCalls = 0;
	kinect->frame_valid = 0;
	kinect->dropframes = (dropframes > 0) ? 1 : 0;
	
	// Create and startup thread:
	if ((rc=PsychCreateThread(&(kinectdevices[handle].captureThread), NULL, (void*) PsychKinectThreadMain, (void*) &(kinectdevices[handle])))) {
		printf("PTB-ERROR: Could not create kinect processing thread [%s]. Prepare for trouble!\n", strerror(rc));
		PsychErrorExitMsg(PsychError_system, "Thread creation failed!");
	}
	
	PsychUnlockMutex(&kinect->mutex);
	
	// TODO: Compute sensible starttime:
	PsychGetAdjustedPrecisionTimerSeconds(&(kinect->captureStartTime));

	PsychCopyOutDoubleArg(1, FALSE, kinect->captureStartTime);
	PsychCopyOutDoubleArg(2, FALSE, 30);
	PsychCopyOutDoubleArg(3, FALSE, 640);
	PsychCopyOutDoubleArg(4, FALSE, 480);
	PsychCopyOutDoubleArg(5, FALSE, 640);
	PsychCopyOutDoubleArg(6, FALSE, 480);

    return(PsychError_none);	
}

PsychError PSYCHKINECTStop(void)
{
    static char useString[] = "PsychKinect('Stop', kinectPtr);";
    //
    static char synopsisString[] = 
    "Stop capture on Kinect box 'kinectPtr'.\n\n";

    static char seeAlsoString[] = "";	

	int handle;

    // All sub functions should have these two lines
    PsychPushHelp(useString, synopsisString,seeAlsoString);
    if(PsychIsGiveHelp()){PsychGiveHelp();return(PsychError_none);};

    //check to see if the user supplied superfluous arguments
    PsychErrorExit(PsychCapNumOutputArgs(0));
    PsychErrorExit(PsychCapNumInputArgs(1));
    
	PsychCopyInIntegerArg(1, TRUE, &handle);

	// Stop device:
	PsychKNStop(handle);

    return(PsychError_none);	
}

PsychError PSYCHKINECTGetStatus(void)
{
    static char useString[] = "status = PsychKinect('GetStatus', kinectPtr);";
    //
    static char synopsisString[] = 
    "Retrieve struct with current status of Kinect box 'kinectPtr'.\n\n"
    "\n";

    static char seeAlsoString[] = "";	

	int handle;
	PsychKNDevice *kinect;

	int navail;
	
    // All sub functions should have these two lines
    PsychPushHelp(useString, synopsisString,seeAlsoString);
    if(PsychIsGiveHelp()){PsychGiveHelp();return(PsychError_none);};

    //check to see if the user supplied superfluous arguments
    PsychErrorExit(PsychCapNumOutputArgs(1));
    PsychErrorExit(PsychCapNumInputArgs(1));
    
	PsychCopyInIntegerArg(1, TRUE, &handle);	
	kinect = PsychGetKinect(handle, FALSE);
	
	PsychLockMutex(&kinect->mutex);

	navail = kinect->recposition - kinect->readposition;	

	printf("\n");
	printf("Running: %i\n", kinect->state);
	printf("Read: %i\n", kinect->readposition);
	printf("Write: %i\n", kinect->recposition);
	printf("navail: %i\n", navail);
	printf("\n");

	PsychUnlockMutex(&kinect->mutex);

	// TODO: Return sensible status struct:
	PsychCopyOutDoubleArg(1, FALSE, 123456);

    return(PsychError_none);	
}

PsychError PSYCHKINECTGrabFrame(void)
{
    static char useString[] = "[result, cts, age] = PsychKinect('GrabFrame', kinectPtr [, waitMode=1][, mostrecent=0]);";
    //							1		2	 3								 1			  2				3
    static char synopsisString[] = 
    "Poll or wait for arrival of new captured frames, release old ones.\n\n"
    "This function checks box 'kinectPtr' for the availability of frames. "
	"If 'waitMode' is 1, the function waits/blocks until data is available "
	"if no new frames are available at time of call. Otherwise it just "
	"polls in a non-blocking fashion.\n"
	"If 'mostrecent' is 1, then the most recent frame is returned and old "
	"frames are discarded - this for low-latency live capture.\n"
	"\n\n"
	"'result' returns the status: 0 = No frame returned, 1 = Frame returned, 2 = Error.\n"
	"'cts' returns a capture time stamp of when the frame was captured.\n"
	"'age' returns the age of the frame -- the number of pending frames in the buffer queue.\n"
	"\n\n";

    static char seeAlsoString[] = "";	

	int handle;
	PsychKNDevice *kinect;
	PsychKNBuffer *buffer;
	int waitMode, mostrecent;
	int navail;
	double cts;
	
    // All sub functions should have these two lines
    PsychPushHelp(useString, synopsisString,seeAlsoString);
    if(PsychIsGiveHelp()){PsychGiveHelp();return(PsychError_none);};

    //check to see if the user supplied superfluous arguments
    PsychErrorExit(PsychCapNumOutputArgs(3));
    PsychErrorExit(PsychCapNumInputArgs(3));
    
	PsychCopyInIntegerArg(1, TRUE, &handle);	
	kinect = PsychGetKinect(handle, FALSE);

	waitMode = 1;
	PsychCopyInIntegerArg(2, FALSE, &waitMode);	

	mostrecent = 0;
	PsychCopyInIntegerArg(3, FALSE, &mostrecent);	

	if (kinect->frame_valid) PsychErrorExitMsg(PsychError_user, "Must 'ReleaseFrame' old frame first, before grabbing a new one!");
	if ((kinect->state == 0) && (waitMode!=0)) PsychErrorExitMsg(PsychError_user, "Must 'Start' before first call to 'GrabFrame'!");
	PsychLockMutex(&kinect->mutex);

	// New data available?
	navail = kinect->recposition - kinect->readposition;	

	if (verbosity > 3) {
		printf("\n");
		printf("Running: %i\n", kinect->state);
		printf("Read: %i\n", kinect->readposition);
		printf("Write: %i\n", kinect->recposition);
		printf("navail: %i\n", navail);
		printf("\n");
	}

	while (navail <= 0) {
		// No. Shall we wait?
		if (waitMode == 0) {
			// No, was just polling: Return no data:
			PsychUnlockMutex(&kinect->mutex);
			PsychCopyOutDoubleArg(1, FALSE, 0);
			PsychCopyOutDoubleArg(2, FALSE, -1);
			PsychCopyOutDoubleArg(3, FALSE, 0);
			return(PsychError_none);
		}
		
		// Yes. Wait for change notification from thread:
		PsychWaitCondition(&kinect->changeSignal, &kinect->mutex);
		
		// Recheck:
		navail = kinect->recposition - kinect->readposition;		

		if (verbosity > 3) {
			printf("\n");
			printf("Running: %i\n", kinect->state);
			printf("Read: %i\n", kinect->readposition);
			printf("Write: %i\n", kinect->recposition);
			printf("navail: %i\n", navail);
			printf("\n");
		}
	}

	// Ok, at this point we have at least one frame of data available.
	if ((navail > 1) && (mostrecent == 1)) {
		// Want most recent. Increment readpos to last avail frame:
		kinect->readposition+= navail - 1;
	}

	// Recompute available amount:
	navail = kinect->recposition - kinect->readposition;		

	// Retrieve bufferptr:
	buffer = PsychGetKNBuffer(kinect, kinect->readposition);
	cts = buffer->cts;

	PsychUnlockMutex(&kinect->mutex);

	PsychCopyOutDoubleArg(1, FALSE, 1);
	PsychCopyOutDoubleArg(2, FALSE, cts);
	PsychCopyOutDoubleArg(3, FALSE, navail);

	kinect->frame_valid = TRUE;

    return(PsychError_none);	
}

PsychError PSYCHKINECTReleaseFrame(void)
{
    static char useString[] = "PsychKinect('ReleaseFrame', kinectPtr);";
    static char synopsisString[] = 
    "Release current frame back to bufferqueue.\n\n";
    static char seeAlsoString[] = "";	

	int handle;
	PsychKNDevice *kinect;

	int navail;
	
    // All sub functions should have these two lines
    PsychPushHelp(useString, synopsisString,seeAlsoString);
    if(PsychIsGiveHelp()){PsychGiveHelp();return(PsychError_none);};

    //check to see if the user supplied superfluous arguments
    PsychErrorExit(PsychCapNumOutputArgs(3));
    PsychErrorExit(PsychCapNumInputArgs(3));
    
	PsychCopyInIntegerArg(1, TRUE, &handle);	
	kinect = PsychGetKinect(handle, FALSE);
	if (!kinect->frame_valid) PsychErrorExitMsg(PsychError_user, "Must 'GrabFrame' a frame first!");

	PsychLockMutex(&kinect->mutex);
	kinect->readposition++;
	navail = kinect->recposition - kinect->readposition;	
	PsychUnlockMutex(&kinect->mutex);

	kinect->frame_valid = FALSE;
	
    return(PsychError_none);	
}

PsychError PSYCHKINECTGetImage(void)
{
    static char useString[] = "[imageOrPtr, width, height, channels, extType, extFormat] = PsychKinect('GetImage', kinectPtr [, imtype=0][, returnTexturePtr=0]);";
    //							1			2	   3	   4		 5		  6									   1			2			3
    static char synopsisString[] = 
    "Return the color image data for the frame fetched via 'GrabFrame'.\n\n"
    "If 'returnTexturePtr' is zero (default), a uint8 3D image matrix is returned, which could "
	"be directly fed to image processing toolbox, imwrite() or Screen('MakeTexture').\n"
	"'imtype' if set to 1 returns a color-coded depth image instead of the RGB camera image.\n"
	"If 'returnTexturePtr' is one, a memory pointer to a RGBA8 rectangle texture buffer "
	"is returned, for use with Screen('SetOpenGLTextureFromMemPointer') for injection into "
	"PTB's texturing system.\n";

    static char seeAlsoString[] = "";	

	int handle;
	PsychKNDevice *kinect;
	PsychKNBuffer* buffer;
	unsigned char* outimg;
	int returnTexturePtr, imtype;
	int i, pval, lb;
	int channels = 3;

    // All sub functions should have these two lines
    PsychPushHelp(useString, synopsisString,seeAlsoString);
    if(PsychIsGiveHelp()){PsychGiveHelp();return(PsychError_none);};

    //check to see if the user supplied superfluous arguments
    PsychErrorExit(PsychCapNumOutputArgs(6));
    PsychErrorExit(PsychCapNumInputArgs(3));

	PsychCopyInIntegerArg(1, TRUE, &handle);	
	kinect = PsychGetKinect(handle, FALSE);
	if (!kinect->frame_valid) PsychErrorExitMsg(PsychError_user, "Must 'GrabFrame' a frame first!");

	imtype=0;
	PsychCopyInIntegerArg(2, FALSE, &imtype);	

	returnTexturePtr=0;
	PsychCopyInIntegerArg(3, FALSE, &returnTexturePtr);	
	
	// Retrieve bufferptr:
	buffer = PsychGetKNBuffer(kinect, kinect->readposition);
	
	if (imtype == 1) {
		// Return color encoded depth image:
		
		// Need to convert into false-color image:		
		for (i=0; i<640*480; i++) {
			pval = t_gamma[buffer->depth[i]];
			lb = pval & 0xff;
			switch (pval>>8) {
				case 0:
					gl_depth_back[3*i+0] = 255;
					gl_depth_back[3*i+1] = 255-lb;
					gl_depth_back[3*i+2] = 255-lb;
					break;
				case 1:
					gl_depth_back[3*i+0] = 255;
					gl_depth_back[3*i+1] = lb;
					gl_depth_back[3*i+2] = 0;
					break;
				case 2:
					gl_depth_back[3*i+0] = 255-lb;
					gl_depth_back[3*i+1] = 255;
					gl_depth_back[3*i+2] = 0;
					break;
				case 3:
					gl_depth_back[3*i+0] = 0;
					gl_depth_back[3*i+1] = 255;
					gl_depth_back[3*i+2] = lb;
					break;
				case 4:
					gl_depth_back[3*i+0] = 0;
					gl_depth_back[3*i+1] = 255-lb;
					gl_depth_back[3*i+2] = 255;
					break;
				case 5:
					gl_depth_back[3*i+0] = 0;
					gl_depth_back[3*i+1] = 0;
					gl_depth_back[3*i+2] = 255-lb;
					break;
				default:
					gl_depth_back[3*i+0] = 0;
					gl_depth_back[3*i+1] = 0;
					gl_depth_back[3*i+2] = 0;
					break;
			}
		}

		// Return image data:
		if (returnTexturePtr) {
			// Just return a memory pointer to the colorbuffer:
			PsychCopyOutDoubleArg(1, FALSE, PsychPtrToDouble((void*) gl_depth_back));
		} else {
			PsychAllocOutUnsignedByteMatArg(1, FALSE, 3, buffer->dheight, buffer->dwidth, &outimg);
			memcpy(outimg, gl_depth_back, buffer->dheight * buffer->dwidth * 3);
		}

		// Fixed for current Kinect:
		PsychCopyOutDoubleArg(2, FALSE, buffer->dwidth);
		PsychCopyOutDoubleArg(3, FALSE, buffer->dheight);		
	}
	else {
		// Return color image from visual camera:
		
		// If these are not bayer filtered by libfreenect, we only have 1 byte per
		// pixel for a 2D raw sensor data matrix:
		channels = (kinect->bayerFilterMode == 1) ? 3 : 1;
		
		if (returnTexturePtr) {
			// Just return a memory pointer to the colorbuffer:
			PsychCopyOutDoubleArg(1, FALSE, PsychPtrToDouble((void*) buffer->color));
		} else {
			PsychAllocOutUnsignedByteMatArg(1, FALSE, channels, buffer->cheight, buffer->cwidth, &outimg);
			memcpy(outimg, buffer->color, buffer->cheight * buffer->cwidth * channels);
		}

		// Fixed for current Kinect:
		PsychCopyOutDoubleArg(2, FALSE, buffer->cwidth);
		PsychCopyOutDoubleArg(3, FALSE, buffer->cheight);		
	}
	
	// Number of image channels:
	PsychCopyOutDoubleArg(4, FALSE, channels);

	// GL_RGB8 or GL_LUMINANCE8
	PsychCopyOutDoubleArg(5, FALSE, (channels == 3) ? 32849 : 0x8040);

	// GL_UNSIGNED_BYTE:
	PsychCopyOutDoubleArg(6, FALSE, 5121);

    return(PsychError_none);	
}

PsychError PSYCHKINECTGetDepthImage(void)
{
    static char useString[] = "[imageOrPtr, width, height, extType, extFormat] = PsychKinect('GetDepthImage', kinectPtr [, format=0][, returnTexturePtr=0]);";
    //							1			2	   3	   4		5										  1			   2		   3
    static char synopsisString[] = 
    "Return the depth image data for the frame fetched via 'GrabFrame'.\n\n"
    "If 'returnTexturePtr' is zero (default), a matrix is returned for processing in Matlab/Octave.\n\n"
	"If 'returnTexturePtr' is one, a memory pointer to a buffer is returned.\n\n"
	"'format' defines the type of returned data:\n"
	"0 = Return raw depths (disparity) image as 2D double matrix with integral values.\n"
	"1 = Return depths z-image as 2D double matrix with z-distance in meters.\n"
	"2 = Return a vertex buffer with (x,y,z) vertices that define a 3D surface mesh.\n"
	"\n\n";

    static char seeAlsoString[] = "";	

	int handle;
	PsychKNDevice *kinect;
	PsychKNBuffer* buffer;
	double* outzmat;
	int returnTexturePtr, format;
	int i, pval, lb;

    // All sub functions should have these two lines
    PsychPushHelp(useString, synopsisString,seeAlsoString);
    if(PsychIsGiveHelp()){PsychGiveHelp();return(PsychError_none);};

    //check to see if the user supplied superfluous arguments
    PsychErrorExit(PsychCapNumOutputArgs(5));
    PsychErrorExit(PsychCapNumInputArgs(3));

	PsychCopyInIntegerArg(1, TRUE, &handle);	
	kinect = PsychGetKinect(handle, FALSE);
	if (!kinect->frame_valid) PsychErrorExitMsg(PsychError_user, "Must 'GrabFrame' a frame first!");

	format = 0;
	PsychCopyInIntegerArg(2, FALSE, &format);	

	returnTexturePtr=0;
	PsychCopyInIntegerArg(3, FALSE, &returnTexturePtr);	
	
	// Retrieve bufferptr:
	buffer = PsychGetKNBuffer(kinect, kinect->readposition);
	
	switch(format) {
		case 0:
			// Return raw depth image:
			for (i=0; i < 640*480; i++) {
				pval = t_gamma[buffer->depth[i]];
				zmap[i] = (double) pval;
			}
			
			// Return image data:
			if (returnTexturePtr) {
				// Just return a memory pointer to the colorbuffer:
				PsychCopyOutDoubleArg(1, FALSE, PsychPtrToDouble((void*) zmap));
			} else {
				PsychAllocOutDoubleMatArg(1, FALSE, buffer->dheight, buffer->dwidth, 1, &outzmat);
				memcpy(outzmat, zmap, buffer->dheight * buffer->dwidth * 1 * sizeof(double));
			}
		break;
		
		case 1:
			// Return encoded and aligned depth image:
			// TODO: DO THIS CORRECTLY!
			for (i=0; i < 640*480; i++) {
				pval = t_gamma[buffer->depth[i]];
				zmap[i] = (double) pval;
			}
			
			// Return image data:
			if (returnTexturePtr) {
				// Just return a memory pointer to the colorbuffer:
				PsychCopyOutDoubleArg(1, FALSE, PsychPtrToDouble((void*) zmap));
			} else {
				PsychAllocOutDoubleMatArg(1, FALSE, buffer->dheight, buffer->dwidth, 1, &outzmat);
				memcpy(outzmat, zmap, buffer->dheight * buffer->dwidth * 1 * sizeof(double));
			}
		break;
		
		case 2:
			// Return (x,y,z) vertex buffer mesh:
			// TODO: DO THIS CORRECTLY!
			for (i=0; i < 640*480; i++) {
				pval = t_gamma[buffer->depth[i]];
				zmap[i*3 + 0] = (double) (i % 640);
				zmap[i*3 + 1] = (double) i / 640;
				zmap[i*3 + 2] = (double) pval;
			}
			
			// Return image data:
			if (returnTexturePtr) {
				// Just return a memory pointer to the colorbuffer:
				PsychCopyOutDoubleArg(1, FALSE, PsychPtrToDouble((void*) zmap));
			} else {
				PsychAllocOutDoubleMatArg(1, FALSE, buffer->dheight, buffer->dwidth, 3, &outzmat);
				memcpy(outzmat, zmap, buffer->dheight * buffer->dwidth * 3 * sizeof(double));
			}
		break;
		
	}
	
	// Fixed for current Kinect:
	PsychCopyOutDoubleArg(2, FALSE, buffer->dwidth);
	PsychCopyOutDoubleArg(3, FALSE, buffer->dheight);		
	
	// GL_RGB8:
	PsychCopyOutDoubleArg(4, FALSE, 32849);

	// GL_UNSIGNED_BYTE:
	PsychCopyOutDoubleArg(5, FALSE, 5121);

    return(PsychError_none);	
}
