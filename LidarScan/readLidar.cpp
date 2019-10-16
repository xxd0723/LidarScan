#include "pch.h"
#include "readLidar.h"

readLidar::readLidar()
{
	hComLidar = CreateFile(TEXT("COM3"),//COM3口
		GENERIC_READ, //允许读
		0, //独占方式
		NULL,
		OPEN_EXISTING, //打开而不是创建
		0, //同步方式
		NULL);
	if (hComLidar == (HANDLE)-1)
	{
		MessageBox(NULL, _T("COM打开失败"), NULL, MB_OK);
	}
	SetupComm(hComLidar, 1024, 1024); //输入缓冲区和输出缓冲区的大小都是1024
	COMMTIMEOUTS TimeOuts;
	//设定读超时
	TimeOuts.ReadIntervalTimeout = 0;
	TimeOuts.ReadTotalTimeoutMultiplier = 0;
	TimeOuts.ReadTotalTimeoutConstant = 0;
	//设定写超时
	TimeOuts.WriteTotalTimeoutMultiplier = 0;
	TimeOuts.WriteTotalTimeoutConstant = 0;
	SetCommTimeouts(hComLidar, &TimeOuts); //设置超时
	DCB dcb;
	GetCommState(hComLidar, &dcb);
	dcb.BaudRate = 9600; //波特率为9600
	dcb.ByteSize = 8; //每个字节有8位
	dcb.Parity = NOPARITY; //无奇偶校验位
	dcb.StopBits = ONESTOPBIT; //1个停止位
	SetCommState(hComLidar, &dcb);
}
//******************************** 获取数据 ********************************
void readLidar::getData()
{
	unsigned char str[39] = { 0 };
	DWORD wCount = 38;								// 读取的字节数
	BOOL bReadStat;
	do
	{
		PurgeComm(hComLidar, PURGE_TXCLEAR | PURGE_RXCLEAR); // 清空缓冲区
		bReadStat = ReadFile(hComLidar, str, 39, &wCount, NULL);
		if (!bReadStat)
		{
			MessageBox(NULL, _T("读串口失败!"), NULL, MB_OK);
		}
		else
		{
			unsigned short uTemp[3] = { 0 };
			vnDistance.clear();
			for (int i = 0; i < 3; i++)
			{
				uTemp[i] = uTemp[i] | str[6 + i * 2];
				uTemp[i] = uTemp[i] << 8;
				uTemp[i] = uTemp[i] | str[7 + i * 2];
				
				vnDistance.push_back(int(uTemp[i]));			//printf("距离%d：%dmm\n", i, uTemp[i]);
			}

			vfAngle.clear();
			short uTemp1[2] = { 0 };
			for (int i = 0; i < 2; i++)
			{
				uTemp1[i] = uTemp1[i] | str[32 + i * 2];
				uTemp1[i] = uTemp1[i] << 8;
				uTemp1[i] = uTemp1[i] | str[33 + i * 2];
				
				vfAngle.push_back(float(uTemp1[i]) / 100);		//printf("角度%d：%.2f°\n", i, float(uTemp1[i]) / 100);
			}
		}
	} while (str[0] != 255 ||
		str[1] != 255 ||
		str[2] != 255 ||
		str[3] != 255 ||
		abs(vnDistance[0] - vnDistance[1]) > 100 ||
		abs(vnDistance[2] - vnDistance[1]) > 200);
	fDistanceMean = float(vnDistance[0] + vnDistance[1] + vnDistance[2]) / 3;
}