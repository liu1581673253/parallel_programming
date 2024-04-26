#include<iostream>
#include<chrono>
#include<cmath>
using namespace std;
typedef unsigned long long ll;
const ll MAXN=pow(2,26);
ll arr[MAXN];
ll ans3=0;//�ݹ鷽������
void func(ll n){
    if(n==1) return;
    for(ll i=0;i<n/2;i++){
        arr[i]=arr[i]+arr[n-i-1];
        n=n/2;
        func(n);
    }
}

int main(){
    //��ʼ������
    for(int i=0;i<MAXN;i++){
        arr[i]=1;
    }

    cout<<"method 1:"<<endl;
    for(int n=pow(2,16);n<=MAXN;n*=2){ //
        auto start = std::chrono::system_clock::now();//��ʼ��ʱ
        //ƽ������
        int count = 10;//ѭ������������ʱ��
        while (count > 0) {
        ll ans=0;
		for(ll i=0;i<n;i++) ans+=arr[i];
		count--;
        }


        auto end = std::chrono::system_clock::now();//������ʱ
        std::chrono::duration<double> dura = end - start;
        std::cout <<"when n="<< n<< "costed "<<dura.count() << "s" << std::endl;
    }

    cout<<"method 2:"<<endl;
    for(int n=pow(2,16);n<=MAXN;n*=2){ //
        auto start = std::chrono::system_clock::now();//��ʼ��ʱ
        //��·��ʽ

        int count = 10;//ѭ������������ʱ��
        while (count > 0) {
        ll sum1=0,sum2=0;
		for(ll i=0;i<n;i+=2) sum1+=arr[i],sum2+=arr[i+1];
		sum1+=sum2;
		count--;
        }


        auto end = std::chrono::system_clock::now();//������ʱ
        std::chrono::duration<double> dura = end - start;
        std::cout <<"when n="<< n<< "costed "<<dura.count() << "s" << std::endl;
    }

    cout<<"method 3:"<<endl;
    for(int n=pow(2,16);n<=MAXN;n*=2){ //
        //�ݹ�
        double time=0.0;
        int count = 10;//ѭ������������ʱ��
        while (count > 0) {
        auto start = std::chrono::system_clock::now();//��ʼ��ʱ
        func(n);
        auto end = std::chrono::system_clock::now();//������ʱ
        std::chrono::duration<double> dura = end - start;
        time+=dura.count();
        count--;
        }


        //��ʼ������
        for(int i=0;i<MAXN;i++){
            arr[i]=1;
        }

        std::cout <<"when n="<< n<< "costed "<<time << "s" << std::endl;
    }

    cout<<"method 4:"<<endl;
    for(int n=pow(2,16);n<=MAXN;n*=2){ //
        //�ݹ�
        double time=0.0;
        int count = 10;//ѭ������������ʱ��
        while (count > 0) {
        auto start = std::chrono::system_clock::now();//��ʼ��ʱ
        for(ll i=n;i>1;i/=2){
            for(ll j=0;j<i/2;j++){
                arr[j]=arr[j*2]+arr[j*2+1];
            }
        }
        auto end = std::chrono::system_clock::now();//������ʱ
        std::chrono::duration<double> dura = end - start;
        time+=dura.count();
        count--;
        }


        //��ʼ������
        for(int i=0;i<MAXN;i++){
            arr[i]=1;
        }

        std::cout <<"when n="<< n<< "costed "<<time << "s" << std::endl;
    }

    return 0;
}
