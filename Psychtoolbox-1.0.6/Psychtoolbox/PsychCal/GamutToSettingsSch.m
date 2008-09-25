function [settings,values] = GamutToSettingsSch(gammaTable,gamut)
% [settings,values] = GamutToSettingsSch(gammaTable,gamut)
%
% Find the best integer device settings to produce
% the passed linear device coordinates.
% 
% The passed coordinates should be in the range [0,1].
% The returned settings run from [0,nlevels-1], where nlevels
% is the number of quantized levels available on the device.
%
% The returned argument values is what you actually should
% get after quantization error.
%
% The routine depends on the calibration globals.
%
% 9/26/93    dhb   Added calData argument.
% 10/19/93   dhb   Allow gamma table dimensions to exceed device settings.
% 11/11/93   dhb   Update for new calData routines.
% 8/4/96     dhb   Update for stuff bag routines.
%            dhb   Pulled out as a subroutine.
% 8/21/97	 	 dhb   Convert for structures.

% Check dimensions and table sizes
[m,n] = size(gamut);
[mg,ng] = size(gammaTable);
if (m > ng)
  error('Mismatch between device coordinate dimensions and gamma table');
end

% Use a search routine to find the best gamma function
settings = zeros(m,n);
values = zeros(m,n);
for i = 1:m
  [settings(i,:),values(i,:)] = SearchTable(gamut(i,:),gammaTable(:,i));
end
settings = settings-ones(m,n);