#include<iostream>
#include<string>
#include<algorithm>
using namespace std;
void pailie(string s,int k,int m){
	if(k==m){
		for(int i=0;i<=m;i++){
			cout<<s[i];
		}
		cout<<" ";
	}
	else{
		for(int i=k;i<=m;i++){
			swap(s[i],s[k]);
			pailie(s,k+1,m);
			swap(s[i],s[k]);
		}
	}
}
int main(){
	string str;
	cin>>str;
	str.pop_back(); 
//	sort(str.begin(), str.end());
//	cout<<str<<" ";
//	while(next_permutation(str.begin(),str.end())){
//		cout<<str<<" ";
//	}
	pailie(str,0,str.length() -1);
	return 0;
}
