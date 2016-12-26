
// SerialTestMFCDialogDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SerialTestMFCDialog.h"
#include "SerialTestMFCDialogDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "Serial.h"

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CSerialTestMFCDialogDlg �Ի���




CSerialTestMFCDialogDlg::CSerialTestMFCDialogDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSerialTestMFCDialogDlg::IDD, pParent)
	, m_nIndexPort(0)
	, m_nIndexBaudRate(0)
	, m_nIndexByteType(0)
	, m_nIndexParityType(0)
	, m_nIndexStopBits(0)
	, m_strGPSPositionX(_T(""))
	, m_strGPSPositionY(_T(""))
	, m_strGPSPositionZ(_T(""))
	, m_strGPSAngle(_T(""))
	, m_strGroupBoxCom1ST(_T(""))
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
	DDX_Text(pDX, IDC_STATI_GPS_POSITIONX, m_strGPSPositionX);
	DDX_Text(pDX, IDC_STATI_GPS_POSITIONY, m_strGPSPositionY);
	DDX_Text(pDX, IDC_STATI_GPS_POSITIONZ, m_strGPSPositionZ);
	DDX_Text(pDX, IDC_STATIC_GPS_ANGLE, m_strGPSAngle);
	DDX_Text(pDX, IDC_GROUP_BOX_COM_1ST, m_strGroupBoxCom1ST);
}

BEGIN_MESSAGE_MAP(CSerialTestMFCDialogDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_COM_OPEN, &CSerialTestMFCDialogDlg::OnBnClickedButtonComOpen)
	ON_BN_CLICKED(IDC_BUTTON_COM_SET, &CSerialTestMFCDialogDlg::OnBnClickedButtonComSet)

	ON_WM_SERIAL(OnSerialMsg)

	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_COM_READ, &CSerialTestMFCDialogDlg::OnBnClickedButtonComRead)
END_MESSAGE_MAP()


// CSerialTestMFCDialogDlg ��Ϣ�������

BOOL CSerialTestMFCDialogDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	setDefault();
	UpdateData( FALSE );

	// ͼƬ�ؼ� ͸�� 
	LoadImg( image, CString("./res/compass-256up.png") );


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSerialTestMFCDialogDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		drawImage();

		CDialogEx::OnPaint();

	}

}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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

	m_bStartRead = false;

	m_nCountEvent = 0;

	SetTimer(0, 100, NULL);		// create a timer named '0'

	sdkCreateTimer( &timer );
	sdkStartTimer( &timer );

	// open console window to printf variable
	AllocConsole();
	freopen("CONOUT$", "w+t", stdout);
	freopen("CONIN$", "r+t", stdin);

}



void CSerialTestMFCDialogDlg::DisplayData( LPCTSTR pszData )
{
	if( !m_parser.isFrameValid() )
		return;

	vgKernel::StringVector* pStrVec;
	

	{
		pStrVec = m_parser.getStringVectorPosition( );
		if( pStrVec->size() >= 9 )
		{
			m_strGPSPositionX = (*pStrVec)[1].c_str() ;		m_strGPSPositionX += " ";
			m_strGPSPositionX += (*pStrVec)[2].c_str() ;
			m_strGPSPositionZ = (*pStrVec)[3].c_str() ;		m_strGPSPositionZ += " ";
			m_strGPSPositionZ += (*pStrVec)[4].c_str() ;
			m_strGPSPositionY = (*pStrVec)[8].c_str() ;
		}
	}


	{
		pStrVec = m_parser.getStringVectorAngle( );
		if( pStrVec->size() >= 8 )
		{
			m_strGPSAngle = (*pStrVec)[1].c_str() ;		m_strGPSAngle += ", ";
			m_strGPSAngle += (*pStrVec)[3].c_str() ;	m_strGPSAngle += ", ";
			m_strGPSAngle += (*pStrVec)[7].c_str() ;
		}
	}


	UpdateData( FALSE );
}

void CSerialTestMFCDialogDlg::OnBnClickedButtonComOpen()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);

	// format com port string, eg. COM3
	m_comboPort.GetLBText( m_nIndexPort, m_strPort );

	// ����Ƿ��Ѿ��򿪣� �����ظ�
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

	m_serial.SetEventChar( '\n' );	// register event EEventRcvEv

	m_strGroupBoxCom1ST = m_strPort;
	UpdateData( FALSE );

}


void CSerialTestMFCDialogDlg::OnBnClickedButtonComSet()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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

LRESULT CSerialTestMFCDialogDlg::OnSerialMsg( WPARAM wParam, LPARAM lParam )
{
	if ( !m_bStartRead )
		return 0;

	CSerial::EEvent eEvent = CSerial::EEvent(LOWORD(wParam));
	CSerial::EError eError = CSerial::EError(HIWORD(wParam));

	if (eEvent & CSerial::EEventRcvEv)
	{
		// Create a clean buffer
		DWORD dwRead;
		char szData[101]={'\0'};
		const int nBuflen = sizeof(szData)-1;

		// Obtain the data from the serial port
		do
		{
			m_serial.Read(szData,nBuflen,&dwRead);
			szData[dwRead] = '\0';
			m_strFrame += std::string(szData);

		} while (dwRead == nBuflen);

		printf( "m_strFrameALL(%d) = %s \n ", m_strFrame.length(), m_strFrame.c_str() );
		//DisplayData( (LPCTSTR)m_strFrame.c_str() );

		if( m_nCountEvent % 10 == 0 )
			DisplayData("\n\n\n");

		std::string strFrameCurrent;

		if ( m_parser.isFrameComplete(m_strFrame) )
		{
			int nRet = m_parser.parseValueFromString( m_strFrame );

			if( nRet != -1 )
			{
				strFrameCurrent = m_parser.formatCurrentFrameToString();

				printf( "frame %d: m_strFrameCurrent(%d) = %s \n ", m_nCountEvent, strFrameCurrent.length(), strFrameCurrent.c_str() );
				DisplayData( (LPCTSTR)strFrameCurrent.c_str() );
				
				Invalidate( FALSE );

				m_nCountEvent ++ ;
			}

		}
	}

	return 0;
}


void CSerialTestMFCDialogDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if ( 0 && !m_lineBuffer.empty() )
	{
		std::string strShow = m_lineBuffer.front();
		m_lineBuffer.pop_front();

		m_parser.parseValueFromString( strShow );
		static int nID = 0;
		printf("\n\nconsume string %d: %s \n", ++nID, strShow.c_str());

		DisplayData( (LPCTSTR)strShow.c_str() );
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CSerialTestMFCDialogDlg::OnBnClickedButtonComRead()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	m_bStartRead = true;

	m_serial.Purge();	// ��ն˿ڻ������Ѿ��ۻ�������
}

void CSerialTestMFCDialogDlg::LoadImg(CImage &image, CString &strPath)
{
	image.Load(strPath);

	if (image.IsNull()) {
		MessageBox(_T("ͼƬδ���سɹ�!"));
		return;
	}

	// �ж��Ƿ���Ҫ͸����ʾ��������Ӧ����
	if (image.GetBPP() == 32) {
		for (int i = 0; i < image.GetWidth(); ++i) {
			for (int j = 0; j < image.GetHeight(); ++j) {
				byte *pByte = (byte *)image.GetPixelAddress(i, j);
				pByte[0] = pByte[0] * pByte[3] / 255;
				pByte[1] = pByte[1] * pByte[3] / 255;
				pByte[2] = pByte[2] * pByte[3] / 255;
			}
		}
	}

}

void CSerialTestMFCDialogDlg::rotateControl( float m_iAngle )
{

	POINT centerPt;
	centerPt.x = (rect.right - rect.left)/2 ;
	centerPt.y = (rect.bottom - rect.top)/2 ;

	//
	// rotate the DC
	//
	nGraphicsMode = SetGraphicsMode(hDc, GM_ADVANCED);
	if ( m_iAngle != 0 )
	{
		double fangle = (double)m_iAngle / 180. * 3.1415926;
		xform.eM11 = (float)cos(fangle);
		xform.eM12 = (float)sin(fangle);
		xform.eM21 = (float)-sin(fangle);
		xform.eM22 = (float)cos(fangle);
		xform.eDx = (float)(centerPt.x - cos(fangle)*centerPt.x + sin(fangle)*centerPt.y);
		xform.eDy = (float)(centerPt.y - cos(fangle)*centerPt.y - sin(fangle)*centerPt.x);

		SetWorldTransform(hDc, &xform);
	}
}

void CSerialTestMFCDialogDlg::restoreControlRotation( float m_iAngle )
{
	// restore the DC
	if ( m_iAngle != 0 )
	{
		xform.eM11 = (float)1.0; 
		xform.eM12 = (float)0;
		xform.eM21 = (float)0;
		xform.eM22 = (float)1.0;
		xform.eDx = (float)0;
		xform.eDy = (float)0;

		SetWorldTransform(hDc, &xform);
	}
	SetGraphicsMode(hDc, nGraphicsMode);
}

void CSerialTestMFCDialogDlg::drawImage()
{
	// draw image 
	CWnd *pWnd = GetDlgItem(IDC_STATIC_COMPASS);
	pDC = pWnd->GetDC();
	hDc = pDC->m_hDC;	
	pWnd->GetClientRect(&rect);	//ȡ�ÿͻ����ߴ�
	pDC->SetStretchBltMode(STRETCH_HALFTONE);	//����ͼƬ��ʧ��

	float m_iAngle = m_parser.getAngle()[0];
	rotateControl(m_iAngle);

	image.Draw( hDc, rect );	//�ѿؼ��ߴ��С����ͼ
	ReleaseDC( pDC );

	restoreControlRotation(m_iAngle);
}
