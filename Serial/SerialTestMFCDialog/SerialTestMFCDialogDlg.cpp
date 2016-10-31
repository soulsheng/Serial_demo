
// SerialTestMFCDialogDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SerialTestMFCDialog.h"
#include "SerialTestMFCDialogDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "Serial.h"

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSerialTestMFCDialogDlg 对话框




CSerialTestMFCDialogDlg::CSerialTestMFCDialogDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSerialTestMFCDialogDlg::IDD, pParent)
	, m_nIndexPort(0)
	, m_nIndexBaudRate(0)
	, m_nIndexByteType(0)
	, m_nIndexParityType(0)
	, m_nIndexStopBits(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSerialTestMFCDialogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PORT, m_comboPort);
	DDX_CBIndex(pDX, IDC_COMBO_PORT, m_nIndexPort);
	DDX_CBIndex(pDX, IDC_COMBO_BAUDRATE, m_nIndexBaudRate);
	DDX_CBIndex(pDX, IDC_COMBO_BYTE, m_nIndexByteType);
	DDX_CBIndex(pDX, IDC_COMBO_PARITY, m_nIndexParityType);
	DDX_CBIndex(pDX, IDC_COMBO_STOP, m_nIndexStopBits);
}

BEGIN_MESSAGE_MAP(CSerialTestMFCDialogDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_COM_OPEN, &CSerialTestMFCDialogDlg::OnBnClickedButtonComOpen)
	ON_BN_CLICKED(IDC_BUTTON_COM_SET, &CSerialTestMFCDialogDlg::OnBnClickedButtonComSet)
END_MESSAGE_MAP()


// CSerialTestMFCDialogDlg 消息处理程序

BOOL CSerialTestMFCDialogDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	setDefault();
	UpdateData( FALSE );

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSerialTestMFCDialogDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSerialTestMFCDialogDlg::OnPaint()
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
HCURSOR CSerialTestMFCDialogDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSerialTestMFCDialogDlg::setDefault()
{
	m_nIndexBaudRate = 12;					// CSerial::EBaud115200;
	m_nIndexByteType = 3;					// CSerial::EData8;
	m_nIndexParityType = 0;					// CSerial::EParNone;
	m_nIndexStopBits = 0;					// CSerial::EStop1;
	m_nIndexPort = 2;						// COM3
}



void CSerialTestMFCDialogDlg::OnBnClickedButtonComOpen()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);

	// format com port string, eg. COM3
	m_comboPort.GetLBText( m_nIndexPort, m_strPort );

	// 检测是否已经打开， 避免重复
	if ( m_serial.IsOpen() )
	{
		CString str( m_strPort );
		str += " is Opened" ;
		AfxMessageBox( str,MB_ICONSTOP|MB_OK);
		return;
	}

	// Open the serial port
	if (m_serial.Open(m_strPort.GetBuffer(),this) != ERROR_SUCCESS)
	{
		AfxMessageBox(_T("Unable to open COM-port"),MB_ICONSTOP|MB_OK);
		GetParent()->PostMessage(WM_CLOSE);
		return ;
	}
	else
	{
		CString str( m_strPort );
		str += " is Opened" ;
		AfxMessageBox( str,MB_OK);
	}

	m_serial.SetEventChar( '\n' );

}


void CSerialTestMFCDialogDlg::OnBnClickedButtonComSet()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);

	// Determine baudrate
	CSerial::EBaudrate eBaudrate = CSerial::EBaudUnknown;
	switch (m_nIndexBaudRate)
	{
	case 0:  eBaudrate = CSerial::EBaud110;   break;
	case 1:  eBaudrate = CSerial::EBaud300;   break;
	case 2:  eBaudrate = CSerial::EBaud600;   break;
	case 3:  eBaudrate = CSerial::EBaud1200;   break;
	case 4:  eBaudrate = CSerial::EBaud2400;   break;
	case 5:  eBaudrate = CSerial::EBaud4800;   break;
	case 6:  eBaudrate = CSerial::EBaud9600;   break;
	case 7:  eBaudrate = CSerial::EBaud14400;  break;
	case 8:  eBaudrate = CSerial::EBaud19200;  break;
	case 9:  eBaudrate = CSerial::EBaud38400;  break;
	case 10:  eBaudrate = CSerial::EBaud56000;  break;
	case 11:  eBaudrate = CSerial::EBaud57600;  break;
	case 12:  eBaudrate = CSerial::EBaud115200; break;
	case 13:  eBaudrate = CSerial::EBaud128000; break;
	case 14:  eBaudrate = CSerial::EBaud256000; break;
	default: ASSERT(false); break;
	}

	CSerial::EDataBits eDataBits = CSerial::EDataUnknown;
	switch (m_nIndexByteType)
	{
	case 0:  eDataBits = CSerial::EData5; break;
	case 1:  eDataBits = CSerial::EData6; break;
	case 2:  eDataBits = CSerial::EData7; break;
	case 3:  eDataBits = CSerial::EData8; break;
	}

	CSerial::EParity eParity = CSerial::EParUnknown;
	switch (m_nIndexParityType)
	{
	case 0: eParity = CSerial::EParNone;  break;
	case 1: eParity = CSerial::EParOdd;   break;
	case 2: eParity = CSerial::EParEven;  break;
	case 3: eParity = CSerial::EParMark;  break;
	case 4: eParity = CSerial::EParSpace; break;
	default: ASSERT(false); break;
	}

	CSerial::EStopBits eStopBits = CSerial::EStopUnknown;
	switch (m_nIndexStopBits)
	{
	case 0: eStopBits = CSerial::EStop1;   break;
	case 1: eStopBits = CSerial::EStop1_5; break;
	case 2: eStopBits = CSerial::EStop2;   break;
	default: ASSERT(false); break;
	}


	// Setup the COM port
	if (m_serial.Setup(eBaudrate,eDataBits,eParity,eStopBits))
		AfxMessageBox(_T("Failed to Setup the COM port!"),MB_ICONSTOP|MB_OK);
	else
	{
		CString str( m_strPort );
		str += " setup ok" ;
		AfxMessageBox( str,MB_OK);
	}
}
