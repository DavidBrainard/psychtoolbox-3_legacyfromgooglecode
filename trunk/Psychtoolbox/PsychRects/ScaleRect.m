function r=ScaleRect(r,horizontalFactor,verticalFactor)
% r=ScaleRect(r,horizontalFactor,verticalFactor)
%
% Scales a rect, by multiplying the left and right coordinates by the 
% horizontal scale factor and multiplying the top and bottom coordinates
% by the vertical scale factor. Also see Expand and InsetRect. 
% Also see PsychRects.

% 5/27/96 Denis Pelli
% 7/10/96 dgp PsychRects
% 8/5/96 dgp check rect size.

if nargin~=3
	error('Usage: r=ScaleRect(r,horizontalFactor,verticalFactor)');
end
if size(r,2)~=4
	error('Wrong size rect argument. Usage: r=ScaleRect(r,horizontalFactor,verticalFactor)');
end
r=r.*SetRect(horizontalFactor,verticalFactor ...
	,horizontalFactor,verticalFactor);
