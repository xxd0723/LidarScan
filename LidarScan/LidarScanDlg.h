
// LidarScanDlg.h: 头文件
//

#pragma once
#include <opencv2/opencv.hpp>
#include "URControlClass.h"
#include "readLidar.h"
using namespace cv;
using namespace std;

// CLidarScanDlg 对话框
class CLidarScanDlg : public CDialogEx
{
// 构造
public:
	CLidarScanDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LIDARSCAN_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void DrawMatTo(Mat& img, UINT nID);
	DECLARE_MESSAGE_MAP()
	
public:
	afx_msg void OnBnClickedRun();
	afx_msg void LidarWalking(vector<Point3d> pointWay, vector<float>& vfDistanceMean);
	afx_msg void DrawMat3DHeight(vector<float> vfDistanceMean, Mat& mDst);
	afx_msg readLidar lidar;
};
