#include<iostream>
#include<algorithm>
#include<cmath>
using namespace std;
vector<double>a{-2,2,2,3,3};
double sum(double x){
	double sum=0;
	for(int i=0;i<a.size();i++){
		sum+=fabs(a[i]-x);
	}
	return sum;
}
int main(){

	double L=-7;
	double R=8;
	double mid;
	double LM,RM;
	while(L+0.001<=R){
		double mid=(R-L)/3;
		LM=L+mid;
		RM=R -mid;
		if(sum(LM)<sum(RM)) R=RM;
		else L=LM;
	}
	cout<<L<<R<<endl;
}
