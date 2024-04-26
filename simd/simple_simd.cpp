#include <iostream>
#include<fstream>
#include<vector>
#include <stdlib.h>
#include <malloc.h>
#include<iostream>
#include<bitset>
#include <nmmintrin.h> //SSSE4.2
#include <chrono>
using namespace std;

struct list{
    uint32_t len;
    uint32_t* l;
};

list* lists;
bitset<128> bit_list_now[196916];
bitset<128> bit_list_ans[196916];
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

    while(!feof(fi)){
        fread(&len_now, 4, 1, fi);
        if(feof(fi)){break;}
        lists[count].len=len_now;
        lists[count].l = (uint32_t *)malloc(len_now * 4);
        if(lists[count].l==NULL){return 1;}
        for(int i=0;i<len_now;i++){
            fread(&lists[count].l[i], 4, 1, fi);
            if(feof(fi)){break;}
            if(lists[count].l[i]>maxnum){maxnum=lists[count].l[i];}
        }
        count++;
    }
    //由于我们用128位的方式并行，我们把maxnum转化成128的倍数
    maxnum=(maxnum/128+1)*128;
    //cout<<maxnum;//maxnum=25205248,为了方便，这里我们使用静态的bitmap
    fclose(fi);
    //现在，已经由lists存储了所有的倒排索引列表，下面我们读入测试数据
    fp=fopen("C:\\Users\\rlex\\Desktop\\ExpQuery","r");

    vector<vector<uint32_t>> querys;
    char* line = new char[100];
    while ((fgets(line, 100, fp)) != NULL){querys.push_back(str_to_int(line));}//字符串转化成int数组
    delete line;
	fclose(fp);
    //for(auto iter1:querys) for(auto iter2:iter1) cout<<iter2<<' ';

    //下面将倒排列表转化成位图化表示
   //由于整块化成位图会导致读写冲突，只能分块进行了
   bitset<128>* bit_lists[2000];
   for(int i=0;i<2000;i++){
        bit_lists[i]=new bitset<128>[196916];
   }
    //开始设置
    for(int i=0;i<count;i++){
        for(int j=0;j<lists[i].len;j++){
            auto temp=lists[i].l[j];
            bit_lists[i][temp/128].set(temp%128);
        }
    }

    vector<vector<int>> ans;
    //最后进行求交，理论上时间计算只计算这一部分，算法的差异也仅在于这一部分
    auto start = std::chrono::system_clock::now();
    for(int i=0;i<1000;i++){//标识第几个query
        //copy第一个query对应的表
        for(int j=0;j<196916;j++){
            bit_list_now[j]=bit_lists[querys[i][0]][j];
        }
        for(int j=1;j<querys[i].size();j++){//标识和第几个query对应的list求交
            bitset<128>*ptr=bit_lists[querys[i][j]];
            for(int k=0;k<196916;k++){
                __m128i xmmA = _mm_load_si128((__m128i*)(bit_list_now+k));
                __m128i xmmB = _mm_load_si128((__m128i*)(ptr+k));


                __m128i xmmResult = _mm_and_si128(xmmA, xmmB);

                // 存储结果
                _mm_store_si128((__m128i*)(bit_list_now+k), xmmResult);
            }
        }
        //结果暂存在bit_list_now


    }


    auto end = std::chrono::system_clock::now();//结束计时
    std::chrono::duration<double> dura = end - start;
    std::cout <<dura.count()*1000 << "ms" << std::endl;

    //for(int i=0;i<count;i++)delete bit_lists[i];
    return 0;
}
