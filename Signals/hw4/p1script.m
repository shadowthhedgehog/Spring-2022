% Problem 3-2

%h = inline(1 + exp(-1i*2*pi*f) + exp(-1i*2*pi*f*2));

% t = [-0.5:0.01:1.5];
% plot(t,abs(h)); hold on;
% plot(t,angle(h)); 
% xline([0 (pi/(3*2*pi)) (pi/(2*2*pi)) ((2*pi)/(3*2*pi)) (pi/(2*pi))],'--b'); hold off;
% title('H(f)');
% legend('|H(f)|','<H(f)');
% xlabel('Frequency [Hz]');
% ylabel('Magnitude');
% shg;


% Problem 3-6

% func =@(t) (exp(1i*t) - exp(-1i*4*t));
% t = [-5:0.01:5];
% plot(t,abs(func(t))); hold on;
% plot(t,angle(func(t))); hold off;
% title('H(w)');
% legend('|H(w)|','<H(w)');
% xlabel('Frequency [w]');
% ylabel('Magnitude');
% shg;




%problem 3-15

% func =@(t) (1 - 2*exp(-1i*t) + exp(-1i*2*t));
% t = [-pi:0.01:pi];
% plot(t,abs(func(t))); hold on;
% plot(t,angle(func(t))); hold off;
% title('H(w)');
% legend('|H(w)|','<H(w)');
% xlabel('Frequency [w]');
% ylabel('Magnitude');
% shg;