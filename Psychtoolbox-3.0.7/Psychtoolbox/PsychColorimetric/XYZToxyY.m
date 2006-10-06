function [xyY] = XYZToxyY(XYZ)
% [xyY] = XYZToxyY(XYZ)
% Compute chromaticity and luminance from
% from tristimulus values.

[m,n] = size(XYZ);
for i = 1:n
  xyY(1,i) = XYZ(1,i)./sum(XYZ(:,i));
  xyY(2,i) = XYZ(2,i)./sum(XYZ(:,i));
  xyY(3,i) = XYZ(2,i);
end
