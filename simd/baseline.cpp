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

    uint32_t** ans=new uint32_t* [1000];
    int len_ans[1000];
    auto start = std::chrono::system_clock::now();

    for(int i=0;i<1000;i++){
        int idx=querys[i][0];
        ans[i]=new uint32_t[lists[idx].len];//ans�ᱻ���ϸ�д
        int cnt_ans=lists[idx].len;
        for(int j=0;j<cnt_ans;j++) ans[i][j]=lists[idx].l[j];


        for(int j=1;j<querys[i].size();j++){
            //����֮,��temp��
            uint32_t* temp=new uint32_t[lists[idx].len];
            for(int j=0;j<cnt_ans;j++) temp[j]=ans[i][j];

            int idx2=querys[i][j];

            int p1=0,p2=0;
            int cnt_next=0;
            while(p1<cnt_ans&&p2<lists[idx2].len){

                if(temp[p1]>lists[idx2].l[p2]){p2++;}
                else if(temp[p1]==lists[idx2].l[p2]){
                    ans[i][cnt_next]=temp[p1];
                    cnt_next++;
                    p1++,p2++;
                }
                else{p1++;}
            }

            cnt_ans=cnt_next;

            delete[] temp;
        }
        len_ans[i]=cnt_ans;

    }

    auto end = std::chrono::system_clock::now();//������ʱ
    std::chrono::duration<double> dura = end - start;
    std::cout <<dura.count()*1000 << "ms" << std::endl;
    //��������֤��ȷ��
    for(int i=0;i<3;i++){
        cout<<"i="<<i<<endl;
        for(int j=0;j<len_ans[i];j++) cout<<ans[i][j]<<' ';
    }


    return 0;
}
