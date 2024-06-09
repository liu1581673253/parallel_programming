#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include<fstream>
#include<vector>
#include <stdlib.h>
#include <malloc.h>
#include<bitset>
#include <nmmintrin.h> //SSSE4.2
#include <chrono>
#include<mpi.h>
using namespace std;

struct list {
    uint32_t len;
    uint32_t* l;
};

list* lists;

vector<vector<uint32_t>> querys;
vector<int> ans[1000];

struct arg {
    int idx_b;
    int idx_e;
};



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

bool inlist(list* l, uint32_t target) {//二分查找
    int left = 0;
    int right = l->len - 1;	// 定义了target在左闭右闭的区间内，[left, right]
    uint32_t* nums = l->l;
    while (left <= right) {	//当left == right时，区间[left, right]仍然有效
        int middle = left + ((right - left) / 2);//等同于 (left + right) / 2，防止溢出
        if (nums[middle] > target) {
            right = middle - 1;	//target在左区间，所以[left, middle - 1]
        }
        else if (nums[middle] < target) {
            left = middle + 1;	//target在右区间，所以[middle + 1, right]
        }
        else {	//既不在左边，也不在右边，那就是找到答案了
            return true;
        }
    }
    //没有找到目标值
    return false;


}


int main(int argc, char* argv[]) {

    int myid, numprocs;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
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
    fclose(fi);

    //现在，已经由lists存储了所有的倒排索引列表，下面我们读入测试数据
    fp = fopen("C:\\Users\\rlex\\Desktop\\ExpQuery", "r");


    char* line = new char[100];
    while ((fgets(line, 100, fp)) != NULL) { querys.push_back(str_to_int(line)); }//字符串转化成int数组
    delete line;
    fclose(fp);

    const int MAXN = 80000 * 1024;
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < lists[i].len; j++) {
            if (lists[i].l[j] >= MAXN) {
                lists[i].len = j;
                break;
            }
        }
    }
    auto start = std::chrono::system_clock::now();

    int len_ans[1000];
    for (int i = myid * 1000 / numprocs; i < (myid + 1) * 1000 / numprocs; i++) {
        //找出最短的表
        int table_idx = querys[i][0];
        int min_len = lists[table_idx].len;
        int query_len = querys[i].size();
        for (int j = 1; j < query_len; j++) {
            if (lists[j].len < min_len) { min_len = lists[j].len, table_idx = j; }
        }

        //开始查询
        for (int j = 0; j < min_len; j+=1) {
            int num_now = lists[table_idx].l[j];
            bool flag = 1;//是否在其他表中
            int p = 0;
            while (p < query_len) {
                if (p == table_idx) {
                    p++;
                    continue;
                }
                flag = inlist(&lists[querys[i][p]], num_now);
                if (!flag) { break; }
                p++;
            }
            if (flag) {
                ans[i].push_back(num_now);
            }
        }
    }

    auto end = std::chrono::system_clock::now();//结束计时
    std::chrono::duration<double> dura = end - start;
    std::cout << dura.count() * 1000 << "ms" << std::endl;

    return 0;
}
