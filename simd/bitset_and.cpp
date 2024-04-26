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
    //�ȴ��ļ���������
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
    bitset<25205248>* bit_lists[2000];
    //��ʼ����
    for(int i=0;i<count;i++){
        bit_lists[i]=new bitset<25205248>;
        for(int j=0;j<lists[i].len;j++){
            bit_lists[i]->set(lists[i].l[j]);
        }
    }
    vector<vector<uint32_t>> ans;
    //�������󽻣�������ʱ�����ֻ������һ���֣��㷨�Ĳ���Ҳ��������һ����
    auto start = std::chrono::system_clock::now();
    bitset<25205248>* temp_bit=new bitset<25205248>;
    for(int i=0;i<1000;i++){

        *temp_bit=*bit_lists[querys[i][0]];//���ϼ�����temp_bit��
        for(auto iter:querys[i]){
            *temp_bit&=(*bit_lists[iter]);
        }

    }

    auto end = std::chrono::system_clock::now();//������ʱ
    std::chrono::duration<double> dura = end - start;
    std::cout <<dura.count()*1000 << "ms" << std::endl;

    for(int i=0;i<count;i++)delete bit_lists[i];
    return 0;
}
