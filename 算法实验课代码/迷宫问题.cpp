#include<iostream>
using namespace std;
#define maxtime 10000
int main() {
	int n, m, start, end;
	cin >> n >> m >> start >> end;
	int* w = new int[n];//存放得分
	for (int i = 0; i < n; i++)
	{
		int value;
		cin >> value;
		w[i] = value;
	}
	int* q = new int[n];
	for (int i = 0; i < n; i++)
		q[i] = 0;
	int** p = new int*[n];//存放i，j两个房间之间的距离
	for (int i = 0; i < n; i++)
	{
		p[i] = new int[n];
		for (int j = 0; j < n; j++)
		{
			if (i == j)p[i][j] = 0;
			else p[i][j] = maxtime;
		}
	}
	for (int i = 0; i < m; i++)
	{
		int x, y, z;
		cin >> x >> y >> z;
		p[x][y] = z;
		p[y][x] = z;//双向
	}
	int** t = new int* [n];
	bool* judge = new bool[n];
	for (int i = 0; i < n; i++)
		judge[i] = false;
	for (int i = 0; i < n; i++)
	{
		t[i] = new int[2];
		int j;
		t[i][0] = -1;//存放上一个房间编号
		t[i][1] = maxtime;//存放到起点的距离
		if (p[start][i]!=maxtime)
		{
			t[i][0] = start;
			t[i][1] = p[start][i];
		}
	}
	for (int i = 0; i < n - 1; i++)
	{
		int temp = maxtime;
		int u = start;
		for (int j = 0; j < n; j++)
		{
			if (j == start)continue;
			if ((!judge[j]) && t[j][1] < temp)
			{
				u = j;
				temp = t[j][1];
			}
		}
		judge[u] = true;
		for (int j = 0; j < n; j++)
		{
			if (u == j)continue;
			int newtime = t[u][1] + p[u][j];
			if (newtime < t[j][1])
			{
				t[j][1] = newtime;
				t[j][0] = u;
				int max = w[j]+w[u];
				int x = u;
				while (t[x][0] != start)//迭代查找，回溯到起点
				{
					max += w[t[x][0]];
					x = t[x][0];
				}
				max += w[start];
				q[j] = max;
			}
			else if (newtime == t[j][1]&&newtime!=maxtime)
			{
				int max1 = w[j];
				int x = j;
				while (t[x][0] != start)//迭代查找
				{
					max1 += w[t[x][0]];
					x = t[x][0];
				}
				max1 += w[start];
				int max2 = w[j] + w[u];
				int y = u;
				while (t[y][0] != start)
				{
					max2 += w[t[y][0]];
					y = t[y][0];
				}
				max2 += w[start];
				if (max2 > max1)
				{
					t[j][0] = u;
					q[j] = max2;
				}
			}
		}
	}
	cout << t[end][1]<<' '<<q[end];
	return 0;
}
