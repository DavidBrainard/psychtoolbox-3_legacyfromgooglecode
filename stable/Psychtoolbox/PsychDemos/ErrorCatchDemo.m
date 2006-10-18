function ErrorCatchDemo
%
% Show how to use try and catch to clean up after an error.
%
% 12/19/03  dhb  Wrote it based on suggestion from Scott French.
% 10/10/06  rhh  Edited it.
% 10/11/06	dhb   Turn off warnings, use max screen.

try
    % Enclose all your real code between try and catch.
    
    % Removes the blue screen flash and minimize extraneous warnings.
	Screen('Preference', 'VisualDebugLevel', 3);
    Screen('Preference', 'SuppressAllWarnings', 1);

    % Find out how many screens and use largest screen number.
    whichScreen = max(Screen('Screens'));
    
    Screen('OpenWindow', whichScreen);
    error('Oops!');
    Screen('CloseAll');

catch
    % If an error occurs, the catch statements executed.
    
    Screen('CloseAll');
    fprintf('We''ve hit an error.\n');
    psychrethrow(psychlasterror);
    fprintf('This text never prints.\n');
    
end