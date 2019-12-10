function output=wavelet(input)
%小波分解
[C,L]=wavedec(input,3,'db5');
%从信号c中提取尺度1，2，3下的细节小波系数
cD1=detcoef(C,L,1);
cD2=detcoef(C,L,2);
cD3=detcoef(C,L,3);
%使用stein的无偏似然估计原理选择各层阈值
%cD1,cD2,cD3为各层小波系数，‘ringrsure’位无偏似然估计阈值类型
thr1=thselect(cD1,'minimaxi');
thr2=thselect(cD2,'minimaxi');
thr3=thselect(cD3,'minimaxi');
%各层的阈值
TR=[thr1, thr2, thr3];
%------去噪-----
%XC为去噪后信号，[CXC,LXC]为小波分解结构，PERF0和PERF2是恢复和压缩的范数百分比
%'lvd'为允许设置各层的阈值
%'gb1'为固定阈值，3为阈值长度
[output,~,~,~,~]=wdencmp('lvd',input,'db5',3,TR,'s');
end