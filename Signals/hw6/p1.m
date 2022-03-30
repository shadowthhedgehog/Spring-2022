x = [1,1,0,0,0,0,0,1];
h = [0,1,0,0,0,0,0,0];
N = length(x);
syms n;

X = exp(-1i*2*pi*(0:N-1)'*(0:N-1)/N)*x(:);

y = cconv(x,h,16)

yo = ifft(fft(x).*fft(h))