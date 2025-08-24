#include<iostream>
using namespace std;
int main()
{
	int n;
	cin >> n;
	while (n--)
	{
		string h;
		cin >> h;
		int k;
		cin >> k;
		int s = k % 26;
		for (int i = 0; i < h.length(); i++)
		{
			if (h[i] >= 'A' && h[i] <= 'Z' )
			{
				int x = h[i] - 'A';
				h[i] = 'A' + (x - s + 26) % 26;
			}
			else if (h[i] >= 'a' && h[i] <= 'z')
			{
				int x = h[i] - 'a';
				h[i] = 'a' + (x - s + 26) % 26;
			}
		}
		for (int i = 0; i < h.length(); i++)
			cout << h[i];
		cout << endl;
	}
}
