#include<iostream>
#include<chrono>

using namespace std;
typedef unsigned long long ll;
const int MAXN=5000;
ll arr[MAXN][MAXN];
ll b[MAXN];
int main(){
    //��ʼ������������
    for(int i=0;i<MAXN;i++){
        b[i]=MAXN-i;
        for(int j=0;j<MAXN;j++) arr[i][j]=i+j;
    }

    for(int n=200;n<=MAXN;n+=200){ //��200Ϊ�������в���
        ll sum[MAXN];
        auto start = std::chrono::system_clock::now();//��ʼ��ʱ

        int count = 10;//ѭ������������ʱ��
        while (count > 0) {
		for (int i = 0;i < n;i++) {
			sum[i] = 0;
		}
		for (int i = 0;i < n;i++) {
			for (int j = 0;j < n;j++) {
				sum[j] += arr[i][j]*b[i];//ƽ���㷨
			}
		}
		count--;
        }


        auto end = std::chrono::system_clock::now();//������ʱ
        std::chrono::duration<double> dura = end - start;
        std::cout <<"when n="<< n<< "costed "<<dura.count() << "s" << std::endl;
    }

    return 0;
}
