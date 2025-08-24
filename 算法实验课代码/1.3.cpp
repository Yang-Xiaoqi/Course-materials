#include<iostream>
#include<vector>
using namespace std;
void jinzhi(int n){
	vector<int>v1;
	int num=0;
	while(n!=0){
		v1.push_back(n%8);
		n=n/8;
	}
	for(int i=v1.size()-1;i>=0;i--)
	{
		cout<<v1[i];
	}
}
int main(){
	long long n;
	cin>>n;
	jinzhi(n);
}

