clc;
clear;
obj=instrfind;
delete(obj);
clear obj;

fs=500;
t=0:1/fs:4;
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
    
    load('ECG_LP.mat');
    %ecg = bands(A,5,45,55,fs);
    ecg=A;
    for i=1:100
        ecg = wavelet(ecg);
    end
    
    threshold=max(A);
    buttom=min(A);
    for i=1:(4*fs)
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
        axis([0 4 buttom-20 threshold+20]);
        drawnow
        delete(plot1);
    end
end
figure;
plot(A);
hold on;
axis([0 2000 buttom-20 threshold+20]);
figure;
plot(ecg);
hold on;
axis([0 2000 buttom-20 threshold+20]);
figure;
plot(A-ecg);
hold on;
axis([0 2000 min(A-ecg)-20 max(A-ecg)+20]);
