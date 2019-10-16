#pragma once
#include <Windows.h>
#include <vector>
#include <stdio.h>
using namespace std;

class readLidar
{
public:
	readLidar();
	void getData();					// 数据获取
public:
	vector<int> vnDistance;			// 3个点距离（mm）
	float fDistanceMean;			// 平均距离（mm）
	vector<float> vfAngle;			// 2个方向角度（°）
private:
	HANDLE hComLidar;
};
