function y = myIIRfilter(b,a,x)
persistent zx zy Lx Ly;

if isempty(zx)
   Lx = length(b);
   Ly = length(a);
   zx = zeros(size(b));
   zy = zeros(size(a));
end


y = 0;

for n = Lx:-1:2
   zx(n) = zx(n-1); 
   y = y + b(n) * zx(n); 
end
zx(1) = x;
y = y + b(1) * zx(1);




for n = Ly:-1:2
   zy(n) = zy(n-1); 
   y = y - a(n) * zy(n); 
end
zy(1) = y;

end

