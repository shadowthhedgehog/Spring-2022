[x,Fs] = audioread('galway11_mono_45sec.wav');
N = length(x);

% Read in input signal
% [h,Fh] = audioread('Impulse gym_01.wav');
% N = length(x);
% 
% L = length(h);

% % Convolve using custom convolution function
% y = myconv(x,h);
% M = N+L-1;
% 
% % Write out the audio signal
% audiowrite('wet_demons.wav',y,Fs);
% 
% % Visualize the filter impulse response
% figure();
% plot([0:L-1],h,'LineWidth',2);
% xlabel('Sample Index');
% ylabel('Amplitude');
% title('Impulse Response');
% grid on;
% shg;
% print -dpng impulse_response.png
% 
% % Visualize the filter frequency response
% NFFT = 2^14;
% F = Fs * ([0:NFFT-1]/NFFT - 0.5); % Frequency vector [Hertz]
% H = fftshift(fft(h,NFFT)); % Compute Fourier transform
% figure();
% plot(F,20*log10(abs(H)),'LineWidth',2); % Plot magnitude response in dB
% axis([0, 1200, -100, 10]); % Set axis limits
% xlabel('Frequency [Hertz]');
% ylabel('Magnitude [dB]');
% title('Frequency Response');
% grid on;
% shg;
% print -dpng magnitude_response.png

% % Visualize signals in time domain
% figure();
% t = [0:M-1]/Fs; % Time vector [seconds]
% plot(t,y); hold on;
% t = [0:N-1]/Fs; % Time vector [seconds]
% plot(t,x); hold off;
% 
% xlabel('Time [seconds]');
% ylabel('Amplitude');
% legend('Input x[n]','Output y[n]');
% title('Input & Output Signals');
% grid on;
% shg;
% print -dpng audio_timedomain.png

% Plot spectrograms of input and output signals
figure();
[S,F,T] = spectrogram(x,hamming(NFFT),round(0.9*NFFT),NFFT,Fs);
imagesc(T,F,20*log10(abs(S)),[0 50]);
colorbar;
set(gca,'YDir','normal');
ylim([0 5500]);
xlabel('Time [seconds]');
ylabel('Frequency [Hertz]');
title('Spectrogram of the Input x[n]');
grid on;

% subplot(212);
% [S,F,T] = spectrogram(y,hamming(NFFT),round(0.9*NFFT),NFFT,Fs);
% imagesc(T,F,20*log10(abs(S)),[0 50]);
% colorbar;
% set(gca,'YDir','normal');
% ylim([0 1500]);
% xlabel('Time [seconds]');
% ylabel('Frequency [Hertz]');
% title('Spectrogram of the Output y[n]');
% grid on;
% shg;
% orient tall;
% print -dpng audio_spectrograms.png