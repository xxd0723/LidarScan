#include "pch.h"
#include "URControlClass.h"


URControlClass::URControlClass()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);		//��ʼ���汾
	sock = socket(PF_INET, SOCK_STREAM, 0);		//�����û���socket

	//���������������
	sockaddr_in sockAddr;								//����sockAddr�ṹ��
	memset(&sockAddr, 0, sizeof(sockAddr));				 //ÿ���ֽڶ���0���
	sockAddr.sin_family = PF_INET;
	sockAddr.sin_port = htons(30003);
	sockAddr.sin_addr.s_addr = inet_addr("192.168.1.6");

	connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));		//��������
}
//******************************** ƽ�Ƽ���ת ********************************
void URControlClass::moveTo(double px, double py, double pz, double rx, double ry, double rz)
{
	char a[MAX_PATH * 2];
	string str;
	
	char cxp[100], cyp[100], czp[100], cxr[100], cyr[100], czr[100];

	sprintf_s(cxp, "%.3lf\0", px);
	sprintf_s(cyp, "%.3lf\0", py);
	sprintf_s(czp, "%.3lf\0", pz);
	sprintf_s(cxr, "%.3lf\0", rx);
	sprintf_s(cyr, "%.3lf\0", ry);
	sprintf_s(czr, "%.3lf\0", rz);
	
	str = "def funcName() :\n  movel(p[" + string(cxp) + ", " + string(cyp) + ", " + string(czp) + ", " + string(cxr) + ", " + string(cyr) + ", " + string(czr) + "], a = 0.3, v = 0.3)\nend \n";

	/*CString ch(str.c_str());
	MessageBoxW(NULL, ch, NULL, MB_OK);	//���Գ���������������������������������������*/

	strcpy_s(a, str.c_str());
	send(sock, a, strlen(a) + 1, 0);
}

//******************************** ƽ�� ********************************
void URControlClass::shiftTo(double px, double py, double pz)
{
	double rx, ry, rz;
	getAngle(rx, ry, rz);
	moveTo(px, py, pz, rx, ry, rz);
}
//******************************** ��ȡƽ�� ********************************
void URControlClass::getPosition(double& px, double& py, double& pz)
{
	//���������������
	SOCKET sock = socket(PF_INET, SOCK_STREAM, 0);		//�����û���socket

	//���������������
	sockaddr_in sockAddr;								//����sockAddr�ṹ��
	memset(&sockAddr, 0, sizeof(sockAddr));				 //ÿ���ֽڶ���0���
	sockAddr.sin_family = PF_INET;
	sockAddr.sin_port = htons(30003);
	sockAddr.sin_addr.s_addr = inet_addr("192.168.1.6");

	connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));		//��������

	char a[MAX_PATH * 2];

	recv(sock, a, 444, 0);		// ͷ�����޹�����
	recv(sock, a, 8, 0);
	px = c2d(a);
	recv(sock, a, 8, 0);
	py = c2d(a);
	recv(sock, a, 8, 0);
	pz = c2d(a);
}

//******************************** ��ȡ�Ƕ� ********************************
void URControlClass::getAngle(double& rx, double& ry, double& rz)
{
	//���������������
	SOCKET sock = socket(PF_INET, SOCK_STREAM, 0);		//�����û���socket

	//���������������
	sockaddr_in sockAddr;								//����sockAddr�ṹ��
	memset(&sockAddr, 0, sizeof(sockAddr));				 //ÿ���ֽڶ���0���
	sockAddr.sin_family = PF_INET;
	sockAddr.sin_port = htons(30003);
	sockAddr.sin_addr.s_addr = inet_addr("192.168.1.6");

	connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));		//��������

	char a[MAX_PATH * 2];

	recv(sock, a, 468, 0);		// ͷ�����޹�����
	recv(sock, a, 8, 0);
	rx = c2d(a);
	recv(sock, a, 8, 0);
	ry = c2d(a);
	recv(sock, a, 8, 0);
	rz = c2d(a);
}

//******************************** �ַ���ת������ ********************************
double URControlClass::c2d(char* a)
{
	double xP;
	char chs[8];
	for (int i = 0; i < 8; i++)
	{
		chs[i] = a[7 - i];
	}
	xP = *(double*)chs;
	return xP;
}
//******************************** �����߲����� ********************************
void URControlClass::WalkingWay(vector<Point3d>& pointWay)
{
	/*CString cstr;
	cstr.Format(_T("%d"), pointWay.size());
	MessageBox(NULL, cstr, NULL, MB_OK);	//���Գ���������������������������������������*/
	double rx, ry, rz;
	for (int i = 0; i < pointWay.size(); i++)
	{
		Point3d pointTemp;
		pointTemp = pointWay[i];
		shiftTo(pointTemp.x, pointTemp.y, pointTemp.z);
		if (i < pointWay.size() - 1)
		{
			while (1)
			{
				double px, py, pz;
				getPosition(px, py, pz);
				if (abs(pointTemp.x - px) < 0.01 &&
					abs(pointTemp.y - py) < 0.01 &&
					abs(pointTemp.z - pz) < 0.01)
					break;
			}
		}
	}
}
