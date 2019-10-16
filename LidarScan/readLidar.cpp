#include "pch.h"
#include "readLidar.h"

readLidar::readLidar()
{
	hComLidar = CreateFile(TEXT("COM3"),//COM3��
		GENERIC_READ, //�����
		0, //��ռ��ʽ
		NULL,
		OPEN_EXISTING, //�򿪶����Ǵ���
		0, //ͬ����ʽ
		NULL);
	if (hComLidar == (HANDLE)-1)
	{
		MessageBox(NULL, _T("COM��ʧ��"), NULL, MB_OK);
	}
	SetupComm(hComLidar, 1024, 1024); //���뻺����������������Ĵ�С����1024
	COMMTIMEOUTS TimeOuts;
	//�趨����ʱ
	TimeOuts.ReadIntervalTimeout = 0;
	TimeOuts.ReadTotalTimeoutMultiplier = 0;
	TimeOuts.ReadTotalTimeoutConstant = 0;
	//�趨д��ʱ
	TimeOuts.WriteTotalTimeoutMultiplier = 0;
	TimeOuts.WriteTotalTimeoutConstant = 0;
	SetCommTimeouts(hComLidar, &TimeOuts); //���ó�ʱ
	DCB dcb;
	GetCommState(hComLidar, &dcb);
	dcb.BaudRate = 9600; //������Ϊ9600
	dcb.ByteSize = 8; //ÿ���ֽ���8λ
	dcb.Parity = NOPARITY; //����żУ��λ
	dcb.StopBits = ONESTOPBIT; //1��ֹͣλ
	SetCommState(hComLidar, &dcb);
}
//******************************** ��ȡ���� ********************************
void readLidar::getData()
{
	unsigned char str[39] = { 0 };
	DWORD wCount = 38;								// ��ȡ���ֽ���
	BOOL bReadStat;
	do
	{
		PurgeComm(hComLidar, PURGE_TXCLEAR | PURGE_RXCLEAR); // ��ջ�����
		bReadStat = ReadFile(hComLidar, str, 39, &wCount, NULL);
		if (!bReadStat)
		{
			MessageBox(NULL, _T("������ʧ��!"), NULL, MB_OK);
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
				
				vnDistance.push_back(int(uTemp[i]));			//printf("����%d��%dmm\n", i, uTemp[i]);
			}

			vfAngle.clear();
			short uTemp1[2] = { 0 };
			for (int i = 0; i < 2; i++)
			{
				uTemp1[i] = uTemp1[i] | str[32 + i * 2];
				uTemp1[i] = uTemp1[i] << 8;
				uTemp1[i] = uTemp1[i] | str[33 + i * 2];
				
				vfAngle.push_back(float(uTemp1[i]) / 100);		//printf("�Ƕ�%d��%.2f��\n", i, float(uTemp1[i]) / 100);
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