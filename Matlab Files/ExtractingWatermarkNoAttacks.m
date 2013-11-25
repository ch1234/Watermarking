% This file extracts watermark from image that has not been attacked.
close all;clear all;clc;

% Read in estimated polarities... 
est_polarity1 = fread3('Stage 1 CB 16\Polarity1.Y',0,0,128,128)';
est_polarity2 = fread3('Stage 1 CB 16\Polarity2.Y',0,0,128,128)';
est_polarity3 = fread3('Stage 1 CB 16\Polarity3.Y',0,0,128,128)';

figure(1);imshow(est_polarity1,'InitialMagnification','fit');axis equal;shg;title 'Estimated Polarity 1';
figure(2);imshow(est_polarity2,'InitialMagnification','fit');axis equal;shg;title 'Estimated Polarity 2'; 
figure(3);imshow(est_polarity3,'InitialMagnification','fit');axis equal;shg;title 'Estimated Polarity 3';

% Read in keys...
key1 = fread3('C:\Users\Matthew\Documents\My Dropbox\SingleStageVQRobustWatermarks\SingleStageVQRobustWatermarks\Keys\CB 16\Key1.Y',0,0,128,128);
key2 = fread3('C:\Users\Matthew\Documents\My Dropbox\SingleStageVQRobustWatermarks\SingleStageVQRobustWatermarks\Keys\CB 16\Key2.Y',0,0,128,128);
key3 = fread3('C:\Users\Matthew\Documents\My Dropbox\SingleStageVQRobustWatermarks\SingleStageVQRobustWatermarks\Keys\CB 16\Key3.Y',0,0,128,128);

%figure(4);imshow(key1,'InitialMagnification','fit');axis equal;shg;title 'Key 1';
%figure(5);imshow(key2,'InitialMagnification','fit');axis equal;shg;title 'Key 2';
%figure(6);imshow(key3,'InitialMagnification','fit');axis equal;shg;title 'Key3';

% Performing XOR of key and estimated polarity should return original
% watermark.
est_watermark1 = xor(est_polarity1,key1);
est_watermark2 = xor(est_polarity2,key2);
est_watermark3 = xor(est_polarity3,key3);

figure(7);imshow(est_watermark1,'InitialMagnification','fit');axis equal;shg;title 'Estimated Watermark 1';
figure(8);imshow(est_watermark2,'InitialMagnification','fit');axis equal;shg;title 'Estimated Watermark 2'; 
figure(9);imshow(est_watermark3,'InitialMagnification','fit');axis equal;shg;title 'Estimated Watermark 3';

% Write est_watermarks to file...
fid1 = fopen('Watermarks\EstWatermark1.Y','w+');
fwrite(fid1,est_watermark1);
fid2 = fopen('Watermarks\EstWatermark2.Y','w+');
fwrite(fid2,est_watermark2);
fid3 = fopen('Watermarks\EstWatermark3.Y','w+');
fwrite(fid3,est_watermark3);

imwrite(est_watermark1, 'Watermarks\EstWatermark1.jpg','jpg','Quality',100);
imwrite(est_watermark2, 'Watermarks\EstWatermark2.jpg','jpg','Quality',100);
imwrite(est_watermark3, 'Watermarks\EstWatermark3.jpg','jpg','Quality',100);