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
#include <mutex>

using namespace std;
using namespace std::chrono;

int counter = 0;//计数器
std::mutex mtx;//锁
const int N =8;//步长


struct list{
    uint32_t len;
    uint32_t* l;
};

list* lists;
bitset<128>* bit_list_now[1000]; // 答案位图表
bitset<128> bit_list_ans[196916];

bitset<196916> bit_idxs[2000];
vector<vector<uint32_t>> querys;
bitset<128>* bit_lists[2000];


//创建四个线程
pthread_t threads[8];
//定义线程处理函数
void* thread_func_1(void* args){
    auto start1 = std::chrono::steady_clock::now();
    int start=0;
    while(counter<1000){
    mtx.lock();//加锁
    start = counter;
    counter+=N;
    mtx.unlock();
    int toend=start+N;
    for(uint32_t i=start;i<toend;i++){//标识第几个query
        bitset<196916> idx_now=bit_idxs[querys[i][0]];
        idx_now&=bit_idxs[querys[i][1]];

        //copy第一个query对应的表,0,1先求交，从而避免了初始化的问题
        for(int j=0;j<196916;j++){
            if(idx_now[j/8]){//只有索引为1时需要求交
                bit_list_now[i][j]=bit_lists[querys[i][0]][j];
                __m128i xmmA = _mm_load_si128((__m128i*)(bit_lists[querys[i][0]]+j));
                __m128i xmmB = _mm_load_si128((__m128i*)(bit_lists[querys[i][1]]+j));
                __m128i xmmResult = _mm_and_si128(xmmA, xmmB);
                _mm_store_si128((__m128i*)(bit_list_now[i]+j), xmmResult);
            }else{//不需要求交即全设置为0
                j+=7;}}
        for(int j=2;j<querys[i].size();j++){
            idx_now&=bit_idxs[querys[i][j]];
            for(int k=0;k<196916;k++){
            if(idx_now[k/8]){//只有索引为1时需要求交
                bit_list_now[i][j]=bit_lists[querys[i][0]][j];
                __m128i xmmA = _mm_load_si128((__m128i*)(bit_list_now[i]+k));
                __m128i xmmB = _mm_load_si128((__m128i*)(bit_lists[querys[i][j]]+k));
                __m128i xmmResult = _mm_and_si128(xmmA, xmmB);
                _mm_store_si128((__m128i*)(bit_list_now[i]+k), xmmResult);
            }else{//不需要求交即全设置为0
                k+=7;}}}}
    }
    auto end = std::chrono::steady_clock::now();//结束计时
    auto dura = std::chrono::duration_cast<microseconds>(end - start1);
    std::cout <<"thread:"<<start<<' '<<(double)dura.count()<< std::endl;
        return nullptr;
}
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

int main()
{
    //先打开文件读入数据
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
    maxnum=(maxnum/128+1)*128;
    fclose(fi);
    fp=fopen("C:\\Users\\rlex\\Desktop\\ExpQuery","r");


    char* line = new char[100];
    while ((fgets(line, 100, fp)) != NULL){querys.push_back(str_to_int(line));}//字符串转化成int数组
    delete line;
	fclose(fp);

   for(int i=0;i<2000;i++){
        bit_lists[i]=new bitset<128>[196916];
   }
   for(int i=0;i<1000;i++){
        bit_list_now[i]=new bitset<128>[196916];
   }

    for(int i=0;i<count;i++){
        for(int j=0;j<lists[i].len;j++){
            auto temp=lists[i].l[j];
            bit_lists[i][temp/128].set(temp%128);
        }
    }
    //建立索引
    //我的评价是建立128*8的索引，即196916这样，然后先对索引求交，对索引上为1的再进行求交
    for(int i=0;i<2000;i++){
        for(int j=0;j<196916/8;j++){
            bool flag=0;
            for(int k=0;k<8;k++){if(bit_lists[i][j*8+k].any()){flag=1;break;}}
            if(flag){bit_idxs[i].set(j);}
        }
    }
    vector<vector<int>> ans;

    auto start = std::chrono::steady_clock::now();
    pthread_create(threads,NULL,thread_func_1,NULL);
    pthread_create(&threads[1],NULL,thread_func_1,NULL);
    pthread_create(&threads[2],NULL,thread_func_1,NULL);
    pthread_create(&threads[3],NULL,thread_func_1,NULL);
    pthread_create(&threads[4],NULL,thread_func_1,NULL);
    pthread_create(&threads[5],NULL,thread_func_1,NULL);
    pthread_create(&threads[6],NULL,thread_func_1,NULL);
    pthread_create(&threads[7],NULL,thread_func_1,NULL);
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
