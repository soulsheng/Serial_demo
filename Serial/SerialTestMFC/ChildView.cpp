#include "StdAfx.h"
#include "SerialTestMFC.h"
#include "ChildView.h"
#include "ComSelectDlg.h"
#include "SheetSettings.h"

#define ENABLE_CMD_WINDOW	1	// AllocConsole

#ifdef _UNICODE
#include <malloc.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CChildView

BEGIN_MESSAGE_MAP(CChildView,CWnd)
	//{{AFX_MSG_MAP(CChildView)
	ON_COMMAND(ID_COM_SETTINGS, OnComSettings)
	ON_UPDATE_COMMAND_UI(ID_COM_SETTINGS, OnUpdateComSettings)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_COMMANDS_SENDFILE, OnCommandSendFile)
	ON_COMMAND(ID_COMMANDS_SENDBREAK, OnCommandSendBreak)
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
	ON_WM_SERIAL(OnSerialMsg)
	ON_WM_TIMER()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CChildView methods

CChildView::CChildView()
{
	m_nCountEvent = 0;
}

CChildView::~CChildView()
{
	// Close the serial port
	m_serial.Close();

#if ENABLE_CMD_WINDOW
	// close console window
	FreeConsole();
#endif

}

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	// Check if the window can be created
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	// Change the styles
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS,::LoadCursor(0, IDC_ARROW), HBRUSH(COLOR_WINDOW), 0);

	return TRUE;
}

void CChildView::OnComSettings() 
{
	// Display the COM settings dialog box
	CSheetSettings sheetSettings(&m_serial,this);
	sheetSettings.DoModal();
}

void CChildView::OnUpdateComSettings(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_serial.GetCommHandle() != INVALID_HANDLE_VALUE);
}

int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	// Call base class
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Create the rich edit control
	VERIFY(m_wndEdit.Create(WS_VISIBLE|WS_CHILD|WS_BORDER|ES_AUTOHSCROLL|ES_AUTOVSCROLL|ES_LEFT|ES_MULTILINE,CRect(0,0,0,0),this,0));

	// Obtain the requested COM port
	CComSelectDlg dlgComSelect(this);
	dlgComSelect.DoModal();

	// Open the serial port
	if (m_serial.Open(dlgComSelect.m_strComPort,this) != ERROR_SUCCESS)
	{
		AfxMessageBox(_T("Unable to open COM-port"),MB_ICONSTOP|MB_OK);
		GetParent()->PostMessage(WM_CLOSE);
		return 0;
	}

	m_serial.SetEventChar( '\n' );
	SetTimer(0, 100, NULL);

	// Set the serial port in the RichCommEdit control
	m_wndEdit.m_pSerial = &m_serial;

#if ENABLE_CMD_WINDOW
	// open console window to printf variable
	AllocConsole();
	freopen("CONOUT$", "w+t", stdout);
	freopen("CONIN$", "r+t", stdin);
#endif

	return 0;
}

void CChildView::OnSize(UINT nType, int cx, int cy) 
{
	// Call base class
	CWnd::OnSize(nType, cx, cy);
	
	// Size the controls as well
	m_wndEdit.SetWindowPos(0,0,0,cx,cy,SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOOWNERZORDER|SWP_NOZORDER);
}

LRESULT CChildView::OnSerialMsg (WPARAM wParam, LPARAM /*lParam*/)
{
	CSerial::EEvent eEvent = CSerial::EEvent(LOWORD(wParam));
	CSerial::EError eError = CSerial::EError(HIWORD(wParam));

	if (eEvent & CSerial::EEventRcvEv)
	{
		// Create a clean buffer
		DWORD dwRead;
		char szData[101];
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

				m_nCountEvent ++ ;
			}

		}
	}

	return 0;
}

void CChildView::DisplayEvent (LPCTSTR lpszMsg)
{
	// Format the selection as red text
	CHARFORMAT cf;
	cf.cbSize = sizeof(cf);
	cf.dwMask = CFM_COLOR;
	m_wndEdit.GetSelectionCharFormat(cf);
	cf.crTextColor = 0x000000FF;
	cf.dwEffects &= ~CFE_AUTOCOLOR;
	m_wndEdit.SetSelectionCharFormat(cf);

	// Display the text
	TCHAR tszMsg[200];
	wsprintf(tszMsg,_T("[%s]"), lpszMsg);
	m_wndEdit.SetSel(-1,-1);
	m_wndEdit.ReplaceSel(tszMsg);
}

void CChildView::DisplayEventSetting (LPCTSTR lpszMsg, LPCTSTR lpszSetting, bool fOn)
{
	TCHAR tszMsg[200];
	wsprintf(tszMsg,_T("%s (%s=%s)"), lpszMsg, lpszSetting, (fOn?_T("on"):_T("off")));
	DisplayEvent(tszMsg);
}

void CChildView::DisplayData (LPCTSTR pszData)
{
	// Format the selection as default text
	CHARFORMAT cf;
	cf.cbSize = sizeof(cf);
	cf.dwMask = CFM_COLOR;
	m_wndEdit.GetSelectionCharFormat(cf);
	cf.dwEffects |= CFE_AUTOCOLOR;
	m_wndEdit.SetSelectionCharFormat(cf);

	// Add the event to the edit control
	m_wndEdit.SetSel(-1,-1);
	m_wndEdit.ReplaceSel(pszData);
}

void CChildView::OnCommandSendFile() 
{
	// Select the file
	CFileDialog dlgFileOpen(TRUE,NULL,NULL,OFN_FILEMUSTEXIST|OFN_HIDEREADONLY|OFN_PATHMUSTEXIST,_T("All files (*.*)|*.*||"),this);
	if (dlgFileOpen.DoModal() != IDOK)
		return;

	// Open the file
	HANDLE hFile = ::CreateFile(dlgFileOpen.GetPathName(),GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_SEQUENTIAL_SCAN,NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		// Obtain file length
		DWORD dwDataLen = ::GetFileSize(hFile,0);

		// Map the file into memory
		HANDLE hMap = ::CreateFileMapping(hFile,NULL,PAGE_READONLY,0,0,NULL);
		if (hMap != NULL)
		{
			LPVOID lpData = ::MapViewOfFile(hMap,FILE_MAP_READ,0,0,0);
			if (lpData)
			{
				// Write all data
				DWORD dwWritten = 0;
				m_serial.Write(lpData,dwDataLen,&dwWritten);

				// Display event
				CString strEvent;
				strEvent.Format(_T("Written %d bytes."), dwWritten);
				DisplayEvent(strEvent);

				// Unmap the view
				::UnmapViewOfFile(lpData);
			}

			// Close mapping
			::CloseHandle(hMap);
		}

		// Close file
		::CloseHandle(hFile);
	}
}

void CChildView::OnCommandSendBreak() 
{
	// Send the break
	m_serial.Break();
}

void CChildView::OnSetFocus(CWnd* pOldWnd) 
{
	// Call base class
	CWnd::OnSetFocus(pOldWnd);
	
	// Pass focus to edit-window
	m_wndEdit.SetFocus();
}


void CChildView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if ( 0 && !m_lineBuffer.empty() )
	{
		std::string strShow = m_lineBuffer.front();
		m_lineBuffer.pop_front();

		DisplayData( (LPCTSTR)strShow.c_str() );
		m_parser.parseValueFromString( strShow );

		DisplayData( (LPCTSTR)strShow.c_str() );
		DisplayData("\n\n");
	}

	CWnd::OnTimer(nIDEvent);
}
