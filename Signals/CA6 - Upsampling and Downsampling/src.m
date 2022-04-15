function [y,fp,fm,Fs] = src(x,fp,fm,U,D,Fs)
% x = input signal
% fp = passing frequency
% fm = max frequency
% U = upsample rate
% D = downsample rate
% Fs = sample rate

if size(x,1) == 1
   x = transpose(x); 
end

% upsample the input
xu = zeros(U*size(x,1),1);
xu(1:U:end) = x;


%create the filter

% Filter Specification
%fstop = min([(1-fm)/U,1/(2*D)]);
fm = min([(D/U)*fm,1/2]);
% Filter order
N = 101; % Length of filter impulse response (odd integer)
% FIR Filter Design Procedure
f1 = (fm + fp)/2;
f2 = (fm - fp)/2;
L = (N-1)/2;
n = [-L:L];
h = (sin(2*pi*f1*n) .* sin(2*pi*f2*n)) ./ ((pi*n) .* (2*pi*f2*n));
h(L+1) = 2*f1;


%filter the sampled input

for n = 1:length(xu)
   z(n) = myFIRfilter(h,xu(n)); 
end

    % Calculate new vars

fp = (D/U)*fp;

Fs = (U/D)*Fs;

sigs = [fp, fm]
Fs

% downsample the input
p = zeros(size(x));
p(1:D:end) = 1;
z = x.*p;
% plot(abs(fftshift(fft(z,2^16)))); hold on;
% shg;
y = z(1:D:end);
plot(abs(fftshift(fft(y,2^16))));% hold off;
shg;
end