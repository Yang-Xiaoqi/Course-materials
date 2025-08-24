#include<iostream>
#include<cmath>
using namespace std;
bool prime(int n)
{
	if (n <= 1)return false;
	if (n == 2)return true;
	for (int i = 2; i <= pow(n, 0.5); i++)
	{
		if (n % i == 0)return false;
	}
	return true;
}
void DFS(bool* visit, int previous, int* num,int n,int first,int &count)
{
	int j;
	for ( j = 1; j <= n; j++)
	{
		if (visit[j] == false)
			break;
	}
	if (j > n)
	{
		if (prime(num[first] + previous) == true)
		{
			count++;
			return;
		}
		else
		{
			return;
		}
	}
	int i;
	for ( i = 1; i <= n; i++)
		if (visit[i] == false&&prime(num[i] + previous) == true)
		{ 
			visit[i] = true;
			DFS(visit, num[i], num, n,first,count);
			visit[i] = false;
		}
}
int main()
{
  int n;
  while (cin >> n)
  {
	  int* num = new int[n + 1];
	  int count = 0;
	  for (int i = 1; i <= n; i++)
	  {
		  num[i] = i;
	  }
	  bool* visit = new bool[n + 1];
	  for (int j = 1; j <= n; j++)
	  {
		  visit[j] = false;
	  }
	  visit[1] = true;
	  DFS(visit, num[1], num, n, 1, count);
	  cout << count << endl;
  }
  return 0;
}
