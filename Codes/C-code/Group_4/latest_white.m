
vid=videoinput('winvideo',2,'RGB24_640x480');
% set(vid,'ReturnedColorSpace','rgb');
preview(vid);
img=getsnapshot(vid);
figure;
imshow(img);






% img = imcrop(img,[87.5100   61.5100  113.9800   41.9800]);
 j = size(img); % compute the size of the ball
imshow(img);
count =0;
for k=1:j(1)
    for l=1:j(2)
        
%                 if((img(k,l,1)>160) &&...
%                 (img(k,l,2)>160))
%                 
%             img_bin(k,l)=1;

        
        
        
        
        if((img(k,l,1)>240 && img(k,l,1)<260) &&...
                (img(k,l,2)>240 && img(k,l,2)<260) &&...
                (img(k,l,3)>240 && img(k,l,3)<260))
            img_bin(k,l)=1;
            count=count+1;
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

stop(vid);
delete(vid);
clear vid;