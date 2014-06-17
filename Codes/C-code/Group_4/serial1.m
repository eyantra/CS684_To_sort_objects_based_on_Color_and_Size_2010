%%%%
% * Copyright (c) 2010, this is a property of ERTS Lab, IIT Bombay.
% * This project is developed as part of the Embedded Systems (Software) course CS 684.
% *
% * PROJECT TITLE: To sort object based on color and size usinf Firebird V robot
% *
% * @section PROJECT SUMMARY
% * The robot covers the entire arena according to a predetermined algorithm.
% * It detects objects and captures its image after proper alignment.
% * In ignores objects that are not of the desired color.
% * For the desired color objects it sorts the objects according to size and places them in seperate bins.
% * This piece of code, is converted to a hex file of ATMEGA2560 standard
% * and burnt onto the FIREBIRD V Robot.
% *
% * @file serial.m
% * This file contains the matlab code for image processing.
% * 
% * @author Group4. Arpita Agrawal, Ruttika Maheshwari, Sukanya Basu
% *
% * @date LAST MODIFIED: Nov 8, 2010
% * @version 1.0
% *
% * @section LICENSE
% * This program is free software; you can redistribute it and/or
% * modify it under the terms of the GNU General Public License as
% * published by the Free Software Foundation; either version 2 of
% * the License, or (at your option) any later version.
% * This program is distributed in the hope that it will be useful, but
% * WITHOUT ANY WARRANTY; without even the implied warranty of
% * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
% * General Public License for more details at
% * http://www.gnu.org/copyleft/gpl.html
% *
% *
% * @section COMPILATION
% * Compile using the AVR studio for ATMEGA2560 and F_CPU=11059200
% * Write the .hex file to the FIREBIRD V robot
% *
% *
%%%




s=serial('COM26');
fopen(s);
get(s,{'STATUS','Type'});

s.ReadAsyncMode = 'continuous';
% fprintf(s,'*IDN?');
% s.BytesAvailable


while(s.BytesAvailable == 0)
end

out = 0;
while(out ~= 08)
out = fscanf(s)
end

%% image processing logic


vid=videoinput('winvideo',2,'RGB24_640x480');
% set(vid,'ReturnedColorSpace','rgb');
preview(vid);
img=getsnapshot(vid);
figure;
imshow(img);






% img = imcrop(img,[87.5100   61.5100  113.9800   41.9800]);
 j = size(img); % compute the size of the ball
imshow(img);
for k=1:j(1)
    for l=1:j(2)
        
%                 if((img(k,l,1)>160) &&...
%                 (img(k,l,2)>160))
%                 
%             img_bin(k,l)=1;

        
        
        
        
        if((img(k,l,1)>40 && img(k,l,1)<110) &&...
                (img(k,l,2)>=0 && img(k,l,2)<30) &&...
                (img(k,l,3)>=0 && img(k,l,3)<10))
            img_bin(k,l)=1;
%         elseif((img(k,l,1) ~= 0 && img(k,l,1) ~= 0) &&...
%                 (img(k,l,2) ~= 0 && img(k,l,2) ~= 0) &&...
%                 (img(k,l,3) ~= 0 && img(k,l,3) ~= 0))
%             img_bin(k,l)=2;
            else
                img_bin(k,l)=0;
        end
    end
end

imshow(img_bin);
            

[label,num]=bwlabel(img_bin);
imagesc(label);

STATS = regionprops(label, 'Area');


%% send data acoorfing to area and color

if  STATS < "nothinng to be picked"
    send=28;
else if STATS > " nothing" && STATS < " max"
        send = 38;
    else if STATS > " max"
            send = 48;
        else
            send = 58;
        end
    end
end





%% Sending back to the bot
fprintf(s,send)

fclose(s)
delete(s)
clear s


stop(vid);
delete(vid);
clear vid;
