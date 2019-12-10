function output=bandp(input,pass_low,pass_high,stop_low,stop_high,decay_bandside,decay_cutoff,samplerate)


% input 输入信号
% pass_low 通带下沿 不可大于samplerate/2
% pass_high 通带上沿 不可大于samplerate/2
% stop_low 阻带下沿 不可大于samplerate/2
% stop_high 阻带上沿 不可大于samplerate/2
% decay_bandside 边带衰减系数
% decay_cutoff 截止区衰减系数
% samplerate 采样率（500Hz）

%将频率参数转化为角频率
pla=2*pi*pass_low/samplerate;
pha=2*pi*pass_high/samplerate;
sla=2*pi*stop_low/samplerate;
sha=2*pi*stop_high/samplerate;
passband=[pla pha];
stopband=[sla sha];

%用切比雪夫滤波器生成函数生成滤波差分方程的分子和分母数组
[orders,passband]=cheb1ord(passband,stopband,decay_bandside,decay_cutoff);
% orders 阶数
% passband 通带（与输入的一致）
[numerators,denominators]=cheby1(orders,decay_bandside,passband/(2*pi));
% numerators 分子
% denominators 分母

%将算出的分子分母数组和输入信号输入到滤波函数得到滤过后的
output=filter(numerators,denominators,input);% output 滤过信号

end