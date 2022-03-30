
% part A
% x = cos((2*pi/8)*[0:15]);
% 
% figure()
% t = [0:15/2^8:15];
% plot(t(1:2^8),abs(fft(x,2^8))); hold on;
% plot(t(1:2^8),angle(fft(x,2^8))); hold off;
% title('DFT of X');
% legend('|X[k]|','<X[k]');
% xlabel('Time');
% ylabel('Magnitude');
% shg;


% part B
% x = sin((3*pi/8)*[0:15]);
% 
% figure()
% t = [0:15/2^8:15];
% plot(t(1:2^8),abs(fft(x,2^8))); hold on;
% plot(t(1:2^8),angle(fft(x,2^8))); hold off;
% title('DFT of X');
% legend('|X[k]|','<X[k]');
% xlabel('Time');
% ylabel('Magnitude');
% shg;


% part C
% x = -cos((pi/2)*[0:15]);
% 
% figure()
% t = [0:15/2^8:15];
% plot(t(1:2^8),abs(fft(x,2^8))); hold on;
% plot(t(1:2^8),angle(fft(x,2^8))); hold off;
% title('DFT of X');
% legend('|X[k]|','<X[k]');
% xlabel('Time');
% ylabel('Magnitude');
% shg;




%later question
% Fs = 8000;
% L = 100;
% t = [0:1/Fs:L/Fs];
% xt = cos(2*pi*220*t);
% x  = xt.*t;
% 
% freq = Fs*(0:L/2)/L;
% 
% y = fft(x,101);
% 
% y = y(1:L/2+1);
% 
% plot(freq, abs(y), 'b'); hold on;
% plot(-freq, abs(y), 'b'); hold off;
% title('101 point DFT');
% legend('|X[k]|');
% xlabel('Freq [Hz]');
% ylabel('Magnitude');
% shg;





%even later questionn

clear x; clear h;
x = [0:9]; h = [0:4];% M = 5;

y = conv(x,h)
cy = cconv(x,h,14);
K = length(y);


fftconv = ifft(fft(x,2^4).*fft(h,2^4))

stem(y);
title('y[n]');
xlabel('index');
ylabel('Magnitude');


