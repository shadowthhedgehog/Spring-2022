function mf = build_matched_filter(prn_code_id,Fs,eb)
% prn_code id = index of PRN code
%   1- 37 GPS
% 120-138 WAAS
%
% Fs = sample rate
%
% eb = excess bandwidth

if(nargin < 2 | isempty(Fs))
    Fs = 4e6; %50e6; % Default to 50 MHz
end

if(nargin < 3 | isempty(eb))
    eb = 0.3; % Default to 30% excess bandwidth
end

% Generate GPS and/or WAAS code
c = gps_waas_code(prn_code_id); % binary code
c = 2*c-1; % antipodal code

% Precompute the code matched filter
Tc = 1/1.023e6; % chip time for GPS and WAAS
Ts = 1/Fs;      % sample time
S = Ts/Tc;      % chips per sample

num_blocks = 1; % number of codes to concatenate
Nc = 1023;      % length of code for GPS and WAAS
Tcode = Nc*Tc;  % number of chips x time per chip = 1 msec for the code

% eb = 0.3;
Lp = 12; % Truncation length of pulse (use SRRC pulse)
L1 = -Lp;
L2 =  Lp;

i = [0:num_blocks*Tcode/Ts-1].';
x = zeros(length(i),1);
for ii = 1:length(i)
    nmin = floor(i(ii)*S-L2);
    nmax = ceil( i(ii)*S-L1);
    n = [nmin:nmax].';
    chips = c(mod(n,Nc)+1,:);
    p = srrc_pulse(i(ii)*S-n,eb);
    x(ii) = p.'*chips;
end

% Don't forget to flip the matched filters
mf = x(end:-1:1); % (should also conjugage if complex)
