#ifndef __OUTMEMORY_SORT_H__
#define __OUTMEMORY_SORT_H__

#include <vector>
#include <string>
#include <fstream>
using namespace std;

#define OUTMEMORY_INVALIDINT 0x7fffffff

/**
 *@brief ��������Ļ��ֲ���
 *@param [in & out] vector<int> &vecSrcData, ��������
 *@param [in] size_t nBegin, ��ʼ�±�λ��
 *@param [in] size_t bEnd, �����±�λ��
 *@return int, Pivot�±�
 *@remarks
 */
int inmemory_partition(vector<int> &vecSrcData, int nBegin, int nEnd);

/**
 *@brief ��������Ԫ��
 */
void inmemory_exchange(int &nL, int &nR);

/**
 *@brief ��������
 */
int inmemory_quicksort(vector<int> &vecSrcData, int nBegin, int nEnd);

/**
 *@brief ��fout������һ������
 *@param [in] ifstream &fout, �ļ�������
 *@return int, ����
 *@remarks
 */
int outmemory_nextKey(ifstream &fout);

/**
 *@brief �����ṩ���ļ����ϣ��������鲢����Ĺ鲢����
 *@param [in] const vector<string> &vecSmallFiles, ԭ�ļ�����
 *@param [in] const string &strDstFile, Ŀ�Ĵ�ŵ�ַ
 *@return int, 0 ok, -1 error
 *@remarks
 */
int outmemory_merge(const vector<string> &vecSmallFiles, const string &strDstFile);

/**
 *@brief �ҵ���С�ؼ��ֵ��±�
 */
int outmemory_findmin(const vector<pair<int, ifstream *>> &vecActAsHeap);

/**
 *@brief ��ȡ���ļ������ΪС�ļ�
 */
int outmemory_readbigfile(const string &strBigFile, int nKeysNumberLimit, vector<string> &vecmSmallFiles);

/**
 *@brief ����������ݵ�С�ļ���
 */
int outmemory_outputsorteddata(vector<int> &vecSrcData, int &nSmallFileIndex, vector<string> &vecmSmallFiles);

/**
 *@brief int ת string
 */
string outmemory_inttostring(int nInt);

/**
 *@brief ����
 */
void outmemory_sort_test();

#endif //__OUTMEMORY_SORT_H__
