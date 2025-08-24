#include<iostream>
using namespace std;
long long exp(int x){
	long long sum1=1;
	long long y=x%100000007;
	while(x>0){
		if(x%2==1){
			sum1=(y*sum1)%100000007;
		}
		y=(y*y)%100000007;
		x=x/2;
			
	}
	return sum1;
}
int main(){
	int x;
	while(cin>>x){
		long long sum1=0;
		for(int i=1;i<=x;i++){
			sum1=(sum1+exp(i))%100000007;
			
		}
		cout<<(sum1+1)%100000007<<endl;
	}
}
