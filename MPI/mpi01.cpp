#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include<fstream>
#include<vector>
#include <stdlib.h>
#include <malloc.h>
#include<iostream>
#include<bitset>
#include <nmmintrin.h> //SSSE4.2
#include <chrono>
#include<mpi.h>

using namespace std;
int MAXN = 100000;
struct list {
    uint32_t len;
    uint32_t* l;
};

list* lists;
bitset<128> bit_list_now[196916];
bitset<128> bit_list_ans[196916];

bitset<196916> bit_idxs[2000];
vector<uint32_t> str_to_int(char* line) {
    vector<uint32_t>arr;
    uint32_t int_temp = 0;
    char char_now;
    int p = 0;
    while (line[p] != 0) {
        if (line[p] == ' ') {
            arr.push_back(int_temp);
            int_temp = 0;
            p++;
        }
        else {
            int num_p = line[p] - '0';
            int_temp = int_temp * 10 + num_p;
            p++;
        }
    }
    return arr;
}


int main(int argc, char* argv[]) {

	int myid, numprocs;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

    cout << myid << ' ' << numprocs << endl;
    //先打开文件读入数据
    FILE* fi;
    FILE* fp;

    fi = fopen("C:\\Users\\rlex\\Desktop\\ExpIndex", "rb");
    lists = (list*)malloc(2000 * sizeof(list));
    uint32_t len_now;
    uint32_t count = 0;
    uint32_t maxnum = 0;
    int a1, a2;
    while (!feof(fi)) {
        fread(&len_now, 4, 1, fi);
        if (feof(fi)) { break; }
        lists[count].len = len_now;
        lists[count].l = (uint32_t*)malloc(len_now * 4);
        if (lists[count].l == NULL) { return 1; }
        for (int i = 0; i < len_now; i++) {
            fread(&lists[count].l[i], 4, 1, fi);
            if (feof(fi)) { break; }
            if (lists[count].l[i] > maxnum) { maxnum = lists[count].l[i]; a1 = count, a2 = i; }
        }
        count++;
    }
    std::cout << "DEBUG:333 " << myid << std::endl;
    //cout<<a1<<' '<<a2;
    //由于我们用128位的方式并行，我们把maxnum转化成128的倍数
    maxnum = (maxnum / 128 + 1) * 128;
    //cout<<maxnum;//maxnum=25205248,为了方便，这里我们使用静态的bitmap
    fclose(fi);
    //现在，已经由lists存储了所有的倒排索引列表，下面我们读入测试数据
    fp = fopen("C:\\Users\\rlex\\Desktop\\ExpQuery", "r");

    vector<vector<uint32_t>> querys;
    char* line = new char[100];
    while ((fgets(line, 100, fp)) != NULL) { querys.push_back(str_to_int(line)); }//字符串转化成int数组
    delete line;
    fclose(fp);
    //for(auto iter1:querys) for(auto iter2:iter1) cout<<iter2<<' ';
    std::cout << "DEBUG:222 " << myid << std::endl;
    //下面将倒排列表转化成位图化表示
   //由于整块化成位图会导致读写冲突，只能分块进行了
    bitset<128>* bit_lists[2000];
    for (int i = 0; i < 2000; i++) {
        bit_lists[i] = new bitset<128>[196916];
    }
    /*for(int i=0;i<1000;i++){
        cout<<lists[i].len<<' ';
    }*/
    //开始设置
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < lists[i].len; j++) {
            auto temp = lists[i].l[j];
            bit_lists[i][temp / 128].set(temp % 128);
        }
    }
    //建立索引
    //我的评价是建立128*8的索引，即196916这样，然后先对索引求交，对索引上为1的再进行求交
    for (int i = 0; i < 2000; i++) {
        for (int j = 0; j < 196916 / 8; j++) {
            bool flag = 0;
            for (int k = 0; k < 8; k++) { if (bit_lists[i][j * 8 + k].any()) { flag = 1; break; } }
            if (flag) { bit_idxs[i].set(j); }
        }
    }
    vector<vector<int>> ans;
    //最后进行求交，理论上时间计算只计算这一部分，算法的差异也仅在于这一部分


    std::cout << "DEBUG:111 "<<myid << std::endl;
    auto start = std::chrono::system_clock::now();
    for (int i = myid*100/ numprocs; i < (myid+1) * 100 / numprocs; i++) {//标识第几个query
        bitset<196916> idx_now = bit_idxs[querys[i][0]];
        idx_now &= bit_idxs[querys[i][1]];
        //copy第一个query对应的表,0,1先求交，从而避免了初始化的问题
        for (int j = 0; j < 196916; j++) {
            if (idx_now[j / 8]) {//只有索引为1时需要求交
                bit_list_now[j] = bit_lists[querys[i][0]][j];
                __m128i xmmA = _mm_load_si128((__m128i*)(bit_lists[querys[i][0]] + j));
                __m128i xmmB = _mm_load_si128((__m128i*)(bit_lists[querys[i][1]] + j));
                __m128i xmmResult = _mm_and_si128(xmmA, xmmB);
                _mm_store_si128((__m128i*)(bit_list_now + j), xmmResult);
            }
            else {//不需要求交即全设置为0
                j += 7;
            }
        }
        for (int j = 2; j < querys[i].size(); j++) {
            idx_now &= bit_idxs[querys[i][j]];
            for (int k = 0; k < 196916; k++) {
                if (idx_now[k / 8]) {//只有索引为1时需要求交
                    bit_list_now[j] = bit_lists[querys[i][0]][j];
                    __m128i xmmA = _mm_load_si128((__m128i*)(bit_list_now + k));
                    __m128i xmmB = _mm_load_si128((__m128i*)(bit_lists[querys[i][j]] + k));
                    __m128i xmmResult = _mm_and_si128(xmmA, xmmB);
                    _mm_store_si128((__m128i*)(bit_list_now + k), xmmResult);
                }
                else {//不需要求交即全设置为0
                    k += 7;
                }
            }
        }
    }


    auto end = std::chrono::system_clock::now();//结束计时
    std::chrono::duration<double> dura = end - start;
    std::cout << dura.count() * 1000 << "ms" << std::endl;
	
	return 0;
}
