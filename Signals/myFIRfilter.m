function y = myFIRfilter(b,x)
persistent z L;

if isempty(z)
   L = length(b);
   z = zeros(size(b));
end

y = 0;

%shift
for n = L:-1:2
   z(n) = z(n-1);
   y = y + b(n) * z(n);
end
z(1) = x;
y = y + b(1) * x;
end

