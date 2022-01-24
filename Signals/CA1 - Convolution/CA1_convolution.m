% Input signal
N = 8;
x = [1:1:8];

% Impulse response
L = 4;
h = [1:1:4];

% Convolve - call custom convolution function
y = myconv(x,h);


% Visualize
M = N+L-1; % Compute length of output
n = [0:M-1]; % Construct a time sequence
stem(n,y,'LineWidth',2); % Plot
xlabel('Time');
ylabel('Output Amplitude');
title('Output Sequence y[n]');
grid on;
shg; % Show handle graphics


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