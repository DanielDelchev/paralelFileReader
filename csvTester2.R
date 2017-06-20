PATH = "C:\\Users\\nooby\\Desktop\\SPO\\csvs\\";
COUNT  = 32;
COLUMNS = 2;
TRIALS = 100;
MatrixArray = array(NA,dim=c(TRIALS,COLUMNS,COUNT))
colnames(MatrixArray) = c("FrequencyTable","TotalRuntime");
#k = 2; n=3; m = 4
#array(NA, c(n,m,k))
#
#, , 1
#
#[,1] [,2] [,3] [,4]
#[1,]   NA   NA   NA   NA
#[2,]   NA   NA   NA   NA
#[3,]   NA   NA   NA   NA
#
#, , 2
#
#[,1] [,2] [,3] [,4]
#[1,]   NA   NA   NA   NA
#[2,]   NA   NA   NA   NA
#[3,]   NA   NA   NA   NA
#


for (i in seq(1,COUNT,1))
{
  filename = paste("TestResultFile" ,as.character(i) ,"Threads.csv",sep="");
  fullPath = paste(PATH,filename,sep="");
  MatrixArray[,,i] = as.matrix(read.csv(fullPath),ncol=2);
}

FrequencyTableMeans = array(NA,dim=COUNT)
TotalRuntimeMeans = array(NA,dim=COUNT)
for (i in seq(1,COUNT,1))
{
  FrequencyTableMeans[i] = mean(MatrixArray[,1,i]) 
  TotalRuntimeMeans[i] = mean(MatrixArray[,2,i])
}

FrequencyTableMeans
TotalRuntimeMeans

#Sp = P1/Pn 
Y = TotalRuntimeMeans[1] / TotalRuntimeMeans;
X = seq(1,COUNT,1);
Sp = matrix(c(X,Y),ncol=2)
colnames(Sp) = c("Threads(p)","Acceleration(Sp=T1/Tp)");

Sp


Y2 = TotalRuntimeMeans;

Tp = matrix(c(X,Y2),ncol=2)
colnames(Tp) = c("Threads(p)","Time in microsec (Tp)");

Tp

#Ep = Sp /p
Y3 = Y / X

Ep = matrix(c(X,Y3),ncol=2)
colnames(Ep) = c("Threads(p)","Efficiency (Ep=Sp/p)");

#######################################################

thickness = 2;

perfect = matrix(c(seq(1,48,1),seq(1,48,1)),ncol=2)
colnames(perfect) = c("Threads","Acceleration");

################################################################### 8
#plot(Tp,type='l',col='red',lwd=thickness,xlim=c(1,8),lab=c(8,8,6))
#
#plot(perfect,type='l',col='red',xlim=c(1,8),ylim=c(1,8),lwd=thickness,lab=c(8,8,6))
#lines(Sp,type='b',lwd=thickness)
#
#plot(Ep,type='l',col='red',lwd=thickness,xlim=c(1,8),lab=c(8,8,6))

#################################################################### 16


#plot(Tp,type='l',col='red',lwd=thickness,xlim=c(1,16),lab=c(16,16,6))
#
#plot(perfect,type='l',col='red',xlim=c(1,16),ylim=c(1,16),lwd=thickness,lab=c(16,16,6))
#lines(Sp,type='b',lwd=thickness)
#
#plot(Ep,type='l',col='red',lwd=thickness,xlim=c(1,16),lab=c(16,16,6))
#
################################################################### 24

plot(Tp,type='l',col='blue',lwd=thickness,xlim=c(1,24),lab=c(12,12,6))

plot(perfect,type='l',col='black',xlim=c(1,24),ylim=c(1,24),lwd=thickness,lab=c(12,12,6))
lines(Sp,type='l',lwd=thickness,col='blue')

plot(Ep,type='l',col='blue',lwd=thickness,xlim=c(1,24),lab=c(12,12,6))

################################################################### 32
#plot(Tp,type='l',col='red',lwd=thickness,xlim=c(1,32),lab=c(16,16,6))
#
#plot(perfect,type='l',col='red',xlim=c(1,32),ylim=c(1,32),lwd=thickness,lab=c(16,16,6))
#lines(Sp,type='b',lwd=thickness)
#
#plot(Ep,type='l',col='red',lwd=thickness,xlim=c(1,32),lab=c(16,16,6))
##################################################################

results = matrix(nrow=32,ncol=4)
results[,1] = seq(1,COUNT,1)
results[,2] = Y2
results[,3] = Y
results[,4] = Y3
colnames(results) = c('Threads(p)','Time in microsec(Tp)','Acceleration(Sp)','Efficiency(Ep)')
results


#install.packages('xlsx')
library(xlsx)
write.xlsx(x=results,file="C:\\Users\\nooby\\Desktop\\SPO\\csvs\\results.xls")



#2
SPFS
TPFS
EPFS

plot(TPFS,type='l',col='blue',lwd=thickness,xlim=c(1,24),ylim = c(250000,30000000),lab=c(12,12,6))
lines(Tp,type='l',col='red',lwd=thickness,xlim=c(1,24),lab=c(12,12,6))

plot(perfect,type='l',col='black',xlim=c(1,24),ylim=c(1,24),lwd=thickness,lab=c(12,12,6))
lines(Sp,type='l',lwd=thickness,col='red')
lines(SPFS,type='l',lwd=thickness,col='blue')

plot(Ep,type='l',col='red',lwd=thickness,xlim=c(1,24),lab=c(12,12,6))
lines(EPFS,type='l',col='blue',lwd=thickness,xlim=c(1,24),lab=c(12,12,6))