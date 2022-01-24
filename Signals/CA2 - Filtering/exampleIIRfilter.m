function y = exampleIIRfilter(b,a,x)
persistent zx zy Lx Ly;

if isempty(zx)
   Lx = length(b);
   Ly = length(a);
   zx = zeros(size(b));
   zy = zeros(size(a));
end


%shift all up 1 (bottom 2 values are duplicates
for n = Lx:-1:2
   zx(n) = zx(n-1); 
end
for n = Ly:-1:2
   zy(n) = zy(n-1);  
end
%overwrite the bottom of x array with x
zx(1) = x;



%clear y
y = 0;
% do all additions (bottom of y array still duplicate,
% so don't add the bottom one)
for n = 1:Lx
    y = y + b(n) * zx(n);
end
for n = 2:Ly
   y = y - a(n) * zy(n);
end
%now copy in y to bottom of y array
zy(1) = y;
end
