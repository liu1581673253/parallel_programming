#include<iostream>

#include<windows.h>
using namespace std;

const int n = 4000;//问题规模，任意指定
int arr[n][n];
int main() {//平凡方法
    DWORD startTime=GetTickCount();


	for (int i = 0;i < n;i++) {
		for (int j = 0;j < n;j++) {
			arr[i][j] = i + j;
		}
	}
	int test[n];
	for (int i = 0;i < n;i++) {
		test[i] = n-i;
	}
	int sum[n];

	int count = 100;//循环计算以拉长时间
	while (count > 0) {
		for (int i = 0;i < n;i++) {
			sum[i] = 0;
		}
		for (int i = 0;i < n;i++) {
			for (int j = 0;j < n;j++) {
				sum[i] += arr[j][i]*test[j] ;
			}
		}
		count--;
	}

	DWORD endTime=GetTickCount();
    cout<<"运行时间为：:"<<endTime-startTime<<"ms"<<endl;
	return 0;
}
