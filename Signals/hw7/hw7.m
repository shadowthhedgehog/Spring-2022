syms z;
F = (1 - 1.5*z^(-1) + .5*z^(-2))/(2 - 2*z^(-1) + .375*z^(-2));
iztrans(F)

% zplane([1 -.5], [1, -.25])
% 
% ztrans((1-.5^n)*heaviside(n))

%[r,p,k] = residuez([],[])