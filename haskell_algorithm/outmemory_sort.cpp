#include "stdafx.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <iomanip>
#include <iostream>
#include <sstream>
#include "outmemory_sort.h"
using namespace std;

int inmemory_partition(vector<int> &vecSrcData, int nBegin, int nEnd)
{
	//随机化
	srand((unsigned int)time(NULL));
	int nPivotIndex = nBegin + ((rand()) % (nEnd - nBegin + 1)); 
	inmemory_exchange(vecSrcData[nEnd], vecSrcData[nPivotIndex]);

	//partition
	int nPivot = vecSrcData[nEnd];
	int nLessIndex = nBegin - 1;
	for (int i = nBegin; i < nEnd; ++i){
		if (vecSrcData[i] < nPivot){
			++nLessIndex;
			inmemory_exchange(vecSrcData[nLessIndex], vecSrcData[i]);
		} else {
			//do nothing
		}
	}

	//exchange pivot
	++nLessIndex;
	inmemory_exchange(vecSrcData[nLessIndex], vecSrcData[nEnd]);
	return nLessIndex;
}

void inmemory_exchange(int &nL, int &nR)
{
	if (&nL == &nR){
		return;
	} else {
		int nTmp = nL;
		nL = nR;
		nR = nTmp;
	} 	
}

int inmemory_quicksort(vector<int> &vecSrcData, int nBegin, int nEnd)
{
	while (nEnd > nBegin){
		int nPivotIndex = inmemory_partition(vecSrcData, nBegin, nEnd);
		inmemory_quicksort(vecSrcData, nBegin, nPivotIndex - 1);
		//inmemory_quicksort(vecSrcData, nPivotIndex + 1, nEnd);
		nBegin = nPivotIndex + 1;
	}
	return 0;
}

int outmemory_nextKey(ifstream &fout)
{
	char ch;

	fout.get(ch);
	while ((!fout.eof()) && ((' ' == ch) || ('\t' == ch))){
		fout.get(ch);
	}

	int nBufIndex = 0;
	/*< 假设整数均没有大于16位的 */
	char chBuf[16];
	memset(chBuf, 0, 16 *  sizeof(char));

	while ((!fout.eof()) && !((' ' == ch) || ('\t') == ch)){
		chBuf[nBufIndex++] = ch;
		fout.get(ch);
	}

	if (0 != nBufIndex){
		return atoi(chBuf);
	} else {
		return OUTMEMORY_INVALIDINT;
	}
}

int outmemory_merge(const vector<string> &vecSmallFiles, const string &strDstFile)
{
	//多路合并最好使用堆, 不过这里的小文件不会太多， 所以性能差距不大
	vector<pair<int, ifstream *>> vecActAsHeap;

	vector<ifstream *> vecFins;
	for (int i = 0; i < (int)vecSmallFiles.size(); ++i){
		ifstream *pFin = new ifstream;
		pFin->open(vecSmallFiles[i], ios::in);
		if (!pFin->fail()){
			vecFins.push_back(pFin);
		} else {
			cout << "open file:" << vecSmallFiles[i] << " failed." << endl;
			delete pFin;
		}
	}
	ofstream fout;
	fout.open(strDstFile, ios::out | ios::app);
	if (fout.fail()){
		cout << "open file:" << strDstFile << " failed." << endl;
		goto endlabel;
	}
	
	for (int i = 0; i < (int)vecFins.size(); ++i){
		int nKey = outmemory_nextKey(*(vecFins[i]));
		if (OUTMEMORY_INVALIDINT != nKey){
			vecActAsHeap.push_back(make_pair(nKey, vecFins[i]));
		}
	}
	while (!vecActAsHeap.empty()){
		int nMinIndex = outmemory_findmin(vecActAsHeap);

		fout << vecActAsHeap[nMinIndex].first << " ";

		int nKey = outmemory_nextKey(*(vecActAsHeap[nMinIndex].second));
		if (OUTMEMORY_INVALIDINT != nKey){
			vecActAsHeap[nMinIndex].first = nKey;
		} else {
			vecActAsHeap.erase(vecActAsHeap.begin() + nMinIndex);
		}
	}


	fout.close();
endlabel:
	for (int i = 0; i < (int)vecFins.size(); ++i){
		vecFins[i]->close();
		delete vecFins[i];
	}

	return 0;
}

int outmemory_findmin(const vector<pair<int, ifstream *>> &vecActAsHeap)
{
	int nMin = vecActAsHeap[0].first;
	int nIndex = 0;
	for (size_t i = 1; i < vecActAsHeap.size(); ++i){
		if (vecActAsHeap[i].first < nMin){
			nMin = vecActAsHeap[i].first;
			nIndex = i;
		}
	}
	return nIndex;
}

int outmemory_readbigfile(const string &strBigFile, int nKeysNumberLimit, vector<string> &vecmSmallFiles)
{
	ifstream fin(strBigFile, ios::in);
	if (fin.fail()){
		cout << "open file: " << strBigFile << " failed." << endl;
		return -1;
	}

	int nSmallFileIndex = 0;
	vector<int> vecSrcData;
	vecSrcData.reserve(nKeysNumberLimit);
	while (!fin.eof()){
		int nKey = outmemory_nextKey(fin);
		if (OUTMEMORY_INVALIDINT != nKey){
			vecSrcData.push_back(nKey);
			if ((int)vecSrcData.size() >= nKeysNumberLimit){
				outmemory_outputsorteddata(vecSrcData, nSmallFileIndex, vecmSmallFiles);
				vecSrcData.clear();
			}
		}
	}
	if (!vecSrcData.empty()){
		outmemory_outputsorteddata(vecSrcData, nSmallFileIndex, vecmSmallFiles);
	}
	vecSrcData.swap(vector<int>());

	fin.close();

	return 0;
}

int outmemory_outputsorteddata(vector<int> &vecSrcData, int &nSmallFileIndex, vector<string> &vecmSmallFiles)
{
	inmemory_quicksort(vecSrcData, 0, vecSrcData.size() - 1);
				
	string strSmallFilePath = "Q:\\Test\\smallfile_" + outmemory_inttostring(nSmallFileIndex++);
	ofstream fout(strSmallFilePath, ios::out);
	if (fout.fail()){
		cout << "open file: " << strSmallFilePath << " failed." << endl;
		return -1;
	}
	for (int i = 0; i < (int)vecSrcData.size(); ++i){
		fout << vecSrcData[i] << " ";
	}
	fout.close();

	vecmSmallFiles.push_back(strSmallFilePath);

	return 0;
}

string outmemory_inttostring(int nInt)
{
	ostringstream oss;
	oss << nInt;
	return oss.str();
}

void outmemory_sort_test()
{
	cout << "outmemory_sort_test." << endl;

	srand((unsigned int)time(NULL));
	vector<int> vecSrcData;
	for (size_t i = 0; i < 10000; ++i){
		for (size_t j = 0; j < 1000; ++j){
			vecSrcData.push_back(rand() * rand());
		}
	}
	ofstream fout("Q:\\Test\\BigFile", ios::out);
	if (fout.fail()){
		cout << "create bigfile failed." << endl;
		return;
	}
	for (size_t i = 0; i < vecSrcData.size(); ++i){
		fout << vecSrcData[i] << " ";
	}
	fout.close();

	vector<string> vecSmallFiles;
	outmemory_readbigfile("Q:\\Test\\BigFile", 1000000, vecSmallFiles);

	outmemory_merge(vecSmallFiles, "Q:\\Test\\BigFileSorted");
}