 t = [-20:1:20];
 h = zeros(size(t));
 x = zeros(size(t));
 Nh = 41;
 for i = 1:31
    h(i) = 1.1^(t(i)); 
 end
 
 for n = 14:41
    x(n) = 1; 
 end
 
 y = conv(x,h);
 %y = y(Nh:2*Nh-1); % trim
 stem(y);
 shg;