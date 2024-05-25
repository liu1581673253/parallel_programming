#include <iostream>
#include<fstream>
#include<vector>
#include <stdlib.h>
#include <malloc.h>
#include<iostream>
#include<bitset>
#include <nmmintrin.h> //SSSE4.2
#include <chrono>
#include<pthread.h>

using namespace std;
using namespace std::chrono;
struct list{
    uint32_t len;
    uint32_t* l;
};

list* lists;

vector<vector<uint32_t>> querys;
vector<int> ans[8000];

struct thread_arg{
    int begin_idx;

};
const int N =8;



vector<uint32_t> str_to_int(char* line){
    vector<uint32_t>arr;
    uint32_t int_temp=0;
    char char_now;
    int p=0;
    while(line[p]!=0){
        if(line[p]==' '){
            arr.push_back(int_temp);
            int_temp=0;
            p++;
        }else{
            int num_p=line[p]-'0';
            int_temp=int_temp*10+num_p;
            p++;
        }
    }
    return arr;
}

bool inlist(list* l,uint32_t target){//二分查找
    int left = 0;
    int right = l->len - 1;	// 定义了target在左闭右闭的区间内，[left, right]
    uint32_t* nums=l->l;
    while (left <= right) {	//当left == right时，区间[left, right]仍然有效
        int middle = left + ((right - left) / 2);//等同于 (left + right) / 2，防止溢出
        if (nums[middle] > target) {
            right = middle - 1;	//target在左区间，所以[left, middle - 1]
        } else if (nums[middle] < target) {
            left = middle + 1;	//target在右区间，所以[middle + 1, right]
        } else {	//既不在左边，也不在右边，那就是找到答案了
            return true;
        }
    }
    //没有找到目标值
    return false;


}


//线程函数
void* thread_func(void* args){

    int b = ((thread_arg*) args) ->begin_idx;
    for(int i=0;i<1000;i++){
        //找出最短的表
        int table_idx = querys[i][0];
        int min_len = lists[table_idx].len;
        int query_len=querys[i].size();
        for(int j=1;j<query_len;j++){
            if(lists[j].len<min_len){min_len=lists[j].len,table_idx=j;}
        }

        //开始查询
        for(int j=b;j<min_len;j+=N){
            int num_now = lists[table_idx].l[j];
            bool flag =1;//是否在其他表中
            int p=0;
            while(p<query_len){
                if(p==table_idx){
                    p++;
                    continue;
                }
                flag = inlist(&lists[querys[i][p]],num_now);
                if(!flag){break;}
                p++;
            }
            if(flag){
                ans[i*N+b].push_back(num_now);
            }


        }


    }

    return nullptr;
}

int main()
{
    FILE *fi;
    FILE *fp;

    fi = fopen("C:\\Users\\rlex\\Desktop\\ExpIndex" ,"rb");
    lists=(list *)malloc(2000 * sizeof(list));
    uint32_t len_now;
    uint32_t count=0;
    uint32_t maxnum=0;
    int a1,a2;
    while(!feof(fi)){
        fread(&len_now, 4, 1, fi);
        if(feof(fi)){break;}
        lists[count].len=len_now;
        lists[count].l = (uint32_t *)malloc(len_now * 4);
        if(lists[count].l==NULL){return 1;}
        for(int i=0;i<len_now;i++){
            fread(&lists[count].l[i], 4, 1, fi);
            if(feof(fi)){break;}
            if(lists[count].l[i]>maxnum){maxnum=lists[count].l[i];a1=count,a2=i;}
        }
        count++;
    }
    fclose(fi);

    //现在，已经由lists存储了所有的倒排索引列表，下面我们读入测试数据
    fp=fopen("C:\\Users\\rlex\\Desktop\\ExpQuery","r");


    char* line = new char[100];
    while ((fgets(line, 100, fp)) != NULL){querys.push_back(str_to_int(line));}//字符串转化成int数组
    delete line;
	fclose(fp);


    //创建四个线程
    pthread_t threads[8];
    auto start = std::chrono::steady_clock::now();
    thread_arg arg1={0};
    thread_arg arg2={1};
    thread_arg arg3={2};
    thread_arg arg4={3};
    thread_arg arg5={4};
    thread_arg arg6={5};
    thread_arg arg7={6};
    thread_arg arg8={7};


    pthread_create(threads,NULL,thread_func,&arg1);
    pthread_create(&threads[1],NULL,thread_func,&arg2);
    pthread_create(&threads[2],NULL,thread_func,&arg3);
    pthread_create(&threads[3],NULL,thread_func,&arg4);

    pthread_create(&threads[4],NULL,thread_func,&arg5);
    pthread_create(&threads[5],NULL,thread_func,&arg6);
    pthread_create(&threads[6],NULL,thread_func,&arg7);
    pthread_create(&threads[7],NULL,thread_func,&arg8);
    pthread_join(threads[0],NULL);
    pthread_join(threads[1],NULL);
    pthread_join(threads[2],NULL);
    pthread_join(threads[3],NULL);
    pthread_join(threads[4],NULL);
    pthread_join(threads[5],NULL);
    pthread_join(threads[6],NULL);
    pthread_join(threads[7],NULL);


    auto end = std::chrono::steady_clock::now();//结束计时
    auto dura = std::chrono::duration_cast<microseconds>(end - start);
    std::cout <<(double)dura.count()/1000 << "ms" << std::endl;

    return 0;
}
