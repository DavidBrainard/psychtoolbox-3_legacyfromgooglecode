function cyl = SensorToCyl(cal,sensor)
% cyl = SensorToCyl(cal,sensor)
%
% Convert from sensor to cylindrical coordinates.
%
% We use the conventions of the CIE Lxx color spaces
% for angle.
%
% 10/17/93		dhb   Wrote it by converting CAP C code.
% 2/20/94			jms   Added single argument case to avoid cData.
% 4/5/02      dhb, ly  New calling interface.

if (nargin==1)
  sensor=cal;
end

cyl = sensor;
cyl(1,:) = sensor(1,:);
cyl(2,:) = sqrt( sensor(2,:).^2 + sensor(3,:).^2 );

index = find( sensor(2,:) == 0.0 & sensor(3,:) == 0.0 );
if (~isempty(index) )
  cyl(3,index) = zeros(1,length(index)); 
end
index = find( sensor(2,:) == 0.0 & sensor(3,:) > 0.0 );
if (~isempty(index) )
  cyl(3,index) = pi/2*ones(1,length(index));  
end
index = find( sensor(2,:) == 0.0 & sensor(3,:) < 0.0 );
if (~isempty(index) )
  cyl(3,index) = 3*pi/2*ones(1,length(index)); 
end    
index = find( sensor(2,:) > 0.0 & sensor(3,:) > 0.0 );
if (~isempty(index) )
  cyl(3,index) = atan(sensor(3,index) ./ sensor(2,index) ); 
end    
index = find( sensor(2,:) > 0.0 & sensor(3,:) < 0.0 );
if (~isempty(index) )
  cyl(3,index) = 2*pi*ones(1,length(index)) + ...
                 atan(sensor(3,index) ./ sensor(2,index) ); 
end
index = find( sensor(2,:) < 0.0 & sensor(3,:) > 0.0 );   
if (~isempty(index) )
  cyl(3,index) = pi*ones(1,length(index)) + ...
                 atan(sensor(3,index) ./ sensor(2,index) ); 
end   
index = find( sensor(2,:) < 0.0 & sensor(3,:) < 0.0 );   
if (~isempty(index) )
  cyl(3,index) = pi*ones(1,length(index)) + ...
                 atan(sensor(3,index) ./ sensor(2,index) ); 
end   
