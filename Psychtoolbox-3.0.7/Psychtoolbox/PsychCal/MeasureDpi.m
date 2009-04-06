function dpi=MeasureDpi(theScreen)
% dpi=MeasureDpi(theScreen)
% Helps the user to measure the screen's dots per inch.
%
% Denis Pelli

% 5/28/96 dgp Updated to use new GetMouse
% 3/20/97 dgp Updated
% 4/2/97  dgp FlushEvents
% 4/26/97 dhb Got rid of call to disp(7), which was writing '7' to the
%             command window and not producing a beep.
% 8/16/97 dgp Changed "text" to "theText" to avoid conflict with TEXT function.
% 4/06/02 awi  -Check all elements of the new multi-element button vector returned   
%               by GetMouse on Windows.
%              -Replaced Chicago font with Arial because it's available on both Mac and Windows

if nargin~=1 | nargout>1
	error('Usage: dpi=MeasureDpi(screen)');
end

disp('Please find an object of known width to hold against the display.');
disp('E.g. a ruler or an 8.5-inch page.')
inches=input('Do you prefer inches (1) or cm (0)? ');
if inches
	unitInches=1;
	units='inches'; % e.g. distance in inches
	unit='inch';	% e.g. 5 inch object
else
	unitInches=1/2.54;
	units='cm';
	unit='cm';
end
objectInches=input(sprintf('How wide is your object, in %s? ',units))*unitInches;
disp('A small correction will be made for your viewing distance and the thickness of');
disp('the monitor''s glass, which separates your object from the screen''s phosphor.');
distanceInches=input(sprintf('What is your viewing distance, roughly, in %s? ',units))*unitInches;
thicknessInches=0.25;
[window,screenRect]=Screen(theScreen,'OpenWindow',0);
Screen(window,'SetClut',[0 0 0],255);
Screen(window,'SetClut',[255 255 255]);

% instructions
theText=sprintf('Hold your %.1f-%s-wide object against the display.'...
	,objectInches/unitInches,unit);
theText=str2mat(theText...
,'Press, drag, and release the mouse to draw a bar'...
,'that matches the width of your object. Use one eye.');
s=24;
Screen(window,'TextFont','Arial');
Screen(window,'TextSize',s);
textLeading=s+8;
textRect=[0,0,Screen(window,'TextWidth',theText(1,:)),size(theText,1)*textLeading];
textRect=CenterRect(textRect,screenRect);
textRect=AlignRect(textRect,screenRect,RectTop);
textRect(RectRight)=screenRect(RectRight);
dragText=theText;
dragTextRect=textRect;

% animate
% track horizontal mouse position to draw a bar of variable width.
for i=1:size(dragText,1)
	Screen(window,'DrawText',dragText(i,:),dragTextRect(RectLeft),dragTextRect(RectTop)+textLeading*i,255);
end
barRect=CenterRect(SetRect(0,0,RectWidth(screenRect),20),screenRect);
top=RectTop;
bottom=RectBottom;
left=RectLeft;
right=RectRight;
Screen(window,'FillRect',0,barRect);
Screen(window,'FrameRect',255,barRect);
oldButton=0;
while 1
	[x,y,button]=GetMouse;
	FlushEvents('mouseDown');
	if any(button) 
		if ~oldButton
			origin=x;
			barRect(left)=origin;
			barRect(right)=origin;
		else
			if x<origin
				barRect(left)=x;
				barRect(right)=origin;
			else
				barRect(left)=origin;
				barRect(right)=x;
			end
		end
		Screen(window,'FillRect',255,barRect);
		background=barRect;
		background(left)=screenRect(left);
		background(right)=barRect(left);
		Screen(window,'FillRect',0,background);
		background(left)=barRect(right);
		background(right)=screenRect(right);
		Screen(window,'FillRect',0,background);
	else
		if oldButton
			objectPix=RectWidth(barRect);
			dpi=objectPix/objectInches;
			dpi=dpi*distanceInches/(distanceInches+thicknessInches);
			if inches
				theText=sprintf('%.0f dots per inch.',dpi);
			else
				theText=sprintf('%.0f dots per inch. (%.0f dots/cm.)',dpi,dpi/2.54);
			end
			theText=str2mat(theText,'Click once to do it again; twice if you''re done.');
			textRect=SetRect(0,0,Screen(window,'TextWidth',theText(2,:)),size(theText,1)*textLeading);
			textRect=CenterRect(textRect,screenRect);
			textRect=AlignRect(textRect,screenRect,RectTop);
			Screen(window,'FillRect',0,InsetRect(dragTextRect,0,-0.3*textLeading));
			for i=1:size(theText,1)
				Screen(window,'DrawText',theText(i,:),textRect(RectLeft),textRect(RectTop)+textLeading*i,255);
			end
			i=GetClicks;
			Screen(window,'FillRect',0,barRect);
			Screen(window,'FillRect',0,InsetRect(textRect,0,-0.3*RectHeight(textRect)));
			if i>1
				break
			end
			for i=1:size(dragText,1)
				Screen(window,'DrawText',dragText(i,:),dragTextRect(RectLeft),dragTextRect(RectTop)+textLeading*i,255);
			end
		end
	end
	oldButton=any(button);
end
Screen(window,'Close');