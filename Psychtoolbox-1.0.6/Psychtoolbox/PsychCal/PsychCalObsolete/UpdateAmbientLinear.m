function cal = UpdateAmbientLinear(cal,new_ambient_linear)
% cal = UpdateAmbient(cal,new_ambient_linear)
%
% Update the ambient light used in the conversions.  The
% value for new_ambient_linear should be passed in the
% same units as defined by T_linear in the call to
% SetColorSpace.
%
% If instead you want to update in the measurement units,
% call UpdateAmbient instead.
%
% UpdateAmbientLinear has been renamed UpdateAmbientSensor".  The old
% name, "UpdateAmbientLinear", is still provided for compatability 
% with existing scripts but will disappear from future releases 
% of the Psychtoolbox.  Please use UpdateAmbientSensor instead.
%
% See Also: PsychCal, PsychCalObsolete, UpdateAmbientSensor

% 7/7/98    dhb		Wrote it.
% 4/5/02    dhb, ly  Call through new interface.
% 4/11/02   awi   Added help comment to use UpdateAmbientSensor instead.
%                 Added "See Also"


cal = UpdateAmbientSensor(cal,new_ambient_linear);