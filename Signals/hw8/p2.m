

NFFT = 2^14;
freq = [0:NFFT-1]/NFFT - 0.5;

fp = 0.24;
fs = 0.26;
Ap = 0.1; % dB
As = 60; % dB

Bp = 10^(Ap/20);
dp = (Bp-1)/(Bp+1); % pass band ripple
Bs = (1+dp)*10^(-As/20);
ds = Bs; % stop band ripple (approx.)

%% Windowed filter design
Fw = fs - fp;
fc = 0.5*(fs + fp);
[d,dind] = min([dp ds]);
if(dind == 1)
    A = Ap; % design is dominated by pass band ripple
else
    A = As; % design is dominated by stop band ripple
end;
% Blackman window cuz 74 dB
L = ceil(11*pi/(2*pi*Fw));
% Now we know the shape (Blackman) and the length.
win = blackman(L);
% Generate the impulse response
M = L-1; % filter order
alpha = M/2; % group delay
n = [0:L-1].' - alpha;
hd = (2*fc)*sinc(2*fc*n);
h = hd.*win;
hblackman = h;

% plot(freq,20*log10(abs(fftshift(fft(hd,NFFT)))),'Color',[0.8 0 0.3],'LineWidth',3);
% hold on;
% plot(freq,20*log10(abs(fftshift(fft(h ,NFFT)))),'Color',[0.0 0.8 0.3],'LineWidth',3);
% ax = axis;
% plot((fp)*[1 1],ax(3:4),'Color',[0 0.5 0.5],'LineWidth',2);
% plot(-(fp)*[1 1],ax(3:4),'Color',[0 0.5 0.5],'LineWidth',2);
% plot((fs)*[1 1],ax(3:4),'Color',[0 0.5 0.5],'LineWidth',2);
% plot(-(fs)*[1 1],ax(3:4),'Color',[0 0.5 0.5],'LineWidth',2);
% hold off;
% grid on;
% xlabel('Normalized Frequency [cyles/sample]');
% ylabel('Magnitude [dB]');
% legend('Non-Windowed','Windowed');
% ylim([-100 10]);



plot(freq,unwrap(angle(fftshift(fft(hd,NFFT)))),'Color',[0.8 0 0.3],'LineWidth',3);
hold on;
plot(freq,unwrap(angle(fftshift(fft(h ,NFFT)))),'Color',[0.0 0.8 0.3],'LineWidth',3);
ax = axis;
plot((fp)*[1 1],ax(3:4),'Color',[0 0.5 0.5],'LineWidth',2);
plot(-(fp)*[1 1],ax(3:4),'Color',[0 0.5 0.5],'LineWidth',2);
plot((fs)*[1 1],ax(3:4),'Color',[0 0.5 0.5],'LineWidth',2);
plot(-(fs)*[1 1],ax(3:4),'Color',[0 0.5 0.5],'LineWidth',2);
hold off;
grid on;
xlabel('Normalized Frequency [cyles/sample]');
ylabel('Magnitude');
legend('Non-Windowed','Windowed');
ylim([-100 10]);







%% Kaiser design
[M,Wn,beta,filtype]=kaiserord([fp fs],[1 0],[dp ds],1)
%M = 60;
L = M+1;
alpha = M/2;
win = kaiser(L,beta);
n = [0:L-1].' - alpha;
hd = (2*fc)*sinc((2*fc)*(n));
h = hd.*win;
hkaiser = h;

plot(freq,unwrap(angle(fftshift(fft(hd,NFFT)))),'Color',[0.8 0 0.3],'LineWidth',3);
hold on;
plot(freq,unwrap(angle(fftshift(fft(h ,NFFT)))),'Color',[0.0 0.8 0.3],'LineWidth',3);
ax = axis;
plot((fp)*[1 1],ax(3:4),'Color',[0 0.5 0.5],'LineWidth',2);
plot(-(fp)*[1 1],ax(3:4),'Color',[0 0.5 0.5],'LineWidth',2);
plot((fs)*[1 1],ax(3:4),'Color',[0 0.5 0.5],'LineWidth',2);
plot(-(fs)*[1 1],ax(3:4),'Color',[0 0.5 0.5],'LineWidth',2);
hold off;
grid on;
xlabel('Normalized Frequency [cyles/sample]');
ylabel('Magnitude');
legend('Non-Windowed','Windowed');
ylim([-100 10]);

