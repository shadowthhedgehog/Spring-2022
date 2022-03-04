function c = gps_waas_code(id)
% INPUTS: id = index of GPS code or WAAS code
% GPS  codes have IDs from 1 to 37
% WAAS codes have IDs from 120 to 138
[fid,msg]=fopen('GPS_WAAS_PRN_CODES.DAT','rb');
if(fid==-1)
  fprintf('ERROR: GPS_WAAS_PRN_CODES.DAT file not found.\n');
  c = [];
  return;
end

NUM_CODE = fread(fid,1,'int');
codes = fread(fid,inf,'int');
codes = reshape(codes,1024,NUM_CODE);
fclose(fid);

ind = find(codes(1,:)==id);
if(length(ind)==0)
  fprintf('ERROR: Code ID %d not found\n',id);
  c = [];
  return;
end

c = codes(2:1024,ind);
return;