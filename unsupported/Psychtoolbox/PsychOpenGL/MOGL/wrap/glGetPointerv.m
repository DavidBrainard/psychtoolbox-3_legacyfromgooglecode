function params = glGetPointerv( pname )

% glGetPointerv  Interface to glGetPointerv
% 
% usage:  params = glGetPointerv( pname )
%
% C function:  void glGetPointerv (GLenum pname, GLvoid* *params);

% 01-Jan-2006 -- created (RFM)

% ---allocate---
% ---protected---

if nargin~=1,
    error('invalid number of arguments');
end

params=uint32(0);
moglcore( 'glGetPointerv', pname, params );

return