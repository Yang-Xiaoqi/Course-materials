#include<iostream>
using namespace std;
long long f(int n){
	if(n==0||n==1) return 1;
	else {
		long long a = 1, b = 1, c;
    	for (int i = 2; i <= n; ++i) {
        	c = (a + b) % 1000000007;
        	a = b;
        	b = c;
    	}
    return b;
	}
}
int main(){
	int n=0;
	while(cin>>n){
		if(n==1) cout<<1<<endl;
		else{
			long long num=f(n);
			cout<<num<<endl;
		}
		
	}
}
