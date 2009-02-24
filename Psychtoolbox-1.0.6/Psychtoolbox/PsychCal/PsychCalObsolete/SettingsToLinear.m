function [linear] = SettingsToLinear(cal,settings)
% [linear] = SettingsToLinear(cal,settings)
%
% Convert from device setting coordinates to
% linear color space coordinates.
%
% SettingsToLinear has been renamed "SettingsToSensor".  The old
% name, "SettingsToLinear", is still provided for compatability 
% with existing scripts but will disappear from future releases 
% of the Psychtoolbox.  Please use SettingsToSensor instead.
%
% See Also: PsychCal, PsychCalObsolete, SettingsToSensor

% 9/26/93    dhb   Added cal argument.
% 4/5/02     dhb, ly  Call through new interface.
% 4/11/02   awi   Added help comment to use SettingsToSensor instead.
%                 Added "See Also"


[linear] = SettingsToSensor(cal,settings);