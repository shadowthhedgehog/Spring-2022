% script for CA6 to change sample rate from 11025 to 8000 via 5 stages

[x,Fs] = audioread('galway11_mono_45sec.wav');

%   Stage 1: U=2, D=1
[y, fp, fm, Fs] = src(x,0.3447,0.5,2,1,Fs);

% [p,f] = pspectrum(y,'TwoSided',true);
% plot(f/pi, abs(p));

%  Stage 2: U=2, D=3
[y, fp, fm, Fs] = src(y,fp,fm,2,3,Fs);

% [p,f] = pspectrum(y,'TwoSided',true);
% plot(f/pi, abs(p));

% Stage 3: U=4, D=3
[y, fp, fm, Fs] = src(y,fp,fm,4,3,Fs);

% [p,f] = pspectrum(y,'TwoSided',true);
% plot(f/pi, abs(p));

% Stage 4: U=4, D=7
[y, fp, fm, Fs] = src(y,fp,fm,4,7,Fs);
% 
% [p,f] = pspectrum(y,'TwoSided',true);
% plot(f/pi, abs(p));

% Stage 5: U=5, D=7
[y, fp, fm, Fs] = src(y,fp,fm,5,7,Fs);

% [p,f] = pspectrum(y,'TwoSided',true);
% plot(f/pi, abs(p));

audiowrite('outputca6.wav',y,floor(Fs));