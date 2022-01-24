[x,Fs] = audioread('fireflyintro.wav');

q = exp(-20*pi/Fs);
Fn = 466.16;
fn = Fn/Fs;
w = 2*pi*fn;

b = [1, -2*cos(w), 1];
a = [1, -2*q*cos(w), q^2];




clear myIIRfilter;




% [H,F] = freqz(b,a,2^12,Fs);
% plot(F,20*log10(abs(H)),'LineWidth',2);
% xlim([0,3000]);
% grid on;
% xlabel('Frequency [Hz]');
% ylabel('Magnitude [dB]');
% shg;


out = zeros(size(x));

for n = 1:length(x)
     out(n) = myIIRfilter(b,a,x(n));
%    out(n) = exampleIIRfilter(b,a,x(n));
end


audiowrite('firefly_notch_output.wav',out,Fs);



NFFT = 2^14;


% Plot spectrograms of input and output signals
figure();
subplot(211);
[S,F,T] = spectrogram(x,hamming(NFFT),round(0.9*NFFT),NFFT,Fs);
imagesc(T,F,20*log10(abs(S)),[0 50]);
colorbar;
set(gca,'YDir','normal');
ylim([0 1500]);
xlabel('Time [seconds]');
ylabel('Frequency [Hertz]');
title('Spectrogram of the Input x[n]');
grid on;

subplot(212);
[S,F,T] = spectrogram(out,hamming(NFFT),round(0.9*NFFT),NFFT,Fs);
imagesc(T,F,20*log10(abs(S)),[0 50]);
colorbar;
set(gca,'YDir','normal');
ylim([0 1500]);
xlabel('Time [seconds]');
ylabel('Frequency [Hertz]');
title('Spectrogram of the Output y[n]');
grid on;
shg;
orient tall;
print -dpng audio_spectrograms.png