#include<iostream>
#include<string>
int len[2000][2000];
using namespace std;
int main(){
  string a,b;
  cin>>a;
  cin>>b;

  int alen = a.length();
  int blen = b.length();
  a = '0' + a;
  b = '0' + b;

  for(int i=1;i<=blen;i++)
    len[0][i] = 0;
  for(int i=1;i<=alen;i++)
    len[i][0] = 0;
  
  for(int i=1;i<=alen;i++)
    for(int j=1;j<=blen;j++){
      if(a[i] == b[j]){
          len[i][j] = len[i-1][j-1] + 1;
      }
      else if(len[i-1][j]>len[i][j-1])
        len[i][j] = len[i-1][j];
      else
        len[i][j] = len[i][j-1];
    }
  cout<<len[alen][blen];
}

