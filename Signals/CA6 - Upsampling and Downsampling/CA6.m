% script for CA6 to change sample rate from 11025 to 8000 via 5 stages
NFFT = 2^10;
[x,Fs] = audioread('galway11_mono_45sec.wav');

% D/U = 1/2
[y, fp, fm, Fs] = src(x,0.3447,0.5,2,1,Fs);

[p,f] = pspectrum(y);
plot(f/pi, abs(p));

% D/U = 3/2
[y, fp, fm, Fs] = src(y,fp,fm,3,2,Fs);

[p,f] = pspectrum(y);
plot(f/pi, abs(p));

% D/U = 3/4
[y, fp, fm, Fs] = src(y,fp,fm,4,3,Fs);

[p,f] = pspectrum(y);
plot(f/pi, abs(p));

% D/U = 7,4
[y, fp, fm, Fs] = src(y,fp,fm,4,7,Fs);

[p,f] = pspectrum(y);
plot(f/pi, abs(p));

% D/U = 7/5
[y, fp, fm, Fs] = src(y,fp,fm,5,7,Fs);

[p,f] = pspectrum(y);
plot(f/pi, abs(p));

audiowrite('outputca6',y,Fs);