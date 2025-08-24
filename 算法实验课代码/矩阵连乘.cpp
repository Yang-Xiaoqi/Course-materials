#include<iostream>
using namespace std;

int main(){
	int n,m;
	int p[27];
	while(cin>>n>>m){
	char ch[26];
	int arr[26][2];
	int arr1[26][2];
	long long m1[26][26]={0};
	string str;
	for(int i=0;i<n;i++)
	{
		cin>>ch[i];
		cin>>arr[i][0];
		cin>>arr[i][1];
	}
// °´ÕÕ×ÖÄ¸½øĞĞÅÅĞò 
	for(int k=0;k<m;k++){
		cin>>str;
		bool flag=true;
		int len=0;
		for(int i=0;i<n;i++){
			for(int j=0;j<n;j++){
//				cout<<str[i]<<" "<<ch[j]<<endl;
				if(str[i]==ch[j]){
					arr1[len][0]=arr[j][0];
					arr1[len][1]=arr[j][1];
					len++;
					break;
				}
			}
	}
	p[0]=arr1[0][0];
	p[n]=arr1[n-1][1];
	for(int i=1;i<n;i++){
		p[i]=arr1[i-1][1];
		if(arr1[i][0]!=arr1[i-1][1]){
			cout<<"MengMengDa"<<endl;
			flag=false;
			break;
		}
	}
	for(int i=0;i<=n;i++){
//		cout<<arr1[i][0]<<" "<<arr1[i][1]<<endl;
//		cout<<p[i]<<endl;
	}
	if(flag){
		
			for(int i=1;i<=n;i++) m1[i][i]=0;
			for(int r=2;r<=n;r++){
				for(int i=1;i<=n-r+1;i++){
					int j = i+r-1;
					m1[i][j]=m1[i+1][j]+p[i-1]*p[i]*p[j];
					for(int k=i+1;k<j;k++){
						int t=m1[i][k]+m1[k+1][j]+p[i-1]*p[k]*p[j];
						if(t<m1[i][j]) m1[i][j]=t;
					}
				}
			}
			cout<<m1[1][n]<<endl;

	}
	
}
	}
	return 0;
}
