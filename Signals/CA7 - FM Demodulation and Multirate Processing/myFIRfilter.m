function y = myFIRfilter(b,x)
%MYFIRFILTER Summary of this function goes here
%   Detailed explanation goes here

persistent z L;

if isempty(z)
   L = length(b);
   z = zeros(size(b));
end

y = 0;

for n = L:-1:2
   z(n) = z(n-1);
   y = y + b(n) * z(n);
end
z(1) = x;
y = y + b(1) * z(1);
end

