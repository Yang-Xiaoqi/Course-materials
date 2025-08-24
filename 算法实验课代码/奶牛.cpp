#include<iostream>
#include<cmath>
double arr[50000][2];
using namespace std;
double cal(double base,int n){
	double ans=0;
	for(int i=0;i<n;i++){
		ans=ans+pow(fabs(arr[i][0]-base),3)*arr[i][1];
	}
	return ans;
}
int main(){
	int ca;
	cin>>ca;
	int n;
	double si,wi;
	double pos;

	for(int i=1;i<=ca;i++){
		cin>>n;
		for(int j=0;j<n;j++){
			cin>>arr[j][0];
			cin>>arr[j][1];
		}
		double L=-1000000;
		double R=1000000;
		double pl;
		double pr;
		double r;
		while(L+0.00001<R){
			r=(R-L)/3;
			pl=L+r;
			pr=R-r;
			if(cal(pl,n)<cal(pr,n)) R=pr;
			else L=pl;
		}
		
		cout<<"Case #"<<i<<": "<<(long long)(cal(L,n)+0.5)<<endl;
	}
}
