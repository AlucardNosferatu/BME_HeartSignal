    clc;
    clear;
% fs=500;
% t=0:1/fs:1;
% a=0;
% b=0;
% heartrate=0;
% round=0;
% m=50;
% while round<m
%     round=round+1;
%     ecg=zeros(1,500); %�ĵ���ʼ��
    load('ECG_RAW_5Hz.mat');
    %figure;
    %plot(A);
    figure;
    Af=bandp(A, 4.9, 5.1, 4, 6, 0.000011, 0.000012, 500);
    plot(Af);
    %figure;
    %plot(A-Af)
%     for i=1:fs
%         ecg(i)=A(i);
%         if(ecg(i)>250)
%             if(i>5)
%                 if(max(ecg(i-5:i-1))<=250)
%                     disp('Peak!');
%                     disp(t(i));
%                     a=b;
%                     b=t(i-1);
%                     T=b-a;
%                     heartrate=(1/T)*60;
%                 end
%             end
%         end
%     title(['����Ϊ',num2str(heartrate),'bpm']);
%     plot1=plot(t(1:i),ecg(1:i));                  %��ͼecg��1��i��
%     hold on
%     axis([0 1 -40 300]);
%     drawnow                                        %����b�̳���plot�����ȳ���
%     delete(plot1);
%     end
%     a=0;
%     b=0;
% end

