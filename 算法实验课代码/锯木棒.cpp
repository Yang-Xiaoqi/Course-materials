#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;
int main(){
	int n;
	long long t;
	vector<long long>v1;
	long long L;
	cin>>n>>L;
	for(int i=0;i<n;i++){
		cin>>t;
		v1.push_back(t);
	}
	sort(v1.begin(),v1.end());
//	for(int i=0;i<v1.size();i++){
//		cout<<v1[i]<<" ";
//	}
	long long sum=0;
	while(v1.size()>1){
//		cout<<1<<endl;
		sum=sum+v1[0]+v1[1];
		v1.push_back(v1[0]+v1[1]); 
		v1.erase(v1.begin());
		v1.erase(v1.begin());
		sort(v1.begin(),v1.end());
//		for(int i=0;i<v1.size();i++){
//		cout<<v1[i]<<" ";
//	}
	}
	cout<<sum<<endl;
}
