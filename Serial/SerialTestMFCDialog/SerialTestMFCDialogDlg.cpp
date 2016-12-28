
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

#define PI 3.1415926


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
		float* m_fAngle = m_parser.getAngle();

		drawImage( m_fAngle[0] );

		CPaintDC dc(this); // ���ڻ��Ƶ��豸������
		DrawCompassBackground( dc );
		DrawAngleUpdate( dc, m_fAngle[0], m_fAngle[1], m_fAngle[2] );

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

	// compass graph
	rect[0] = CRect( CPoint(350,  45), CSize(260, 260) ); // (630, 100, 930, 400);  
	rect[1] = CRect( CPoint(350, 340), CSize(270,  10) ); // (590, 470, 950, 500)
	rect[2] = CRect( CPoint(300,  80), CSize( 10, 270) ); // (520,  90, 550, 450)

	x5 = rect[0].left;
	y5 = rect[0].top;
	x6 = rect[0].right;
	y6 = rect[0].bottom;

	x1 = rect[1].left;
	y1 = rect[1].top;
	x2 = rect[1].right;
	y2 = rect[1].bottom;

	x3 = rect[2].left;
	y3 = rect[2].top;
	x4 = rect[2].right;
	y4 = rect[2].bottom;

	//Բ�İ뾶
	radius = (x6-x5)/2;

	//��Բ������
	x0 = x5 + (x6-x5)/2;
	y0 = y5 + (y6-y5)/2;

	//10��Ϊһ����5��Ϊ1���޸Ĵ�ֵΪ10    -90��90,18�� 
	lineCount = 18;
	space = (y4 - y3) / lineCount;

	xYawSrc = 0;
	yYawSrc = 0;

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
	centerPt.x = (rectCompass.right - rectCompass.left)/2 ;
	centerPt.y = (rectCompass.bottom - rectCompass.top)/2 ;

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

void CSerialTestMFCDialogDlg::drawImage( float m_iAngle )
{
	// draw image 
	CWnd *pWnd = GetDlgItem(IDC_STATIC_COMPASS);
	pDC = pWnd->GetDC();
	hDc = pDC->m_hDC;	
	pWnd->GetClientRect(&rectCompass);	//ȡ�ÿͻ����ߴ�
	pDC->SetStretchBltMode(STRETCH_HALFTONE);	//����ͼƬ��ʧ��


	rotateControl(m_iAngle);

	image.Draw( hDc, rectCompass );	//�ѿؼ��ߴ��С����ͼ
	ReleaseDC( pDC );

	restoreControlRotation(m_iAngle);
}

void CSerialTestMFCDialogDlg::DrawAngleYaw( CPaintDC &dc, CRect rt )
{

	//��Բ
	//dc.Ellipse(x5,y5,x6,y6);//(x1,y1),(x2,y2)Ϊ��Χԭ�͵ľ��ε����ϽǺ����½ǵ�����

	//Բ�����ʶ
	dc.TextOut(x5 + (x6-x5) / 2 - 5, y5 + 5, _T("N"));
	dc.TextOut(x6 - 20, y5 + (y6-y5) / 2 - 10, _T("E"));
	dc.TextOut(x5 + 10, y5 + (y6-y5) / 2 - 10, _T("W"));
	dc.TextOut(x5 + (x6-x5) / 2 - 5, y6 - 20, _T("S"));

	//Բ�Ƕ��ı���ʶ�� 45��ı�������8��
	dc.TextOut(x5 + (x6-x5) / 2 - 5, y5 - 20, _T("0"));
	dc.TextOut(x6 + 5, y5 + (y6-y5) / 2 - 10, _T("90"));
	dc.TextOut(x5 - 20, y5 + (y6-y5) / 2 -10, _T("270"));
	dc.TextOut(x5 + (x6-x5) / 2 - 10, y6 + 5, _T("180"));


	dc.TextOut(x0 + radius * cos(45*PI/180.0)+5, y0 - radius * sin(45*PI/180.0)-15, _T("45"));
	dc.TextOut(x0 + radius * cos(45*PI/180.0), y0 + radius * sin(45*PI/180.0), _T("135"));
	dc.TextOut(x0 - radius * cos(45*PI/180.0)-20, y0 + radius * sin(45*PI/180.0), _T("225"));
	dc.TextOut(x0 - radius * cos(45*PI/180.0)-20, y0 - radius * sin(45*PI/180.0)-15, _T("315"));


	//���廭ˢ��ɫ����ɫСԲ����8�� 
	CBrush brushGreen(RGB(0, 255, 0));
	CBrush* pOldBrush = dc.SelectObject(&brushGreen);

	dc.Ellipse(x5 + (x6-x5) / 2 - 3, y5 - 3, x5 + (x6-x5) / 2 + 3, y5 + 3);
	dc.Ellipse(x6 - 3, y5 + (y6-y5) / 2 - 3, x6 + 3, y5 + (y6-y5) / 2 + 3);
	dc.Ellipse(x5 - 3, y5 + (y6-y5) / 2 - 3, x5 + 3, y5 + (y6-y5) / 2 + 3);
	dc.Ellipse(x5 + (x6-x5) / 2 - 3, y6 - 3, x5 + (x6-x5) / 2 + 3, y6 + 3);

	dc.Ellipse((int)(x0 + radius * cos(45*PI/180.0))-3, (int)(y0 - radius * sin(45*PI/180.0))-3, (int)(x0 + radius * cos(45*PI/180.0))+3, (int)(y0 - radius * sin(45*PI/180.0))+3);
	dc.Ellipse((int)(x0 + radius * cos(45*PI/180.0))-3, (int)(y0 + radius * sin(45*PI/180.0))-3, (int)(x0 + radius * cos(45*PI/180.0))+3, (int)(y0 + radius * sin(45*PI/180.0))+3);
	dc.Ellipse((int)(x0 - radius * cos(45*PI/180.0))-3, (int)(y0 + radius * sin(45*PI/180.0))-3, (int)(x0 - radius * cos(45*PI/180.0))+3, (int)(y0 + radius * sin(45*PI/180.0))+3);
	dc.Ellipse((int)(x0 - radius * cos(45*PI/180.0))-3, (int)(y0 - radius * sin(45*PI/180.0))-3, (int)(x0 - radius * cos(45*PI/180.0))+3, (int)(y0 - radius * sin(45*PI/180.0)) + 3);

	dc.SelectObject(&pOldBrush);

	//���廭ˢ��ɫ����ɫ����СԲ�����Ǽ�ͷ
	CBrush brushRed(RGB(255, 0, 0));
	pOldBrush = dc.SelectObject(&brushRed);

	//Բ�����ĵ�СԲ
	int x7 = x5+(x6-x5)/2-5;
	int y7 = y5+(y6-y5)/2-5;
	int x8 = x5+(x6-x5)/2+5;
	int y8 = y5+(y6-y5)/2+5;

	dc.Ellipse(x7,y7,x8,y8);//(x1,y1),(x2,y2)Ϊ��ΧԲ�͵ľ��ε����ϽǺ����½ǵ�����

	dc.SelectObject(&pOldBrush);

	CPen hPenGreen;
	hPenGreen.CreatePen(PS_SOLID,1,RGB(0,255,0));//���ж���CPEN����ɫ���޸�RGB��ֵ����ɫʮ����
	CPen *pOldPen = dc.SelectObject(&hPenGreen);
	//Բ���м��������
	dc.MoveTo(x5+40, y5+(y6-y5)/2);
	dc.LineTo(x6-40, y5+(y6-y5)/2);

	dc.MoveTo(x5+(x6-x5)/2, y5+40);
	dc.LineTo(x5+(x6-x5)/2, y6-40);


	dc.SelectObject(pOldPen);

}
void CSerialTestMFCDialogDlg::DrawAnglePitch( CPaintDC &dc, CRect rt )
{
	//��������ο�
	dc.Rectangle(&rt);

	//����ο���
	for(int i=1;i<lineCount;i++)
	{
		dc.MoveTo(x1+space*i, y1);
		dc.LineTo(x1+space*i, y2);
	}
	//��ɺ���ο���

	//�����ֵ��ʶ
	int nLeft = x1 - 10;
	int nTop = y2 + 5;
	for( int i = 0; i <= lineCount; i +=3 )
	{
		CString strAngle ;
		strAngle.Format( _T("%3d"), -90+i*10 );

		dc.TextOut(nLeft + i*space, nTop, strAngle );
	}

}
void CSerialTestMFCDialogDlg::DrawAngleRoll( CPaintDC &dc, CRect rt )
{
	//���������ο�
	dc.Rectangle(&rt);

	//����ο���
	//int space = 20;//10��Ϊһ����5��Ϊ1���޸Ĵ�ֵΪ10
	//int lineCount = (y4 - y3) / space;

	//�����ο���
	for(int i=1;i<lineCount;i++)
	{
		dc.MoveTo(x3, y3+space*i);
		dc.LineTo(x4, y3+space*i);
	}
	//��������ο���

	//������ֵ��ʶ
	int nLeft = x3 - 25;
	int nTop = y3 - 5;
	for( int i = 0; i <= lineCount; i +=3 )
	{
		CString strAngle ;
		strAngle.Format( _T("%3d"), 90-i*10 );

		dc.TextOut(nLeft, nTop + i*space, strAngle );
	}

}
void CSerialTestMFCDialogDlg::DrawAngleUpdate( CPaintDC &dc, float yaw, float pitch, float roll )
{
	float tilt = pitch;
	float tiltSrc1 = roll;
	float yawSrc1 = yaw;

	//���廭ˢ��ɫ����ɫ���Ǽ�ͷ
	CBrush brushRed(RGB(255, 0, 0));
	CBrush* pOldBrush = dc.SelectObject(&brushRed);

	//���ָʾ��ͷ��pitch 
	CPoint pts[3];
	pts[0].x = x1 + space * (lineCount / 2) - 10 + tilt * (space / 10.0f);//10��һ�ָ�
	pts[0].y = y1 - 15;

	pts[1].x = x1 + space * (lineCount / 2) + 10 + tilt * (space / 10.0f);
	pts[1].y = y1 - 15;

	pts[2].x = x1 + space * (lineCount / 2) + tilt * (space / 10.0f);
	pts[2].y = y1;

	dc.Polygon(pts, 3);

	//����ָʾ��ͷ��roll 
	CPoint pts2[3];

	pts2[0].x = x4;//10��һ�ָ�
	pts2[0].y = y3 + space * (lineCount / 2) - tiltSrc1 * (space / 10.0f);

	pts2[1].x = x4 + 15;
	pts2[1].y = y3 + space * (lineCount / 2) - 10 - tiltSrc1 * (space / 10.0f);

	pts2[2].x = x4 + 15;
	pts2[2].y = y3 + space * (lineCount / 2) + 10 - tiltSrc1 * (space / 10.0f);

	dc.Polygon(pts2, 3);


	//Բָʾ��ͷ��yaw 
	CPoint pts3[3];

	if(yawSrc1 == 0.0f)
	{
		xYawSrc = x5+(x6-x5)/2;
		yYawSrc = y5;
	} 
	else 
	{
		float yawSrcRadian = 0.0f;
		//if(0.0<yawSrc1<90.0)
		if((yawSrc1>0.0) &&(yawSrc1-90.0)<0.0)
		{
			//����
			yawSrcRadian = yawSrc1 * PI / 180.0f;
			//��֪Բ�����꣬�뾶�ͽǶȣ���Բ��������������㹫ʽ
			xYawSrc = (int)(x0 + radius * sin(yawSrcRadian));
			yYawSrc = (int)(y0 - radius * cos(yawSrcRadian));
		}

		else if((yawSrc1-90.0)>0.0 && (yawSrc1-180.0)<0.0)
		{
			//����
			yawSrcRadian = (yawSrc1-90) * PI / 180.0f;
			//��֪Բ�����꣬�뾶�ͽǶȣ���Բ��������������㹫ʽ
			xYawSrc = (int)(x0 + radius * cos(yawSrcRadian));
			yYawSrc = (int)(y0 + radius * sin(yawSrcRadian));
		}

		else if((yawSrc1-180.0)>0.0 && (yawSrc1-270.0)<0.0)
		{
			//����
			yawSrcRadian = (yawSrc1-180) * PI / 180.0f;
			//��֪Բ�����꣬�뾶�ͽǶȣ���Բ��������������㹫ʽ
			xYawSrc = (int)(x0 - radius * cos(yawSrcRadian));
			yYawSrc = (int)(y0 + radius * sin(yawSrcRadian));
		}

		else if((yawSrc1-270.0)>0.0 && (yawSrc1-360.0)<0.0)
		{
			//����
			yawSrcRadian = (yawSrc1-270) * PI / 180.0f;
			//��֪Բ�����꣬�뾶�ͽǶȣ���Բ��������������㹫ʽ
			xYawSrc = (int)(x0 - radius * cos(yawSrcRadian));
			yYawSrc = (int)(y0 - radius * sin(yawSrcRadian));
		}

	}

	pts3[0].x = xYawSrc - 10;
	pts3[0].y = yYawSrc -15;

	pts3[1].x = xYawSrc + 10;
	pts3[1].y = yYawSrc -15;

	pts3[2].x = xYawSrc;
	pts3[2].y = yYawSrc;

	dc.Polygon(pts3, 3);

	//����ʵ�ʽǶȻ���
	CPen hPenGreen;
	hPenGreen.CreatePen(PS_SOLID,1,RGB(0,255,0));//���ж���CPEN����ɫ���޸�RGB��ֵ����ɫʮ����
	CPen *pOldPen = dc.SelectObject(&hPenGreen);
	dc.MoveTo(x0, y0);
	dc.LineTo(xYawSrc, yYawSrc);
	dc.SelectObject(pOldPen);

}

void CSerialTestMFCDialogDlg::DrawCompassBackground( CPaintDC &dc )
{


	CPen hPenBlack;
	hPenBlack.CreatePen(PS_SOLID,1,RGB(0,0,0));//���ж���CPEN����ɫ���޸�RGB��ֵ,����������ɫ

	CPen* oldPen = dc.SelectObject(&hPenBlack);
	//dc.SetViewportOrg(200, 130);//�ƶ�ԭ��
	dc.SetTextColor(RGB(0,0,255));//����������ɫΪ��ɫ

	CFont font;
	font.CreatePointFont(80, _T("����"), NULL);
	CFont *pOldFont=dc.SelectObject(&font);

	// ������/������ɫ��͸����ɫ
	CBrush* pOldBrush = (CBrush*)dc.SelectStockObject(NULL_BRUSH);  

	DrawAngleYaw( dc, rect[0] );

	dc.SelectObject(pOldBrush);

	pOldBrush = (CBrush*)dc.SelectStockObject(NULL_BRUSH); 

	DrawAnglePitch( dc, rect[1] );

	DrawAngleRoll( dc, rect[2] );

	dc.SelectObject(oldPen);
	dc.SelectObject(pOldFont);

}
