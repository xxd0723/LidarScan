
// LidarScanDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "LidarScan.h"
#include "LidarScanDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CLidarScanDlg 对话框



CLidarScanDlg::CLidarScanDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LIDARSCAN_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLidarScanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CLidarScanDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	
	ON_BN_CLICKED(IDC_RUN, &CLidarScanDlg::OnBnClickedRun)
END_MESSAGE_MAP()


// CLidarScanDlg 消息处理程序

BOOL CLidarScanDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CLidarScanDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CLidarScanDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CLidarScanDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//******************************** 图像画到MFC框内 ********************************
void CLidarScanDlg::DrawMatTo(cv::Mat& img, UINT nID)
{
	cv::Mat imgTmp;
	CRect rect;
	GetDlgItem(nID)->GetClientRect(&rect);  // 获取控件大小
	cv::resize(img, imgTmp, cv::Size(rect.Width(), rect.Height()));// 缩小或放大Mat并备份

	// 转一下格式 ,这段可以放外面,
	switch (imgTmp.channels())
	{
	case 1:
		cv::cvtColor(imgTmp, imgTmp, COLOR_GRAY2BGRA); // GRAY单通道
		break;
	case 3:
		cv::cvtColor(imgTmp, imgTmp, COLOR_BGR2BGRA);  // BGR三通道
		break;
	default:
		break;
	}

	int pixelBytes = imgTmp.channels() * (imgTmp.depth() + 1); // 计算一个像素多少个字节
	// 制作bitmapinfo(数据头)
	BITMAPINFO bitInfo;
	bitInfo.bmiHeader.biBitCount = 8 * pixelBytes;
	bitInfo.bmiHeader.biWidth = imgTmp.cols;
	bitInfo.bmiHeader.biHeight = -imgTmp.rows;
	bitInfo.bmiHeader.biPlanes = 1;
	bitInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitInfo.bmiHeader.biCompression = BI_RGB;
	bitInfo.bmiHeader.biClrImportant = 0;
	bitInfo.bmiHeader.biClrUsed = 0;
	bitInfo.bmiHeader.biSizeImage = 0;
	bitInfo.bmiHeader.biXPelsPerMeter = 0;
	bitInfo.bmiHeader.biYPelsPerMeter = 0;
	// Mat.data + bitmap数据头 -> MFC
	CDC* pDC = GetDlgItem(nID)->GetDC();
	::StretchDIBits(
		pDC->GetSafeHdc(),
		0, 0, rect.Width(), rect.Height(),
		0, 0, rect.Width(), rect.Height(),
		imgTmp.data,
		&bitInfo,
		DIB_RGB_COLORS,
		SRCCOPY
	);
	ReleaseDC(pDC);
}

void CLidarScanDlg::OnBnClickedRun()
{
	// TODO: 在此添加控件通知处理程序代码
	URControlClass URControl;
	//URControl.moveTo(-0.4, 0, -0.4, 1.78, -2.6, 0.029);
	//URControl.shiftTo(-0.4, 0, -0.4);

	vector<Point3d> pointWay;

	pointWay.push_back(Point3d(-0.4, -0.3, -0.3));
	pointWay.push_back(Point3d(-0.4, -0.1, -0.3));
	pointWay.push_back(Point3d(-0.4, 0.1, -0.3));
	pointWay.push_back(Point3d(-0.4, 0.3, -0.3));
	pointWay.push_back(Point3d(-0.4, 0.3, -0.4));
	pointWay.push_back(Point3d(-0.4, 0.1, -0.4));
	pointWay.push_back(Point3d(-0.4, -0.1, -0.4));
	pointWay.push_back(Point3d(-0.4, -0.3, -0.4));
	pointWay.push_back(Point3d(-0.4, -0.3, -0.5));
	pointWay.push_back(Point3d(-0.4, -0.1, -0.5));
	pointWay.push_back(Point3d(-0.4, 0.1, -0.5));
	pointWay.push_back(Point3d(-0.4, 0.3, -0.5));
	//URControl.WalkingWay(pointWay);

	vector<float> vfDistanceMean;
	vfDistanceMean.clear();
	LidarWalking(pointWay, vfDistanceMean);

	
	//Mat mSrc = imread("lena.jpg");
	Mat mSrc;
	DrawMat3DHeight(vfDistanceMean, mSrc);
	DrawMatTo(mSrc, IDC_PIC1);
}
//******************************** 走一点Lidar测一点 ********************************
void CLidarScanDlg::LidarWalking(vector<Point3d> pointWay, vector<float>& vfDistanceMean)
{
	URControlClass URControl;
	for (int i = 0; i < pointWay.size(); i++)
	{
		Point3d pointTemp;
		pointTemp = pointWay[i];
		URControl.shiftTo(pointTemp.x, pointTemp.y, pointTemp.z);	// 平移

		while (1)
		{
			double px, py, pz;
			URControl.getPosition(px, py, pz);
			if (abs(pointTemp.x - px) < 0.001 &&
				abs(pointTemp.y - py) < 0.001 &&
				abs(pointTemp.z - pz) < 0.001)
			{
				lidar.getData();
				vfDistanceMean.push_back(lidar.fDistanceMean);	// 每一点的距离输出
				/*CString cstr;
				cstr.Format(_T("%f"), lidar.fDistanceMean);
				MessageBox(NULL, cstr, NULL);	//调试长长长长长长长长长长长长长长长长长长长长*/
				break;
			}
		}
	}
}
void CLidarScanDlg::DrawMat3DHeight(vector<float> vfDistanceMean, Mat& mDst)
{
	mDst = Mat::zeros(3, 4, CV_8UC1);

	auto maxPosition = max_element(vfDistanceMean.begin(), vfDistanceMean.end());
	auto minPosition = min_element(vfDistanceMean.begin(), vfDistanceMean.end());

	for (int i = 0; i < mDst.rows; i++)	//行循环
	{
		uchar* dataR = mDst.ptr<uchar>(i);			// 获取mLabel的i行首地址
		for (int j = 0; j < mDst.cols; j++)			// 列循环
		{
			if (i%2)				// 奇数行
			{
				dataR[j] = (*maxPosition - vfDistanceMean[(i + 1) * 4 - j - 1] ) / (*maxPosition - *minPosition) * 255;
			}
			else					// 偶数行
			
			{
				dataR[j] = (*maxPosition - vfDistanceMean[i * 4 + j]) / (*maxPosition - *minPosition) * 255;
			}
		}
	}
}