#include<iostream>

#include<windows.h>
using namespace std;

const int n = 4000;//�����ģ������ָ��
int arr[n][n];
int main() {//ƽ������
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

	int count = 100;//ѭ������������ʱ��
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
    cout<<"����ʱ��Ϊ��:"<<endTime-startTime<<"ms"<<endl;
	return 0;
}
