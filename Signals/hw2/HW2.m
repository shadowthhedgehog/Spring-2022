% HW2 7a
T = [0:0.01:10];
S = exp(-1i*2*pi*.1*T);
plot(T,S,'LineWidth',2);
hold on;
plot(T,exp(-1i*2*pi*1.1*T),'LineWidth',2);
%hold off;
grid on;
xlabel('Time');
ylabel('Magnitude');
orient landscape;


% HW2 7b
n = [0:10];
S = exp(-1i*2*pi*.1*n);
stem(n,S,'LineWidth',2);
stem(n,exp(-1i*2*pi*1.1*n),'LineWidth',2);
hold off;
grid on;
orient landscape;


theta = [0:0.01:2*pi];
f = exp(1i*theta);
plot(real(f),imag(f));
