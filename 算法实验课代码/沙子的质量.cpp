#include<iostream>
using namespace std;
const int N = 305;
int dp[N][N];
int a[N];
int main()
{
	int n;
	cin>>n;
	for(int i=1;i<=n;i++)
		cin>>a[i];
	for(int i=1;i<=n;i++)
		dp[i][i] = 0;
	for(int r=2;r<=n;r++)
	{
		for(int i=1;i<=n-r+1;i++)
		{
			int j = i+r-1;
			int sum=0;
			for(int k=i;k<=j;k++)
				sum+=a[k];
			dp[i][j] = dp[i][i] + dp[i+1][j] + sum;
			for(int k=i+1;k<j;k++)
			{
				int t = dp[i][k] + dp[k+1][j] + sum;
				if(t<dp[i][j])
					dp[i][j] = t;
			}
		}
	}
	cout<<dp[1][n];
}
