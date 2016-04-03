%This program takes the data obtained by the temperature module developed
%in the arduino part together with altitude information obtained from
%another module and plots the results. The data displayed is the
%theoretical atmospheric temperature, the real atmospheric temperature, the
%temperature of the visible sky (with an IR sensor pointing upwards)and the
%temperature of the interior of the module.

dataGPS= xlsread('GPSLog.xlsx');
dataTemp=xlsread('TempLog.xlsx');
dataOther= xlsread('OtherLog.xlsx');

%The data is splited between diferent arrays for posterior processing
altGPS = dataGPS(:,4);
tempSky= dataTemp(:,1);
tempExt= dataTemp(:,8);
tempInt= dataTemp(:,9);
altPres= dataOther(:,5);

count=1;
step=75;
m=1;
data=zeros(6900,7);

for i=1: 6900           %6900 in not a random number but the number of point data that belong to the ascend, in order to ignore the descent
    
    if(i==count)        %Altitude points are assigned to temperature points. 
        data(i,1)=altGPS(m)/1000;
        count=count+step;
        m=m+1;
    else        %Due to the fact that there are more temperature points than altitude points, the missing altitude points are aproximated as a linear increase between the known previous and next altitude data points
        data(i,1)=data(i-1,1)+(altGPS(m)-altGPS(m-1))/(1000*step);
    end
    
    data(i,2)=tempExt(i);
    data(i,3)=tempSky(i);
    data(i,4)=tempInt(i);
    
    if(data(i,1)<11)            %this lines are in charge to draw the ISA temperature curve
        data(i,5)=15.5-6.5*data(i,1);
        
    elseif (data(i,1)>20)
        data(i,5)=-56.5+data(i,1)-20;
    else
        data(i,5)=-56.6;
    end
    
end

count1=1;
step1=1.31855;
m1=1;
for i=1: 9098       %9098 is the last pressure point that belongs to the ascent
    
        if(i>=count1)           %pressure points are discarded in order to match the data temperature and altitude data points
        data(m1,6)=altPres(i);
        count1=count1+step1;
        m1=m1+1;

    end
end

data(:,7)=data(:,6)./(287*(data(:,2)+273));   %air density

densitat_acumulada=zeros(6900);

for i=1: 6900
    for j=1 : 6900 
        densitat_acumulada(i)=densitat_acumulada(i)+data(j,7);
    end
end

plot(data(:,2),data(:,1),data(:,3),data(:,1),data(:,4),data(:,1),data(:,5),data(:,1))
title('Temperature versus altitude data')
xlabel('Temperature (ºC)')
ylabel('Altitude (km)')

legend('Atmospheric Temperature','Visible Sky temperature','Interior Temperature of the Module','ISA Atmosphere Temperature')

grid on



filename = 'data.xlsx';
xlswrite(filename,data)
