#include<iostream>
#include<chrono>

using namespace std;
typedef unsigned long long ll;
const int MAXN=5000;
ll arr[MAXN][MAXN];
ll b[MAXN];
int main(){
    //初始化数组与向量
    for(int i=0;i<MAXN;i++){
        b[i]=MAXN-i;
        for(int j=0;j<MAXN;j++) arr[i][j]=i+j;
    }

    for(int n=200;n<=MAXN;n+=200){ //以200为步长进行测试
        ll sum[MAXN];
        auto start = std::chrono::system_clock::now();//开始计时

        int count = 10;//循环计算以拉长时间
        while (count > 0) {
		for (int i = 0;i < n;i++) {
			sum[i] = 0;
		}
		for (int i = 0;i < n;i++) {
			for (int j = 0;j < n;j++) {
				sum[j] += arr[i][j]*b[i];//平凡算法
			}
		}
		count--;
        }


        auto end = std::chrono::system_clock::now();//结束计时
        std::chrono::duration<double> dura = end - start;
        std::cout <<"when n="<< n<< "costed "<<dura.count() << "s" << std::endl;
    }

    return 0;
}
