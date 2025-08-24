#include<iostream>
#include<vector>
#include<stack>
#include<queue>
#include<algorithm>
using namespace std;
struct Student{
	int num;
	string str;
};
int main(){
//	vector
	vector<int>v1;
	v1.push_back(3);
	v1.push_back(2);
	v1.push_back(2);
	v1.push_back(2);
	v1.push_back(2);
	v1.push_back(2);
	sort(v1.begin(),v1.end());
	v1.insert(v1.begin(),5); 
	v1.pop_back();
	v1.erase(v1.begin(),v1.begin()+1);
		for(int i=0;i<v1.size();i++){
		cout<<v1[i];
	}
//	stack
	stack<int>s1;
	s1.push(1);
	s1.push(2);
	s1.push(3);
	s1.push(4);
	s1.pop();
	for(int i=0;i<s1.size();i++){
//		cout<<s1.top();
//		int a=s1.pop();
	}
	cout<<s1.empty();
//	
//	q.front()	返回队首元素 O ( 1 ) O(1)O(1)
//q.back()	返回队尾元素 O ( 1 ) O(1)O(1)
//q.push(element)	尾部添加一个元素element 进队O ( 1 ) O(1)O(1)
//q.pop()	删除第一个元素 出队 O ( 1 ) O(1)O(1)
//q.size()	返回队列中元素个数，返回值类型unsigned int O ( 1 ) O(1)O(1)
//q.empty()	判断是否为空，队列为空，返回true O ( 1 ) O(1)O(1)
	Student students[20];
	students[0].num =1;
	return 0;
}
