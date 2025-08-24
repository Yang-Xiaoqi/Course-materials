#include<iostream>
#include<algorithm>
using namespace std;
struct Worktime {
	int start;
	int end;
	Worktime(int s = NULL, int e = NULL) :start(s), end(e) {};
};
bool cmp(Worktime& a, Worktime& b)
{
	if (a.start != b.start)
		return a.start < b.start;
	return a.end < b.end;
}
int main()
{
	int N;
	cin >> N;
	Worktime* p = new Worktime[N];
	for (int i = 0; i < N; i++)
	{
		int x;
		cin >> x;
		p[i].start = x;
		int y;
		cin >> y;
		p[i].end = y;
	}
	sort(p, p + N, cmp);
	int max1 = p[0].end - p[0].start;
	int max2 = 0;
	int start = p[0].start;
	int end = p[0].end;
	for (int i = 0; i <N-1; i+=1)
	{
		if (end >= p[i+1].start)
		{
			end = p[i + 1].end > end ? p[i + 1].end : end;
			max1 = end-start> max1 ? end-start : max1;
		}
		else
		{
			start = p[i + 1].start;
			max2 =  p[i+1].start-end > max2 ? p[i+1].start - end : max2;
			end = p[i + 1].end;
		}
	}
	cout << max1 << ' ' << max2 << endl;
}
