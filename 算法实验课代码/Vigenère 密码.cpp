#include<iostream>
#include<cstring>
#include<cmath>
using namespace std;
int main()
{
	string h;
	cin >> h;
	for (int i = 0; i < h.length(); i++)
	{
		if (h[i] >= 'a' && h[i] <= 'z')
			h[i] = h[i] - 32;
	}
	string s;
	cin >> s;
	for (int i = 0; i < s.length(); i++)
	{
		if (s[i] >= 'a' && s[i] <= 'z')
		{
			int x = s[i] - 'a';
			int y = h[i % h.length()] - 'A';
			s[i] = 'a' + (x - y+26) % 26;
		}
		else if (s[i] >= 'A' && s[i] <= 'Z')
		{
			int x = s[i] - 'A';
			int y = h[i % h.length()] - 'A';
			s[i] = 'A' + (x-y+26) % 26;
		}
	}
	for (int i = 0; i < s.length(); i++)
		cout << s[i];
	cout << endl;
}
