#include "StdAfx.h"
#include "SerialTestMFC.h"
#include "ComSelectDlg.h"
#include "Serial.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CComSelectDlg

BEGIN_MESSAGE_MAP(CComSelectDlg, CDialog)
	//{{AFX_MSG_MAP(CComSelectDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CComSelectDlg methods

CComSelectDlg::CComSelectDlg(CWnd* pParent /*=0*/)
	: CDialog(CComSelectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CComSelectDlg)
	m_nComIndex = 4;
	//}}AFX_DATA_INIT
}


void CComSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CComSelectDlg)
	DDX_Radio(pDX, IDC_COM1, m_nComIndex);
	//}}AFX_DATA_MAP
}

BOOL CComSelectDlg::OnInitDialog() 
{
	// Call base class
	CDialog::OnInitDialog();
#if 0
	// Enable only existing COM-ports
	GetDlgItem(IDC_COM1)->EnableWindow(CSerial::CheckPort(_T("COM1")) == CSerial::EPortAvailable);
	GetDlgItem(IDC_COM2)->EnableWindow(CSerial::CheckPort(_T("COM2")) == CSerial::EPortAvailable);
	GetDlgItem(IDC_COM3)->EnableWindow(CSerial::CheckPort(_T("COM3")) == CSerial::EPortAvailable);
	GetDlgItem(IDC_COM4)->EnableWindow(CSerial::CheckPort(_T("COM4")) == CSerial::EPortAvailable);
#endif
	return TRUE;
}

void CComSelectDlg::OnOK (void)
{
	// Make sure the data is valid
	UpdateData(TRUE);

	// Generate the COM port string
	m_strComPort.Format(_T("COM%d"), m_nComIndex+1);

	if( CSerial::CheckPort( m_strComPort ) != CSerial::EPortAvailable )
		AfxMessageBox(m_strComPort);

	// Call base class
	CDialog::OnOK();
}
