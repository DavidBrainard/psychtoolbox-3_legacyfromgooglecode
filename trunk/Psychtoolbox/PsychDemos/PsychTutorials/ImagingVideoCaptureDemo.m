function ImagingVideoCaptureDemo(filtertype, kwidth)

AssertOpenGL;
screen=max(Screen('Screens'));

if nargin < 1
    filtertype = 1;
end

if nargin < 2
    kwidth=11;
end;

try
    %Screen('Preference', 'Verbosity', 10);
    % Prepare pipeline for configuration. This marks the start of a list of
    % requirements/tasks to be met/executed in the pipeline:
    PsychImaging('PrepareConfiguration');
    
    % Ask pipeline to horizontally flip/mirror the output image, so user
    % doesn't get confused by orientation of its mirror image ;-)
    PsychImaging('AddTask', 'AllViews', 'FlipHorizontal');
    %PsychImaging('AddTask', 'AllViews', 'FlipVertical');
    %PsychImaging('AddTask', 'General', 'FloatingPoint16Bit');
    %PsychImaging('AddTask', 'General', 'EnableBits++Mono++Output');
    %PsychImaging('AddTask', 'AllViews', 'GeometryCorrection', '/tmp/BezierCalibdata.mat');
    %Testblock for Radeon native 10bpc framebuffer support:
    %PsychImaging('AddTask', 'General', 'FloatingPoint16Bit');
    %PsychImaging('AddTask', 'General', 'EnableNative10BitFramebuffer');

    [win winRect]=PsychImaging('OpenWindow', screen, 0, [], [], [], [], [], mor(kPsychNeedFastBackingStore, kPsychNeedImageProcessing));

    % Initial flip to a blank screen:
    Screen('Flip',win);

    % Set text size for info text. 24 pixels is also good for Linux.
    Screen('TextSize', win, 24);
        

    blurshader = LoadGLSLProgramFromFiles('ParametricBoxBlurShader', 1);
    glUseProgram(blurshader);
    glUniform1i(glGetUniformLocation(blurshader, 'Image'), 0);
    glUniform1i(glGetUniformLocation(blurshader, 'FilterMap'), 1);
    glUseProgram(0);
    bluroperator = CreateGLOperator(win, [], blurshader, 'Parametric box blur operator.');
    
    
    grabber = Screen('OpenVideoCapture', win, 0, [0 0 640 480]);

    blurmaptex = Screen('OpenOffscreenWindow', win, 0, [0 0 640 480]);
    cr = CenterRect([0 0 640 480], winRect);
    xr = cr(RectRight);
    yt = cr(RectTop);
    
    Screen('StartVideoCapture', grabber, 30, 1);

    oldpts = 0;
    count = 0;
    ftex = 0;
    tex = 0;
    t=GetSecs;
    while ~KbCheck        
        [tex pts nrdropped]=Screen('GetCapturedImage', win, grabber, 1, tex);
        % fprintf('tex = %i  pts = %f nrdropped = %i\n', tex, pts, nrdropped);
        Screen('FillRect', win, [255 0 0], [1 1 1680 1050]);
        Screen('FillRect', win, [0 255 0], [0 0 10 10]);
        if (tex>0)
            ftex = Screen('TransformTexture', tex, bluroperator, blurmaptex, ftex);
            % Draw new texture from framegrabber.
            Screen('DrawTexture', win, ftex); %Screen('Rect', win));
            
            % Show it.
            Screen('Flip', win);
        end;        
        
        count = count + 1;
        [x y buttons] = GetMouse(win);
        if any(buttons)
            x = xr - x;
            y = y - yt;
            if buttons(1)
                blurlevel = 5;
            else
                blurlevel = 0;
            end
            Screen('FillRect', blurmaptex, blurlevel, CenterRectOnPoint([0 0 30 30], x, y));
        end
    end
    telapsed = GetSecs - t
    Screen('StopVideoCapture', grabber);
    Screen('CloseVideoCapture', grabber);
    Screen('CloseAll');
    avgfps = count / telapsed
catch
   Screen('CloseAll');
   psychrethrow(psychlasterror);
end;