#ifndef __OUTMEMORY_SORT_H__
#define __OUTMEMORY_SORT_H__

#include <vector>
#include <string>
#include <fstream>
using namespace std;

#define OUTMEMORY_INVALIDINT 0x7fffffff

/**
 *@brief 快速排序的划分部分
 *@param [in & out] vector<int> &vecSrcData, 输入数据
 *@param [in] size_t nBegin, 开始下标位置
 *@param [in] size_t bEnd, 结束下标位置
 *@return int, Pivot下标
 *@remarks
 */
int inmemory_partition(vector<int> &vecSrcData, int nBegin, int nEnd);

/**
 *@brief 交换两个元素
 */
void inmemory_exchange(int &nL, int &nR);

/**
 *@brief 快速排序
 */
int inmemory_quicksort(vector<int> &vecSrcData, int nBegin, int nEnd);

/**
 *@brief 从fout读出下一个整数
 *@param [in] ifstream &fout, 文件读入流
 *@return int, 整数
 *@remarks
 */
int outmemory_nextKey(ifstream &fout);

/**
 *@brief 根据提供的文件集合，将其做归并排序的归并操作
 *@param [in] const vector<string> &vecSmallFiles, 原文件集合
 *@param [in] const string &strDstFile, 目的存放地址
 *@return int, 0 ok, -1 error
 *@remarks
 */
int outmemory_merge(const vector<string> &vecSmallFiles, const string &strDstFile);

/**
 *@brief 找到最小关键字的下标
 */
int outmemory_findmin(const vector<pair<int, ifstream *>> &vecActAsHeap);

/**
 *@brief 读取大文件，存放为小文件
 */
int outmemory_readbigfile(const string &strBigFile, int nKeysNumberLimit, vector<string> &vecmSmallFiles);

/**
 *@brief 输出有序数据到小文件中
 */
int outmemory_outputsorteddata(vector<int> &vecSrcData, int &nSmallFileIndex, vector<string> &vecmSmallFiles);

/**
 *@brief int 转 string
 */
string outmemory_inttostring(int nInt);

/**
 *@brief 测试
 */
void outmemory_sort_test();

#endif //__OUTMEMORY_SORT_H__
