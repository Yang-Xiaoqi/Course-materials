#include<bits/stdc++.h>
#include<queue> 
using namespace std;
int b[26];
priority_queue<int, vector<int>, greater<int> >q;
void Cal(){
	int sum = 0, x, y;
	while(!q.empty()) q.pop();
	for(int i=0;i<26;i++){
		if(b[i]) q.push(b[i]);
	}
	while(q.size()>1){
		x = q.top(), q.pop();
		y= q.top(), q.pop();
		q.push(x + y);
		sum = sum + x + y;
	}
	cout<<sum<<endl;
}

int main(){
	int T, len; 
	cin>>T;
	while(T--){
		string a;
		cin>>a;
		len = a.size();
		memset(b, 0, sizeof(b));
		for( int i=0; i<len; i++) b[a[i] - 'a'] ++;
		Cal();
	}
	return 0;
}

