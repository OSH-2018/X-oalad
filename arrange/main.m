i=0;
while(1)
    if(i>300) break;
    end
    p=imread('lefteye.bmp');
    p=rgb2gray(p);
    p=histeq(p);
    p=imresize(p,[3,5]);
    p=reshape(p,1,15);
    p=double(p)
    y=sim(net,p');
    y=floor(y);
    if(y==0||y==1||y==4||y==5) y=0;
    elseif (y==2||y==3||y==6||y==7) y=1;
    elseif(y==8||y==9||y==12||y==13) y=2;
    else y=3;
    end
    f=fopen('result.txt','w');
    fprintf(f,'%d',y);
    fclose(f);
    i=i+1;
    pause(0.2);
    end