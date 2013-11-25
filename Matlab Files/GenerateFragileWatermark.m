% This file generates the fragile watermark by reading in an image with
% size 32x32 and replicating it to achieve size of 128x128 and writes to .Y
% file as short int using fwrite3 so it's suitable for use with the 2nd
% stage vq without any casting operations needed.

close all;clear all;clc;
pic = imread('Watermarks\Temp\Fragile Image','png');
figure(1);image(pic);colormap(gray(256));
grey_pic = rgb2gray(pic);
level = graythresh(grey_pic);
bw_pic = im2bw(grey_pic,level);
figure(2);imshow(bw_pic,'InitialMagnification','fit');axis equal;shg;

new_pic = [128, 128];

% Blow up image 4x4 times, i and j control which block of 32 is being
% written to (16 times).
for i=0:3
    for j=0:3;
        for row=1:32;
            for col=1:32;
            new_pic((row+i*32),(col+j*32)) = bw_pic(row,col);
            end
        end
    end
end

%Cast to short int and write to file...
fwrite3(new_pic','Watermarks\Fragile Watermark.Y',1,0);
% Write to jpeg for report..note: no transpose
imwrite(new_pic, 'Watermarks\Fragile Watermark Jpeg.jpg', 'jpg', 'Quality', 100);

% Test to see working ok...
in_pic = fread3('Watermarks\Fragile Watermark.Y',1,0,128,128);
figure(3);imshow(in_pic','InitialMagnification','fit');axis equal;shg;title 'Read back in';