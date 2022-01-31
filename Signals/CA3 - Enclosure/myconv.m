function y = myconv(x,h)
N = length(x); % Length of x[n]
L = length(h); % Length of h[n]

K = N+2*(L-1); % Length of zero-added x[n]
xzp = zeros(K,1); % Allocate memory
xzp(L:L+N-1) = x; % Zero-pad x[n] on both sides

M = N+L-1; % Length of output sequence
y = zeros(M,1); % Allocate memory
for n=1:M % Loop over output times
    for k=1:L % Multiply-accumulate loop
        y(n) = y(n) + xzp(L+(n-k))*h(k); % <= Array indexes?
    end
end
end