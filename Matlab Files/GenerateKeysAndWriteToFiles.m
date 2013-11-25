% This file generates the secret keys and writes them to a file.
close all;clear all;clc;

% Read in polarities from VQ'ed Lena...
p1 = fread3('Polarities\CB 16\Polarity1.Y',0,0,128,128);
p2 = fread3('Polarities\CB 16\Polarity2.Y',0,0,128,128);
p3 = fread3('Polarities\CB 16\Polarity3.Y',0,0,128,128);

figure(1);imshow(p1','InitialMagnification','fit');axis equal;shg;title 'Polarity 1';
figure(2);imshow(p2','InitialMagnification','fit');axis equal;shg;title 'Polarity 2';
figure(3);imshow(p3','InitialMagnification','fit');axis equal;shg;title 'Polarity 3';

% Reading in watermarks in standard format, convert to binary images and write to file...
% Only need to run this once.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%w1 = imread('Watermarks\Temp\campanile.jpg');
%w1 = imresize(w1, [128 128]);
%w1 = rgb2gray(w1);
%l1 = graythresh(w1);
%w1 = im2bw(w1,l1);

%fidw1 = fopen('Watermarks\Watermark1.Y','w+');
%fwrite(fidw1,w1);

%w2 = imread('Watermarks\Temp\einstein.jpg'); %already grayscale
%w2 = imresize(w2, [128 128]);
%l2 = graythresh(w2);
%w2 = im2bw(w2,l2);

%fidw2 = fopen('Watermarks\Watermark2.Y','w+');
%fwrite(fidw2,w2);

%w3 = imread('Watermarks\Temp\WatermarkNumber3.png');
%w3 = imresize(w3, [128 128]);
%w3 = rgb2gray(w3);
%l3 = graythresh(w3);
%w3 = im2bw(w3,l3);

%fidw3 = fopen('Watermarks\Watermark3.Y','w+');
%fwrite(fidw3,w3);
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

w1 = fread3('Watermarks\Watermark1.Y',0,0,128,128);
w2 = fread3('Watermarks\Watermark2.Y',0,0,128,128);
w3 = fread3('Watermarks\Watermark3.Y',0,0,128,128);

figure(4);imshow(w1,'InitialMagnification','fit');axis equal;shg;title 'Watermark1';
figure(5);imshow(w2,'InitialMagnification','fit');axis equal;shg;title 'Watermark2';
figure(6);imshow(w3,'InitialMagnification','fit');axis equal;shg;title 'Watermark3';

% Using XOR operation to generate keys and writing to file...
key1 = xor(p1',w1);
fid1 = fopen('Keys\CB 16\Key1.Y','w+');
fwrite(fid1,key1);

key2 = xor(p2',w2);
fid2 = fopen('Keys\CB 16\Key2.Y','w+');
fwrite(fid2,key2);

key3 = xor(p3',w3);
fid3 = fopen('Keys\CB 16\Key3.Y','w+');
fwrite(fid3,key3);

figure(7);imshow(key1,'InitialMagnification','fit');axis equal;shg;title 'Key 1';
figure(8);imshow(key2,'InitialMagnification','fit');axis equal;shg;title 'Key 2';
figure(9);imshow(key3,'InitialMagnification','fit');axis equal;shg;title 'Key 3';

% Applying XOR again retrieves watermark... rwi = Retrieved Watermark
rw1 = xor(p1',key1);
rw2 = xor(p2',key2);
rw3 = xor(p3',key3);

figure(10);imshow(rw1,'InitialMagnification','fit');axis equal;shg;title 'Extracted Watermark 1';
figure(11);imshow(rw2,'InitialMagnification','fit');axis equal;shg;title 'Extracted Watermark 2';
figure(12);imshow(rw3,'InitialMagnification','fit');axis equal;shg;title 'Extracted Watermark 3';

% Write to jpegs for report..
imwrite(p1', 'Polarities\CB 16\Mean Polarity.jpg','jpg', 'Quality', 100);
imwrite(p2', 'Polarities\CB 16\Var Polarity.jpg','jpg', 'Quality', 100);
imwrite(p3', 'Polarities\CB 16\XOR Polarity.jpg','jpg', 'Quality', 100);

% Only need once
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%imwrite(w1, 'Watermarks\W1.jpg','jpg', 'Quality', 100);
%imwrite(w2, 'Watermarks\W2.jpg','jpg', 'Quality', 100);
%imwrite(w3, 'Watermarks\W3.jpg','jpg', 'Quality', 100);
%
imwrite(key1, 'Keys\CB 16\Key1.jpg','jpg', 'Quality', 100);
imwrite(key2, 'Keys\CB 16\Key2.jpg','jpg', 'Quality', 100);
imwrite(key3, 'Keys\CB 16\Key3.jpg','jpg', 'Quality', 100);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



