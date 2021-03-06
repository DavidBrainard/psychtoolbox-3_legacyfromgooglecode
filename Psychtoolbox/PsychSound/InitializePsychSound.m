function InitializePsychSound(reallyneedlowlatency)
% InitializePsychSound([reallyneedlowlatency=0])
%
% This routine loads the PsychPortAudio sound driver for high-precision,
% low-latency, multi-channel sound playback and recording.
%
% Call it at the beginning of your experiment script, optionally providing
% the 'reallyneedlowlatency' flag set to one to push really hard for low
% latency.
%
% On MacOS/X and GNU/Linux, the PsychPortAudio driver will just work with
% the lowest possible latency and highest timing precision after this
% initialization.
%
% On Microsoft Windows, things are more complicated and painful as always:
%
% By default PsychPortAudio on Windows will use the "portaudio_x86.dll"
% low-level sound driver included in the Psychtoolbox/PsychSound/
% subfolder. This driver supports the Windows MME (MultiMediaExtensions)
% and DirectSound sound systems. Apart from being buggy on some systems,
% these sound systems only have a mildly accurate timing and a fairly high
% inherent latency, typically over 30 milliseconds. On 150$ class sound
% hardware we were able to achieve a trial-to-trial sound onset variability
% with a standard deviation of about 1 millisecond, which is still good
% enough for many purposes.
%
% The driver also supports the ASIO sound system provided by professional
% class sound cards. If you need really low latency or high precision sound
% on Windows, ASIO is what you want to use: Some (usually more expensive >
% 150$) professional class sound cards ship with ASIO enabled sound
% drivers, or at least there's such a driver available from the support
% area of the website of your sound card vendor.
%
% Disclaimer: "ASIO is a trademark and software of Steinberg Media
% Technologies GmbH."
%
% For cards without native ASIO drivers, there's the free ASIO4ALL driver,
% downloadable from http://asio4all.com, which may or may not work well on
% your specific sound card - The driver emulates the ASIO interface on top
% of the WDM-KS (Windows Driver Model Kernel Streaming) API from Microsoft,
% so the quality depends on the underlying WDM driver. If you manage to get
% such an ASIO enabled sound driver working on your sound hardware, and
% your ASIO enabled driver and sound card are of sufficiently high quality,
% you can enjoy latencies as low as 5 msecs and a sound onset accuracy with
% a standard deviation from the mean of less than 0.1 milliseconds on
% MS-Windows - We measured around 20 microseconds on some setups, e.g., the
% M-Audio Delta 1010-LT soundcard under Windows-XP SP2.
%
% Using OS/X or Linux will usually get you comparably good or better
% results with standard sound hardware.

% History:
% 6/6/2007    Written (MK).
% 10/20/2011  Update: We always use ASIO enabled plugin on Windows by
%             default, as the PTB V3.0.9 MIT style license allows bundling
%             of an ASIO enabled proprietary dll with Psychtoolbox. (MK)

if nargin < 1
    reallyneedlowlatency = [];
end

if isempty(reallyneedlowlatency)
    reallyneedlowlatency = 0; %#ok<NASGU> % Default: Don't push too hard for low latency.
end

% The usual tricks for MS-Windows:
if IsWin
    % Special ASIO enabled low-latency driver installed?
    if exist([PsychtoolboxRoot 'portaudio_x86.dll']) >= 2 %#ok<EXIST>
        % Yes! Use it:
        fprintf('Detected optional PortAudio override driver plugin in Psychtoolbox root folder. Will use that.\n');
        driverloadpath = PsychtoolboxRoot;
    else
        % No - We use our standard driver, which is also ASIO capable since
        % October 2011:
        driverloadpath = [PsychtoolboxRoot 'PsychSound'];
    end

    % Standard path trick: Change working directory to driver load path,
    % preinit PsychPortAudio so it gets linked against the proper DLL, change
    % path back to old current directory:
    try
        olddir = pwd;
        cd(driverloadpath);
        % We force loading+linking+init of the driver and at the same time
        % query for ASIO support: API-Id 3 means to only query ASIO
        % devices.
        d = PsychPortAudio('GetDevices', 3);
        cd(olddir);
        
        fprintf('\nWill use ASIO enhanced Portaudio driver DLL. See Psychtoolbox/PsychSound/PortAudioLICENSE.txt\n');
        fprintf('for the exact terms of use for this dll.\n');
        
        % Comply with the license...
        fprintf('\n\nDisclaimer: "ASIO is a trademark and software of Steinberg Media Technologies GmbH."\n');

        % Found ASIO device?
        if ~isempty(d)
            % And some more commercials as required by the license...
            fprintf('Using "ASIO Interface Technology by Steinberg Media Technologies GmbH"\n\n');
            fprintf('Found at least one ASIO enabled soundcard in your system. Good, will use that in low-latency mode!\n');
        else
            % ASIO PsychPortAudio driver, but no ASIO device in system!
            fprintf('PTB-Warning: Although using the ASIO enabled Psychtoolbox sound driver,\n');
            fprintf('PTB-Warning: could not find any ASIO capable soundcard in your system.\n');
            fprintf('PTB-Warning: If you think you should have an ASIO card, please check your\n');
            fprintf('PTB-Warning: system for properly installed and configured drivers and retry.\n');
            fprintf('PTB-Warning: Read "help InitializePsychSound" for more info about ASIO et al.\n');
        end
    catch
        cd(olddir);
        error('Failed to load PsychPortAudio driver for unknown reason! Dependency problem?!?');
    end
end

% Maybe some tricks (in the future) for OS/X? None yet.
if IsOSX
    try
        % We force loading+linking+init of the driver here, so in case
        % something goes wrong we can catch this and output useful
        % troubleshooting tips to the user:
        d = PsychPortAudio('GetDevices'); %#ok<NASGU>
        fprintf('PsychPortAudio initialized. Will use CoreAudio for audio.\n');
    catch
        fprintf('Failed to load PsychPortAudio driver!\n\n');
        fprintf('The most likely cause is that the helper library libportaudio.0.0.19.dylib is not\n');
        fprintf('stored in one of the library directories. This is the case at first use of the new\n');
        fprintf('sound driver.\n\n');
        fprintf('A copy of this file can be found in %s \n', [PsychtoolboxRoot 'PsychSound/libportaudio.0.0.19.dylib']);
        fprintf('You need to copy that file into one of the following directories, then retry:\n');
        fprintf('If you have administrator permissions, copy it to (at your option): /usr/local/lib\n');
        fprintf('or /usr/lib  -- you may need to create that directories first.\n\n');
        fprintf('If you are a normal user, you can also create a subdirectory lib/ in your home folder\n');
        fprintf('then copy the file there. E.g., your user name is lisa, then copy the file into\n');
        fprintf('/Users/lisa/lib/ \n\n');
        fprintf('Please try this steps, then restart your script.\n\n');
        em = psychlasterror;
        fprintf('The exact error message of the linker was: %s\n', em.message);
        fprintf('\n\n');
        error('Failed to load PsychPortAudio driver.');
    end
end

return;
