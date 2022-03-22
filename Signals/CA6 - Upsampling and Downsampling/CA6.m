% script for CA6 to change sample rate from 11025 to 8000 via 5 stages

[x,Fs] = audioread('galway11_mono_45sec.wav');

plot(x); hold on;

[y, fp, fm, Fs] = src(x,0.3447,0.5,2,1,Fs);

plot(y); hold off;


plot(abs(fftshift(fft(x,2^16)))/Fs); hold on;
plot(abs(fftshift(fft(y,2^16)))/Fs); hold off;