% HW1 question 1
T = 10;
F = [-2:0.01:2];
S = T*sinc(F*T);
subplot(321);
plot(F,S,'LineWidth',2);
grid on;
xlabel('Frequency (Hz)');
ylabel('T sinc(FT)');
subplot(322);
plot(F,20*log10(abs(S)),'LineWidth',2);
grid on;
ylim([-40,0]);
xlabel('Frequency (Hz)');
ylabel('20 log_{10}|T sinc(FT)| dB');
orient landscape;


% HW1 question 2
T = 10;
F = [0:0.01:1];
S = T*sinc(F*T).*(exp(-1i*pi*F*T));
subplot(323);
plot(F,S,'LineWidth',2);
hold on;
plot(F,imag(S),'LineWidth',2);
hold off;
grid on;
xlabel('Frequency (Hz)');
ylabel('T sinc(FT)');
subplot(324);
plot(F,20*log10(abs(S)),'LineWidth',2);
hold on;
plot(F,imag(20*log10(abs(S))),'LineWidth',2);
hold off;
grid on;
ylim([-40,0]);
xlabel('Frequency (Hz)');
ylabel('20 log_{10}|T sinc(FT)| dB');
orient landscape;


% HW1 question 3
T = 5;
F = [-1:0.01:1];
S = T*sinc(F*T).*((exp(15*1i*pi*F))+(exp(-15*1i*pi*F)));
subplot(325);
plot(F,S,'LineWidth',2);
hold on;
plot(F,imag(S),'LineWidth',2);
hold off;
grid on;
xlabel('Frequency (Hz)');
ylabel('T sinc(FT)');
subplot(326);
plot(F,20*log10(abs(S)),'LineWidth',2);
hold on;
plot(F,imag(20*log10(abs(S))),'LineWidth',2);
hold off;
grid on;
ylim([-40,0]);
xlabel('Frequency (Hz)');
ylabel('20 log_{10}|T sinc(FT)| dB');
orient landscape;