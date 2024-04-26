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

 bitset<196916> bit_idxs[2000];
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
    //�ȴ��ļ���������
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
    //cout<<a1<<' '<<a2;
    //����������128λ�ķ�ʽ���У����ǰ�maxnumת����128�ı���
    maxnum=(maxnum/128+1)*128;
    //cout<<maxnum;//maxnum=25205248,Ϊ�˷��㣬��������ʹ�þ�̬��bitmap
    fclose(fi);
    //���ڣ��Ѿ���lists�洢�����еĵ��������б��������Ƕ����������
    fp=fopen("C:\\Users\\rlex\\Desktop\\ExpQuery","r");

    vector<vector<uint32_t>> querys;
    char* line = new char[100];
    while ((fgets(line, 100, fp)) != NULL){querys.push_back(str_to_int(line));}//�ַ���ת����int����
    delete line;
	fclose(fp);
    //for(auto iter1:querys) for(auto iter2:iter1) cout<<iter2<<' ';

    //���潫�����б�ת����λͼ����ʾ
   //�������黯��λͼ�ᵼ�¶�д��ͻ��ֻ�ֿܷ������
   bitset<128>* bit_lists[2000];
   for(int i=0;i<2000;i++){
        bit_lists[i]=new bitset<128>[196916];
   }
    /*for(int i=0;i<1000;i++){
        cout<<lists[i].len<<' ';
    }*/
    //��ʼ����
    for(int i=0;i<count;i++){
        for(int j=0;j<lists[i].len;j++){
            auto temp=lists[i].l[j];
            bit_lists[i][temp/128].set(temp%128);
        }
    }
    //��������
    //�ҵ������ǽ���128*8����������196916������Ȼ���ȶ������󽻣���������Ϊ1���ٽ�����
    for(int i=0;i<2000;i++){
        for(int j=0;j<196916/8;j++){
            bool flag=0;
            for(int k=0;k<8;k++){if(bit_lists[i][j*8+k].any()){flag=1;break;}}
            if(flag){bit_idxs[i].set(j);}
        }
    }
    vector<vector<int>> ans;
    //�������󽻣�������ʱ�����ֻ������һ���֣��㷨�Ĳ���Ҳ��������һ����



    auto start = std::chrono::system_clock::now();
    for(int i=0;i<1000;i++){//��ʶ�ڼ���query
        bitset<196916> idx_now=bit_idxs[querys[i][0]];
        idx_now&=bit_idxs[querys[i][1]];
        //copy��һ��query��Ӧ�ı�,0,1���󽻣��Ӷ������˳�ʼ��������
        for(int j=0;j<196916;j++){
            if(idx_now[j/8]){//ֻ������Ϊ1ʱ��Ҫ��
                bit_list_now[j]=bit_lists[querys[i][0]][j];
                __m128i xmmA = _mm_load_si128((__m128i*)(bit_lists[querys[i][0]]+j));
                __m128i xmmB = _mm_load_si128((__m128i*)(bit_lists[querys[i][1]]+j));
                __m128i xmmResult = _mm_and_si128(xmmA, xmmB);
                _mm_store_si128((__m128i*)(bit_list_now+j), xmmResult);
            }else{//����Ҫ�󽻼�ȫ����Ϊ0
                j+=7;}}
        for(int j=2;j<querys[i].size();j++){
            idx_now&=bit_idxs[querys[i][j]];
            for(int k=0;k<196916;k++){
            if(idx_now[k/8]){//ֻ������Ϊ1ʱ��Ҫ��
                bit_list_now[j]=bit_lists[querys[i][0]][j];
                __m128i xmmA = _mm_load_si128((__m128i*)(bit_list_now+k));
                __m128i xmmB = _mm_load_si128((__m128i*)(bit_lists[querys[i][j]]+k));
                __m128i xmmResult = _mm_and_si128(xmmA, xmmB);
                _mm_store_si128((__m128i*)(bit_list_now+k), xmmResult);
            }else{//����Ҫ�󽻼�ȫ����Ϊ0
                k+=7;}}}}


    auto end = std::chrono::system_clock::now();//������ʱ
    std::chrono::duration<double> dura = end - start;
    std::cout <<dura.count()*1000 << "ms" << std::endl;
    /*for(int i=0;i<196916;i++){
        for(int j=0;j<128;j++){
            if(bit_list_now[i][j]==1) cout<<i*128+j<<' ';
        }
    }*/
    //for(int i=0;i<count;i++)delete bit_lists[i];

    return 0;
}
