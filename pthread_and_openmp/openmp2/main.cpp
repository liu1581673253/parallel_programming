#include <iostream>
#include<fstream>
#include<vector>
#include <stdlib.h>
#include <malloc.h>
#include<iostream>
#include<bitset>
#include <nmmintrin.h> //SSSE4.2
#include <chrono>
#include<omp.h>
#include<pthread.h>

using namespace std;

struct list{
    uint32_t len;
    uint32_t* l;
};

list* lists;

vector<vector<uint32_t>> querys;
vector<int> ans[1000];

struct arg{
    int idx_b;
    int idx_e;
};

//�̺߳���
void* thread_func(void* args){


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

bool inlist(list* l,uint32_t target){//���ֲ���
    int left = 0;
    int right = l->len - 1;	// ������target������ұյ������ڣ�[left, right]
    uint32_t* nums=l->l;
    while (left <= right) {	//��left == rightʱ������[left, right]��Ȼ��Ч
        int middle = left + ((right - left) / 2);//��ͬ�� (left + right) / 2����ֹ���
        if (nums[middle] > target) {
            right = middle - 1;	//target�������䣬����[left, middle - 1]
        } else if (nums[middle] < target) {
            left = middle + 1;	//target�������䣬����[middle + 1, right]
        } else {	//�Ȳ�����ߣ�Ҳ�����ұߣ��Ǿ����ҵ�����
            return true;
        }
    }
    //û���ҵ�Ŀ��ֵ
    return false;


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

    //���ڣ��Ѿ���lists�洢�����еĵ��������б��������Ƕ����������
    fp=fopen("C:\\Users\\rlex\\Desktop\\ExpQuery","r");


    char* line = new char[100];
    while ((fgets(line, 100, fp)) != NULL){querys.push_back(str_to_int(line));}//�ַ���ת����int����
    delete line;
	fclose(fp);



    auto start = std::chrono::system_clock::now();

    int len_ans[1000];
     #pragma omp parallel for  schedule(dynamic,10) num_threads(4)
    for(int i=0;i<1000;i++){
        //�ҳ���̵ı�
        int table_idx = querys[i][0];
        int min_len = lists[table_idx].len;
        int query_len=querys[i].size();
        for(int j=1;j<query_len;j++){
            if(lists[j].len<min_len){min_len=lists[j].len,table_idx=j;}
        }
        for(int j=0;j<min_len;j++){
            int num_now = lists[table_idx].l[j];
            bool flag =1;//�Ƿ�����������
            int p=0;
            while(p<query_len){
                if(p==table_idx){
                    p++;
                    continue;
                }
                flag = inlist(&lists[querys[i][p]],num_now);
                if(!flag){break;}
                p++;}
            if(flag){
                ans[i].push_back(num_now);
            }}}
    auto end = std::chrono::system_clock::now();//������ʱ
    std::chrono::duration<double> dura = end - start;
    std::cout <<dura.count()*1000  << std::endl;

    //for(auto iter:ans[2]) cout<<iter<<endl;

    return 0;
}
