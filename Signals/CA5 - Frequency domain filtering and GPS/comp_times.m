P = 2^20;
[x,Fs] = audioread('Demons anechoic_01.wav');
N = length(x);

% Read in input signal
[h,Fh] = audioread('Impulse gym_01.wav');

L = length(h);

% Convolve using custom convolution function
tic,
y = myconv(x,h);
toc
M = N+L-1;



% Visualize signals in time domain
figure();
t = [0:M-1]/Fs; % Time vector [seconds]
plot(t,y); hold on;
t = [0:N-1]/Fs; % Time vector [seconds]
plot(t,x); hold off;

xlabel('Time [seconds]');
ylabel('Amplitude');
legend('Input x[n]','Output y[n]');
title('Input & Output Signals');
grid on;
shg;


% Convolve using frequency domain multiplication
tic,
yf = real(ifft(fft(x,P).*fft(h,P)));
toc

% Visualize signals in time domain
figure();
tf = [0:M-1]/Fs; % Time vector [seconds]
plot(yf); hold on;
tf = [0:N-1]/Fs; % Time vector [seconds]
plot(tf,x); hold off;

xlabel('Time [seconds]');
ylabel('Amplitude');
legend('Input x[n]','Output y[n]');
title('Input & Output Signals');
grid on;
shg;