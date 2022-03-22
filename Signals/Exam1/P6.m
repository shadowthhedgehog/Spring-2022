f = [-0.5:0.001:0.5];
H = @(f) (2 - 1.5371*exp(-1i*2*pi*f)) ./ (1 - 1.5371*exp(-1i*2*pi*f) + .9025*exp(-1i*4*pi*f));
Y = @(f) (2 - 1.5371*exp(-1i*2*pi*f));
X = @(f) (1 - 1.5371*exp(-1i*2*pi*f) + .9025*exp(-1i*4*pi*f));
plot(f,abs(H(f))); hold on;
plot(f,angle(H(f))); hold off;
title('H(f)');
legend('|H(f)|','<H(f)');
xlabel('Frequency [Hz]');
ylabel('Magnitude');
shg;