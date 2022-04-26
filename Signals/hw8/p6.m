
% Design derivative and delay filter
L = 22; % Filter delay
n = [-L:L].'; % Time vector
deriv = (-1).^n ./ n; % Derivative filter impulse response
deriv(L+1) = 0; % Fix the zero in the center
deriv = deriv.*hamming(2*L+1); % Include the Hamming window
delay = zeros(2*L+1,1); % Make a delay filter
delay(L+1) = 1; % Set the delay


%plot the derivative and delay filter impulse response
t = [0:length(deriv)-1] - (floor(length(deriv)/2));
figure();
subplot(211);
plot(t,deriv); hold on;
plot(t,delay); hold off;
xlabel('Sample index');
ylabel('Amplitude');
title('Impulse response h[n]');
legend('Derivative','Delay');
shg;

t = [0:length(deriv)-1]/length(deriv) - 0.5;
subplot(212);
plot(t,fftshift(abs(fft(deriv)))); hold on;
plot(t,fftshift(abs(fft(delay)))); hold off;
xlabel('Frequency [samples/cycle]');
ylabel('Magnitude [linear]');
title('Magnitude response H(f)');
legend('Derivative','Delay');
shg;