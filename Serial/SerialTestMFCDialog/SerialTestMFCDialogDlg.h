
// SerialTestMFCDialogDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"

#include "SerialMFC.h"
#include "StringParserDIY.h"
#include "helper_timer.h"

#include <deque>
#include <string>

#include <gdiplus.h>
#include "afxcmn.h"
#include "CAdobeSliderCtrl.h"

// CSerialTestMFCDialogDlg �Ի���
class CSerialTestMFCDialogDlg : public CDialogEx
{
// ����
public:
	CSerialTestMFCDialogDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CSerialTestMFCDialogDlg();

// �Ի�������
	enum { IDD = IDD_SERIALTESTMFCDIALOG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	void	setDefault();
	void	DisplayData (LPCTSTR pszData);
	void	LoadImg(CImage &image, CString &strPath);

	void	rotateControl( float m_iAngle );
	void	restoreControlRotation( float m_iAngle );
	void	drawImage( float m_iAngle );

	void DrawCompassBackground( CPaintDC &dc );

	void DrawAngleYaw( CPaintDC &dc, CRect rt );
	void DrawAnglePitch( CPaintDC &dc, CRect rt );
	void DrawAngleRoll( CPaintDC &dc, CRect rt );
	void DrawAngleUpdate( CPaintDC &dc, float yaw, float pitch, float roll );

public:
	CComboBox m_comboPort;
	// �˿�������0��ӦCOM1
	int m_nIndexPort;
	// ��������������
	int m_nIndexBaudRate;
	// ����λ��������
	int m_nIndexByteType;
	// У����������
	int m_nIndexParityType;
	// ֹͣλ��������
	int m_nIndexStopBits;

	CSerialMFC		m_serial;
	CString			m_strPort;		// com port string, eg. COM3
	std::deque<std::string>		m_lineBuffer;
	StringParserDIY	m_parser;
	bool			m_bStartRead;	// start to read or not 

	CString m_strGPSPositionX;
	CString m_strGPSPositionY;
	CString m_strGPSPositionZ;

	afx_msg void OnBnClickedButtonComOpen();
	afx_msg void OnBnClickedButtonComSet();

	afx_msg LRESULT OnSerialMsg (WPARAM wParam, LPARAM lParam);

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonComRead();

	CString m_strGPSAngle;
	CString m_strGroupBoxCom1ST;

	StopWatchInterface* timer;		// test time interval

	std::string		m_strFrame;
	int				m_nCountEvent;

	CImage			image;
	CDC *pDC;
	CRect rectCompass;
	HDC hDc;
	int nGraphicsMode ;
	XFORM xform;

	// compass background graph
	CRect		rect[3]; // yaw, pitch, roll 
	int			x1,y1,x2,y2,x3,y3,x4,y4,x5,y5,x6,y6;
	int			space, lineCount;
	float		x0, y0, radius;	// center(x0, y0) radius of circle for yaw 
	int			xYawSrc, yYawSrc;

	ULONG_PTR           m_gdiplusToken;
	Gdiplus::Image		*gdi_image;
	// ˮƽ�̶�
	//CAdobeSliderCtrl m_sliderCtrlHor;
};
