#include<iostream>
#include<algorithm>
using namespace std;
int main(){
	double price[10];
	int index[10];
	double singleprice[10];
	for(int i=0;i<10;i++){
		cin>>price[i];
		index[i]=i+1;
		singleprice[i]=price[i]/(i+1);
	}
	for(int i=0;i<9;i++){
		for(int j=0;j<9-i;j++){
			if(singleprice[j]>singleprice[j+1]){
				double temp1=singleprice[j];
				singleprice[j]=singleprice[j+1];
				singleprice[j+1]=temp1;
				double temp2=price[j];
				price[j]=price[j+1];
				price[j+1]=temp2;
				int temp3=index[j];
				index[j]=index[j+1];
				index[j+1]=temp3;
			}
		}
	}
//	for(int i=0;i<10;i++){
//		cout<<index[i]<<""<<price[i]<<endl;
//		
//	}
	double sumprice=0;
	int idx=0;
	int n;
	cin>>n;
	while(n>0){
//		cout<<n<<endl;
//		cout<<1<<endl;
		while(index[idx]>n){
			idx++;
		}
		int x=n/index[idx];
//		cout<<x<<" ";
		sumprice=sumprice+x*price[idx];
		n=n-x*index[idx];
	}
	cout<<sumprice<<endl;
}
