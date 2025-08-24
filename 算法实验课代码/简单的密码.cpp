#include<bits/stdc++.h>
using namespace std;
int main(){
	int n, dp[31]={0,0,0,1};
	for(int i=4;i<31;i++){
		dp[i]=2*dp[i-1]+(1<<(i-4))-dp[i-4];
	}
	while(cin>>n){
		cout<<dp[n]<<endl;
	}
	return 0;
}


