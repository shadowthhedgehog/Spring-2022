Fs = 2.4e6; % [samples/second] = sample rate (given on web page and in filename)
Fc = 106.7e6; % [Hz] = center frequency (given on web page)
Nsec = 5; % [seconds] = number of seconds of data to read in
Ns = Nsec*Fs; % Number of samples to read in
fid = fopen('fm_rds_2400k_complex2','rb'); % Open the file
x = fread(fid,2*1e5,'float'); % Toss first 10^5 samples (they can have unwanted transients)
x = fread(fid,2*Ns,'float'); % Read in the samples you want
fclose(fid); % Close the file
x = complex(x(1:2:end),x(2:2:end)); % Convert to complex I/Q


%p=fmdemod(y,fc,fs,(fc-fm));


%Part 4
Fd = 1/3
%x = x.*exp(-1i*2*pi*(106.7e6 - 107.5e6));
x1 = x.*exp(-1i*2*pi*Fd*Ns);
%End part 4     X(:,1)



NFFT = 2^18; % FFT size, this may not be all the data and that's ok
fx = [0:NFFT-1]/NFFT - 0.5; % Normalized frequency (if NFFT is an even number)
Fx = fx*Fs + Fc; % True frequencies in Hz
X = abs(fftshift(fft(x1,NFFT))); % Compute the spectrum


subplot(211); plot(Fx/1e6,20*log10(X)); % Plot in MHz
title('FM signal on true freq axis');
xlabel('Frequency [Mhz]');
ylabel('Magnitude');
subplot(212); plot(fx,20*log10(X)); % Plot in normalized frequency
title('FM signal on scaled freq axis');
xlabel('Frequency');
ylabel('Magnitude');



