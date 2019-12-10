clc;
clear;
obj=instrfind;
delete(obj);
clear obj;

s = serial('COM7');
set(s,'baudrate',9600);
set(s,'parity','none');
set(s,'databits',8);
set(s,'stopbits',1);
set(s,'inputbuffersize',1024);   % set input buffer size
set(s,'timeout',500);
%fopen(s);

% data = fread(s,200,'uint8');  % read data from serial port
% fwrite(s,2,'uint8');    % write data to serial port
% fclose(s);
% delete(s);
% A = reshape(data,2,length(data)/2);
% V = A(2,:)*256 + A(1,:);
% V = 1.517/1023*V;
% figure; plot(V); grid on
% delete(s);
% data2 = data(2:(end-1));
% A = reshape(data2,2,length(data2)/2);
% V = A(2,:)*256 + A(1,:);
% V = 1.517/1023*V;
% figure; plot(V);grid on

fs=500;
t=0:1/fs:16;
nfs=0:fs;                    %采样率设置
a=0;
b=0;
heartrate=0;
round=0;
m=1;
while round<m
    round=round+1;
    %data = fread(s,500,'uint8');
    
    %从串口读取500字节数据
    
    %data = modelecg(500);
    %A = reshape(data,1,length(data));   %列数据转行数据
    %A = bands(A,30,50,39,41,0.1,30,fs);  %带阻滤波
    load('ECG_LP.mat');
    A=[A A];
    A=[A A];
    for i=1:100
        A = bandp(A,0.2,2,0.1,2.1,0.000000001,0.0000000015,fs);
    end
    ecg = A;
    
    threshold=max(A);
    buttom=min(A);
    for i=1:(16*fs)
        if(ecg(i)>threshold-10)
            if(i>10)
                if(max(ecg(i-10:i-1))<=threshold-10)
                    disp('Peak!');
                    a=b;
                    b=t(i-1);
                    while(b-a<0.15)
                        b=b+1;
                    end
                    T=b-a;
                    heartrate=(1/T)*60;
                end
            end
        end
        % 发送给MSP430心率值
        title(['心率为',num2str(heartrate),'bpm']);
        plot1=plot(t(1:i),ecg(1:i));
        hold on
        axis([0 16 buttom-20 threshold+20]);
        drawnow
        delete(plot1);
    end
    %fwrite(s,heartrate,'uint8'); %现在立刻出现plot不会等程序
end
figure;
plot(A);
hold on;
axis([0 8000 buttom-20 threshold+20]);
figure;
plot(ecg);
hold on;
axis([0 8000 buttom-20 threshold+20]);
figure;
plot(A-ecg);
hold on;
axis([0 8000 min(A-ecg)-20 max(A-ecg)+20]);
%fclose(s);
%delete(s);