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
    bitset<25205248>* bit_lists[2000];
    //开始设置
    for(int i=0;i<count;i++){
        bit_lists[i]=new bitset<25205248>;
        for(int j=0;j<lists[i].len;j++){
            bit_lists[i]->set(lists[i].l[j]);
        }
    }
    vector<vector<uint32_t>> ans;
    //最后进行求交，理论上时间计算只计算这一部分，算法的差异也仅在于这一部分
    auto start = std::chrono::system_clock::now();
    bitset<25205248>* temp_bit=new bitset<25205248>;
    for(int i=0;i<1000;i++){

        *temp_bit=*bit_lists[querys[i][0]];//不断加载与temp_bit求交
        for(auto iter:querys[i]){
            *temp_bit&=(*bit_lists[iter]);
        }

    }

    auto end = std::chrono::system_clock::now();//结束计时
    std::chrono::duration<double> dura = end - start;
    std::cout <<dura.count()*1000 << "ms" << std::endl;

    for(int i=0;i<count;i++)delete bit_lists[i];
    return 0;
}
