function [y,fp,fm,Fs] = src(x,fp,fm,U,D,Fs)
%SRC Summary of this function goes here
%   Detailed explanation goes here

% upsample the input
xu = zeros(U*size(x,1),1);
xu(1:U:end) = x;


%create the filter

% Filter Specification
% Filter order
N = 101; % Length of filter impulse response (odd integer)
% FIR Filter Design Procedure
f1 = (Fs + fm)/2;
f2 = (fm - fp)/2;
L = (N-1)/2;
n = [-L:L];
h = (sin(2*pi*f1*n) .* sin(2*pi*f2*n)) ./ ((pi*n) .* (2*pi*f2*n));
h(L+1) = 2*f1;



plot(abs(fft(h,2^16)));

%filter the sampled input

for n = 1:length(xu)
   y(n) = myFIRfilter(h,xu(n)); 
end

    % Calculate new vars
%fstop = min([(1-fm)/U,1/(2*D)]);
fp = (D/U)*fp;
fm = min([(D/U)*fm,1/2]);
Fs = (D/U)*Fs;

% downsample the input
% p = zeros(size(x));
% p(1:D:end) = 1;
% y = x.*p;
y = y(1:D:end);
end