#include<iostream>
#include<string>
int c[2000][2000];
char s1[2000];
char s2[2000];
using namespace std;
int main(){
	string s11,s22;
	cin>>s11;
	cin>>s22;
	int m=s11.length();
	int n=s22.length();
	int i,j;
	
	for(int i=1;i<=m;i++) {c[i][0]=0; s1[i]=s11[i-1];}
	for(int i=1;i<=n;i++) {c[0][i]=0; s2[i]=s22[i-1];}
	for(int i=1;i<=m;i++){
		for(int j=1;j<=n;j++){
			if(s1[i]==s2[j]){
				c[i][j]=c[i-1][j-1]+1;
			}
			else if(c[i-1][j]>=c[i][j-1]){
				c[i][j]=c[i-1][j];
			}
			else{
					c[i][j]=c[i][j-1];
				}
		}
	}
	cout<<c[m][n]<<endl;
	return 0;
} 
