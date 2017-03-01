#if !defined(AFX_RULERWND_H__9D8CA79F_99BA_4075_A9CD_5FA8B55E26B8__INCLUDED_)
#define AFX_RULERWND_H__9D8CA79F_99BA_4075_A9CD_5FA8B55E26B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RulerWnd.h : header file
//

/*****************************************

  Uður HAMZADAYI 2002   
			 
  Ruler Control	    

***************************************/

/////////////////////////////////////////////////////////////////////////////
// CRulerWnd window					 

#define RULERWINDOW_CLASSNAME  _T( "CRulerWnd" )


//Ruler Window Style
#define RWSTYLE_VERT          0x0001
#define RWSTYLE_HORZ          0x0002
#define RWSTYLE_LEFTALIGN     0x0004
#define RWSTYLE_RIGHTALIGN    0x0008
#define RWSTYLE_TOPALIGN      RWSTYLE_LEFTALIGN
#define RWSTYLE_BOTTOMALIGN   RWSTYLE_RIGHTALIGN
#define RWSTYLE_DRAWMODAL     0x0010
#define RWSTYLE_DRAWEDGE      0x0020   

				   
//Notify Message
#define  NM_RULER_NOTIFICATIONMESSAGE   0x1112
#define  NMSUB_RULER_SEPERATORCHANGE    0x0001
#define  NMSUB_RULER_SEPERATORCHANGING  0x0002


//Seperator Types
#define SEPTYPE_NOPOSCHANGE    0x0001


class CRulerWnd : public CWnd
{
// Construction				 
public:
	CRulerWnd();

// Attributes
public:

	typedef struct _tagRULERWNDNOTIFY_INFO{

	   NMHDR   hdr;

	   UINT    nSubMessage;

	   DWORD   dwRulerStyle;

	   int     iSepID;
	   int     iNewPos;
	   int     iOldPos;

	   int     iParam1;
	   int     iParam2;

	}RULERWNDNOTIFY_INFO;


	typedef struct _tagSEPERATOR_TYPE{

	   int      iPos;
	   int      iType;
	   int      iID;
	   COLORREF	clrLine;
	   COLORREF	clrFill;

	   int      iMinMargin;
	   int      iMaxMargin;

	   LPARAM lParam;  

	}SEPERATOR_TYPE;


// Operations
public:

	SEPERATOR_TYPE* GetSeperator( int iID );
	int DeleteAllSeperator();
	int DeleteSeperator( int iID );
	int AddSeperator( int iPos , int iID , int iType = 0 , LPARAM lParam = NULL , COLORREF	clrLine = RGB( 0 , 0 , 0 ) , COLORREF clrFill = RGB( 255 ,255 , 220 ) , int iMinMargin = 0 , int iMaxMargin = 0xFFFFFFF );
	

	//Get Property
	DWORD    GetStyle() { return m_dwStyle; }
	COLORREF GetBackGroundColor() { return m_clrBackGround; }
	COLORREF GetMilimeterLineColor() { return m_clrMilimeterLineColor; }
	COLORREF GetTextColor() { return m_clrTextColor; }
	UINT     GetStartSeperateSize() { return m_nSeperateSize; }
	UINT     GetMargin() { return m_nRulerMargin; }
	UINT     GetMilimeterPixel() { return m_nMilimeterPixel; }
	UINT     GetSeperatorSize() { return m_nSeperatorSize; }
	long     GetScrollPos() { return m_lScrolPos; }
	CWnd*	 GetMessageTarget() { return m_pMessageTarget; }


	//Set Property
	BOOL     SetStyle( DWORD dwStyle );
	BOOL     SetBackGroundColor( COLORREF clr );
	BOOL     SetMilimeterLineColor( COLORREF clr );
	BOOL     SetTextColor( COLORREF clr );
	BOOL     SetStartSeperateSize( UINT nSize );
	BOOL     SetMargin( UINT nMargin );
	BOOL     SetMilimeterPixel( UINT nPixel );
	BOOL     SetSeperatorSize( UINT nSize );
	BOOL     SetScrollPos( long lPos );
	BOOL     SetMessageTarget( CWnd *pTarget = NULL );


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRulerWnd)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	LRESULT SendParentNotifyMessage( UINT nSubMessage , int iSepID , int iNewPos , int iOldPos , int iParam1 = 0 , int iParam2 = 0 );
	int GetClientPixelFromPos( int iPos );
	int GetMostPos( CPoint point );
	int SeperatorHitTest( CPoint point );
	BOOL DraggingSeperator( int iDragID , CPoint point );
	BOOL Create( DWORD dwStyle , const RECT& rect ,  CWnd* pParentWnd = NULL ,  UINT nID = 0  );
	virtual ~CRulerWnd();

	// Generated message map functions
protected:
	void SetCursorClipRect();
	BOOL DrawHSeperator( CDC *pDC , SEPERATOR_TYPE *pSep );
	BOOL DrawVSeperator( CDC *pDC , SEPERATOR_TYPE *pSep );
	BOOL DrawVRuler( CDC *pDC );
	BOOL DrawHRuler( CDC *pDC );


	DWORD     m_dwStyle;
	COLORREF  m_clrBackGround;
	COLORREF  m_clrMilimeterLineColor;
	COLORREF  m_clrTextColor;
	UINT      m_nSeperateSize;
	UINT      m_nRulerMargin;
	UINT      m_nMilimeterPixel;
	UINT      m_nSeperatorSize;

	long      m_lScrolPos;

	CPtrArray m_Seperators;

	BOOL      m_bLButtonDown;
	BOOL      m_bDraging;
	int       m_iDragID;
	int       m_iDragPos;
    int       m_iOldDragingPos;

	CWnd      *m_pMessageTarget;

	RULERWNDNOTIFY_INFO m_NotifyInfo;

	BOOL RegisterWindowClass();
	//{{AFX_MSG(CRulerWnd)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RULERWND_H__9D8CA79F_99BA_4075_A9CD_5FA8B55E26B8__INCLUDED_)
