#pragma once
#include <Windows.h>
#include <vector>
#include <stdio.h>
using namespace std;

class readLidar
{
public:
	readLidar();
	void getData();					// ���ݻ�ȡ
public:
	vector<int> vnDistance;			// 3������루mm��
	float fDistanceMean;			// ƽ�����루mm��
	vector<float> vfAngle;			// 2������Ƕȣ��㣩
private:
	HANDLE hComLidar;
};
