% Set up some important variables

Fs = 4e6; % Sample rate of data [samples/second]

% GPS matched filter impulse responses
prn_code_id = [1:37]; %#ok<*NBRAK> % PRN code IDs for GPS satellites (32 + extras = 37)
for i=1:length(prn_code_id)
  mf(:,i) = build_matched_filter(prn_code_id(i),Fs);
end
[Nh,~] = size(mf); % Get the length of the matched filter impulse response

% Parameters for fast convolution
N = 2^17; % FFT size [samples]
MF = fft(mf,N); % Compute zero-padded DFT of matched filters for frequency-domain processing

% Parameters for doppler frequency search
Fmax = 8000; % Maximum doppler frequency [Hz]
Fmin = -Fmax; % Minimum doppler frequency [Hz]
Fbin = Fs/N; % FFT bin width [Hz]
KFmax = ceil(Fmax/Fbin); % Number of frequency bins (positive side)
KFmin = floor(Fmin/Fbin); % Number of frequency bins (negative side)
ksearch = [KFmin:KFmax]; % Set of bins for doppler frequency sesarch


% Read gps.dat
fid = fopen('gps.dat','rb'); % Open the file for binary read
Nx = N - Nh + 1; % Number of data samples so that Nx + Nh - 1 = N
x = fread(fid,2*Nx,'int16=>float32'); % Read samples and convert from short to float
fclose(fid); % Close the file
x = complex(x(1:2:end), x(2:2:end)); % Convert interleaved real-imaginary to complex


% Transform and search

X = fft(x,N); % Compute zero-padded DFT of samples in x

% Begin search
for i=1:size(MF,2) % For loop over satellites
  for k=1:length(ksearch) % For loop over cyclic shifts (doppler frequency search)
      y(ksearch(k)) = ifft(X(ksearch(k))*MF(ksearch(k)));
    % a. Implement cyclic shift
    % b. Perform convolution in the frequency domain
    % c. Inverse transform to the time domain
    % [Hint: Can to a, b, and c in 1 line of Matlab code.]
    % d. Trim the startup and ending convolution transients [1 line of code]
    yt = y(1:Nx+Nh-1); % trim and print
    % e. Plot result and look for peak, record peak delay and doppler offset
    plot(Fs,yt);
  end
end