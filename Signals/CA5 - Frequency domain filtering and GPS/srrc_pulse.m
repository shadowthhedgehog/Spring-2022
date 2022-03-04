function p = srrc_pulse(t,eb)
Tc = 1/1.023e6;
u = (4*eb)*t;
num = sin((pi*(1-eb))*t) + u.*cos((pi*(1+eb)).*t);
den = pi*t.*(1-u.^2);
p = num./den;
p(t==0) = 1+(4/pi-1)*eb;
p(abs(t)==(1/(4*eb))) = (eb/sqrt(2)) * ((1+2/pi)*sin(pi/(4*eb)) + (1-2/pi)*cos(pi/(4*eb)));
if(any(isnan(p)))
  fprintf('ERROR: Divide by zero in srrc_pulse.\n');
end