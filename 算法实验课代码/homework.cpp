#include<iostream>
#include<iomanip>
using namespace std;
double value[20];
double time1[20];
double singlevalue[20];
int main(){
	int M,N;
	
	while(true){
		cin>>M>>N;
		int totaltime=N;
		if(M==0&&N==0) break;
		for(int i=0;i<M;i++){
		cin>>time1[i]>>value[i];
		singlevalue[i]=value[i]/time1[i];
	}
	for(int i=0;i<M-1;i++){
		for(int j=0;j<M-i-1;j++){
			if(singlevalue[j]<singlevalue[j+1]){
				swap(singlevalue[j],singlevalue[j+1]);
				swap(time1[j],time1[j+1]);
				swap(value[j],value[j+1]);
			}
			
		}
	}
	double totalprice=0;
	int idx=0;
	while(totaltime>0&&idx<M){
		if(time1[idx]<=totaltime){
			totalprice+=value[idx];
			totaltime=totaltime-time1[idx];
		}
		else{
			totalprice=totalprice+singlevalue[idx]*(totaltime);
			totaltime=0;
		}
		
		idx++;
	}

	printf("%.2lf\n", totalprice);
	}
	
}
