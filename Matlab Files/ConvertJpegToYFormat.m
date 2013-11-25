% Converts bmps of jpegs back to .Y so C can read them...
close all; clear all; clc;

a = imread('Watermarked Lena QF 30.jpg');
b = imread('Watermarked Lena QF 50.jpg');
c = imread('Watermarked Lena QF 80.jpg');
d = imread('Watermarked Lena QF 100.jpg');

figure(1);image(a);colormap(gray(256));axis equal;title 'Qf 30';
figure(2);image(b);colormap(gray(256));axis equal;title 'Qf 50';
figure(3);image(c);colormap(gray(256));axis equal;title 'Qf 80';
figure(4);image(d);colormap(gray(256));axis equal;title 'Qf 100';


fid1 = ['Stage 1 and 2\Qf 30\Watermarked Lena QF 30.Y'];
fwrite3(a',fid1,0,0);

fid2 = ['Stage 1 and 2\Qf 50\Watermarked Lena QF 50.Y'];
fwrite3(b',fid2,0,0);

fid3 = ['Stage 1 and 2\Qf 80\Watermarked Lena QF 80.Y'];
fwrite3(c',fid3,0,0);

fid4 = ['Stage 1 and 2\Qf 100\Watermarked Lena QF 100.Y'];
fwrite3(d',fid4,0,0);

aa = fread3('Stage 1 and 2\Qf 30\Watermarked Lena QF 30.Y',0,0,512,512);
bb = fread3('Stage 1 and 2\Qf 50\Watermarked Lena QF 50.Y',0,0,512,512);
cc = fread3('Stage 1 and 2\Qf 80\Watermarked Lena QF 80.Y',0,0,512,512);
dd = fread3('Stage 1 and 2\Qf 100\Watermarked Lena QF 100.Y',0,0,512,512);

figure(5);image(aa');colormap(gray(256));axis equal;title 'Qf 30.Y';
figure(6);image(bb');colormap(gray(256));axis equal;title 'Qf 50.Y';
figure(7);image(cc');colormap(gray(256));axis equal;title 'Qf 80.Y';
figure(8);image(dd');colormap(gray(256));axis equal;title 'Qf 100.Y';
