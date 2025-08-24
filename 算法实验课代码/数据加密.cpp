#include<iostream>
#include<cstring>
using namespace std;
int main()
{
	int N;
	while (cin >> N)
	{
		char* p = new char[N];
		string s;
		cin >> s;
		int count = 0;
		int top = 0;
		int bottom = s.length() - 1;
		if (N == 1)
		{
			cout << s << endl;
			continue;
		}
		while (top <= bottom)
		{
			if (s[top] - s[bottom] > 0)
			{
				p[count++] = s[bottom];
				bottom--;
			}
			else if (s[top] - s[bottom] < 0)
			{
				p[count++] = s[top];
				top++;
			}
			else
			{
				int i;
				for (i = 1; i <= (bottom - top) / 2; i++)
				{
					if (s[top + i] != s[bottom - i])
						break;
				}
				if (i > (bottom - top) / 2)
				{
					p[count++] = s[bottom];
					bottom--;
					continue;
				}
				if (s[top + i] > s[bottom - i])
				{
					p[count++] = s[bottom];
					bottom--;
				}
				else if (s[top + i] < s[bottom - i])
				{
					p[count++] = s[top];
					top++;
				}
			}
		}
		for (int i = 0; i < N; i++)
			cout << p[i];
		cout << endl;
	}
}
