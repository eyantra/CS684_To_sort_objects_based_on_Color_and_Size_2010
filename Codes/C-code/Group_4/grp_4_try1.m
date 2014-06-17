clc;
close all;
clear all;

img1=imread('2nalls2.jpg');
imshow(img1);
img = imcrop(img1,[87.5100   61.5100  113.9800   41.9800]);
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

% i=1;
% indent=0;
% indicator=0;
% count_num=0;
% while(i ~= num)
%     indicator=0;
%     if(STATS(i).Area < 400 && STATS(i).Area ~= 0)
%         count_num=count_num+1;
%         for m=i:1:(num-1)
%             STATS(m).Area=STATS(m+1).Area;
%         end
%         STATS(num-count_num+1).Area=0;
%         i=i-1;
%         indicator=1;
%     end
%     if(indicator == 0 && STATS(i).Area ~= 0)
%         indent= indent+1;
%         obj(indent)=i;
%     end
%     i=i+1;
% end
% num1=num-count_num;
%   






        
