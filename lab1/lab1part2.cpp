#include<iostream>

#include<windows.h>
using namespace std;
typedef unsigned long long ll;
const ll n=67108864;
;
long long arr[n];
long long arr2[n];
void func(long long n){
    if(n==1) return ;
    for(ll i=0;i<n/2;i++){
        ll temp=arr[i]+arr[n-i-1];
        n=n/2;
        func(n);
    }
}

int main() {//ƽ������

    long long sum1=0,sum2=0;
    for(ll i=0;i<n;i++) arr[i]=1;
    int cnt=10;
    /*
    DWORD startTime=GetTickCount();
    for(int i=0;i<cnt;i++){
        for(ll i=0;i<n;i++) sum1+=arr[i];
    }


	DWORD endTime=GetTickCount();
    cout<<"ƽ����������ʱ��Ϊ��:"<<endTime-startTime<<"ms"<<endl;*/


     /*for(ll i=0;i<n;i++) arr[i]=1;
     DWORD   startTime=GetTickCount();
    for(ll i=0;i<cnt;i++){
        func(n);
    }


	 DWORD endTime=GetTickCount();
    cout<<"�ݹ鷽������ʱ��Ϊ��:"<<endTime-startTime<<"ms"<<endl;*/

    DWORD startTime=GetTickCount();
    for(int i=0;i<cnt;i++){
        sum1=0,sum2=0;


    for(ll i=0;i<n;i+=2) sum1+=arr[i],sum2+=arr[i+1];
    sum1+=sum2;

    }

	 DWORD endTime=GetTickCount();
    cout<<"��·��ʽ����ʱ��Ϊ��:"<<endTime-startTime<<"ms"<<endl;
	return 0;
}
