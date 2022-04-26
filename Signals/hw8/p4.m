

NFFT = 2^14;
freq = [0:NFFT-1]/NFFT - 0.5;

fp = 150/2e4;
fs = 300/2e4;
Ap = 40; % dB
As = 40; % dB

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
% Hann window cuz 44 dB
L = ceil(6.2*pi/(2*pi*Fw));
M = L-1; % filter order
if(mod(M,2)==1)
    M = M+1;
    L = L+1;
end
% Now we know the shape (Hann) and the length.
win = hanning(L);
% Generate the impulse response
alpha = M/2; % group delay
n = [0:L-1].' - alpha;
hd = (3*2*fc)*sinc(2*fc*n);
hd(alpha+1) = hd(alpha+1) + 1;
h = hd.*win;
hhamming = h;

%magnitude dB
plot(freq,20*log10(abs(fftshift(fft(hd,NFFT)))),'Color',[0.8 0 0.3],'LineWidth',3);
hold on;
plot(freq,20*log10(abs(fftshift(fft(h ,NFFT)))),'Color',[0.0 0.8 0.3],'LineWidth',3);
ax = axis;
plot((fp)*[1 1],ax(3:4),'Color',[0 0.5 0.5],'LineWidth',2);
plot(-(fp)*[1 1],ax(3:4),'Color',[0 0.5 0.5],'LineWidth',2);
plot((fs)*[1 1],ax(3:4),'Color',[0 0.5 0.5],'LineWidth',2);
plot(-(fs)*[1 1],ax(3:4),'Color',[0 0.5 0.5],'LineWidth',2);
hold off;
grid on;
xlabel('Normalized Frequency [cyles/sample]');
ylabel('Magnitude [dB]');
legend('Non-Windowed','Windowed');
ylim([-100 10]);

%magnitude linear
plot(freq,(abs(fftshift(fft(hd,NFFT)))),'Color',[0.8 0 0.3],'LineWidth',3);
hold on;
plot(freq,(abs(fftshift(fft(h ,NFFT)))),'Color',[0.0 0.8 0.3],'LineWidth',3);
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
ylim([-.5 1.5]);


%phase
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
if(mod(M,2)==1)
    M = M+1; 
end
L = M+1;
alpha = M/2;
win = kaiser(L,beta);
n = [0:L-1].' - alpha;
hd = (3*2*fc)*sinc(2*fc*n);
hd(alpha+1) = hd(alpha+1) + 1;
h = hd.*win;
hkaiser = h;

plot(freq,20*log10(abs(fftshift(fft(hd,NFFT)))),'Color',[0.8 0 0.3],'LineWidth',3);
hold on;
plot(freq,20*log10(abs(fftshift(fft(h ,NFFT)))),'Color',[0.0 0.8 0.3],'LineWidth',3);
ax = axis;
plot((fp)*[1 1],ax(3:4),'Color',[0 0.5 0.5],'LineWidth',2);
plot(-(fp)*[1 1],ax(3:4),'Color',[0 0.5 0.5],'LineWidth',2);
plot((fs)*[1 1],ax(3:4),'Color',[0 0.5 0.5],'LineWidth',2);
plot(-(fs)*[1 1],ax(3:4),'Color',[0 0.5 0.5],'LineWidth',2);
hold off;
grid on;
xlabel('Normalized Frequency [cyles/sample]');
ylabel('Magnitude [dB]');
legend('Non-Windowed','Windowed');
ylim([-100 10]);

plot(freq,(abs(fftshift(fft(hd,NFFT)))),'Color',[0.8 0 0.3],'LineWidth',3);
hold on;
plot(freq,(abs(fftshift(fft(h ,NFFT)))),'Color',[0.0 0.8 0.3],'LineWidth',3);
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
ylim([-.25 1.2]);


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






%% Equiripple filter design
[M,F,A,W]=firpmord([fp fs],[4 1],[dp ds],1)
%M = M + 2;
h = firpm(M,F,A,W);
hfirpm = h;

plot(freq,20*log10(abs(fftshift(fft(hhamming,NFFT)))),'Color',[0.8 0 0.3],'LineWidth',3);
hold on;
plot(freq,20*log10(abs(fftshift(fft(hkaiser ,NFFT)))),'Color',[0.0 0.8 0.3],'LineWidth',3);
plot(freq,20*log10(abs(fftshift(fft(hfirpm  ,NFFT)))),'Color',[0.0 0.3 0.8],'LineWidth',3);
ax = axis;
plot((fp)*[1 1],ax(3:4),'Color',[0 0.5 0.5],'LineWidth',2);
plot(-(fp)*[1 1],ax(3:4),'Color',[0 0.5 0.5],'LineWidth',2);
plot((fs)*[1 1],ax(3:4),'Color',[0 0.5 0.5],'LineWidth',2);
plot(-(fs)*[1 1],ax(3:4),'Color',[0 0.5 0.5],'LineWidth',2);
hold off;
grid on;
xlabel('Normalized Frequency [cyles/sample]');
ylabel('Magnitude [dB]');
legend('Hamming','Kaiser','FIR-PM');
ylim([-100 10]);

fprintf('Hamming = %d\nKaiser  = %d\nFIR-PM  = %d\n',...
        length(hhamming),length(hkaiser),length(hfirpm));

    
    
    
    
    
    
    
    
plot(freq,(abs(fftshift(fft(hhamming,NFFT)))),'Color',[0.8 0 0.3],'LineWidth',3);
hold on;
plot(freq,(abs(fftshift(fft(hkaiser ,NFFT)))),'Color',[0.0 0.8 0.3],'LineWidth',3);
plot(freq,(abs(fftshift(fft(hfirpm  ,NFFT)))),'Color',[0.0 0.3 0.8],'LineWidth',3);
ax = axis;
plot((fp)*[1 1],ax(3:4),'Color',[0 0.5 0.5],'LineWidth',2);
plot(-(fp)*[1 1],ax(3:4),'Color',[0 0.5 0.5],'LineWidth',2);
plot((fs)*[1 1],ax(3:4),'Color',[0 0.5 0.5],'LineWidth',2);
plot(-(fs)*[1 1],ax(3:4),'Color',[0 0.5 0.5],'LineWidth',2);
hold off;
grid on;
xlabel('Normalized Frequency [cyles/sample]');
ylabel('Magnitude');
legend('Hamming','Kaiser','FIR-PM');
ylim([-100 10]);





plot(freq,unwrap(angle(fftshift(fft(hhamming,NFFT)))),'Color',[0.8 0 0.3],'LineWidth',3);
hold on;
plot(freq,unwrap(angle(fftshift(fft(hkaiser ,NFFT)))),'Color',[0.0 0.8 0.3],'LineWidth',3);
plot(freq,unwrap(angle(fftshift(fft(hfirpm  ,NFFT)))),'Color',[0.0 0.3 0.8],'LineWidth',3);
ax = axis;
plot((fp)*[1 1],ax(3:4),'Color',[0 0.5 0.5],'LineWidth',2);
plot(-(fp)*[1 1],ax(3:4),'Color',[0 0.5 0.5],'LineWidth',2);
plot((fs)*[1 1],ax(3:4),'Color',[0 0.5 0.5],'LineWidth',2);
plot(-(fs)*[1 1],ax(3:4),'Color',[0 0.5 0.5],'LineWidth',2);
hold off;
grid on;
xlabel('Normalized Frequency [cyles/sample]');
ylabel('Magnitude');
legend('Hamming','Kaiser','FIR-PM');
ylim([-100 10]);
