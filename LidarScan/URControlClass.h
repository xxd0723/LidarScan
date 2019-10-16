#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <WinSock2.h>
#include <string>
#include <time.h>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;
#pragma comment(lib, "ws2_32.lib")
class URControlClass
{
public:
	URControlClass();
	void moveTo(double px, double py, double pz, double rx, double ry, double rz);
	void shiftTo(double px, double py, double pz);
	void getPosition(double& rx, double& ry, double& rz);
	void WalkingWay(vector<Point3d>& pointWay);
private:
	//int xp, yp, zp, xr, yr, zr;
	SOCKET sock;
	
	void getAngle(double& rx, double& ry, double& rz);
	double c2d(char* a);
};
