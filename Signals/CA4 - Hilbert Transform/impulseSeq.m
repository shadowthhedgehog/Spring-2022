clear myFIRfilter;

out = [];
x = [];

h = [.0625,.2764,.4182,.2764,.0625];
out = zeros(size(x));


for n = 1:length(x)
   out(n) = myFIRfilter(h,x(n));
end

out
plot(out)


Fs = 1; % samples/sec
[H,F] = freqz(out,1,2^14,Fs);
plot(F,abs(H),'LineWidth',2);
xlabel('Frequency [Hz]');
ylabel('Magnitude [dB]');