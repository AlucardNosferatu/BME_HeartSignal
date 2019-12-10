function output=bands(input,pass_high,stop_low,stop_high,samplerate)

Wn=[stop_low/(samplerate/2),stop_high/(samplerate/2)];%由设定的阻带频率得到阻带参数
[B_n,A_n]=butter(5,Wn,'stop');%把阻带参数赋值给巴特沃斯滤波器参数生成函数，滤波阶数设定为5

Wn_lp = pass_high/(samplerate/2);%由设定的低通上限频率得到通带参数
[B_lp,A_lp]=butter(7,Wn_lp,'low');%把通带参数赋值给巴特沃斯滤波器参数生成函数，滤波阶数设定为5，模式为设为低通

output=filter(B_n,A_n,input);%先进行陷波滤波
output=filter(B_lp,A_lp,output);%对陷波滤波的结果进行低通滤波
end