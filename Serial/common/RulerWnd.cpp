// RulerWnd.cpp : implementation file
//

#include "stdafx.h"
#include "RulerWnd.h"
#include <math.h>
#include "MemDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRulerWnd
CRulerWnd::CRulerWnd()
{

   m_bLButtonDown   = m_bDraging = FALSE;
   m_iDragID	    = -1;
   m_iDragPos		= -1;
   m_iOldDragingPos = -1;

   m_dwStyle         = RWSTYLE_VERT | RWSTYLE_LEFTALIGN;
   m_nSeperateSize   = 10;
   m_nSeperatorSize	 = 5;
   m_nRulerMargin    = 500;
   m_lScrolPos	     = 0;
   m_nMilimeterPixel = 4;

   m_clrBackGround         = GetSysColor( COLOR_3DFACE );
   m_clrMilimeterLineColor = RGB( 0 , 0 , 0 );
   m_clrTextColor          = RGB( 0 , 0 , 0 );

   m_pMessageTarget        = NULL;

   RegisterWindowClass();

   DeleteAllSeperator();

}



CRulerWnd::~CRulerWnd()
{

   DeleteAllSeperator();

   DestroyWindow();

   WNDCLASS wndcls = { 0 };

   if( ( ::GetClassInfo( AfxGetResourceHandle() , RULERWINDOW_CLASSNAME , &wndcls ) ) ) 
   {

      ::UnregisterClass( wndcls.lpszClassName , AfxGetResourceHandle() );

   }

}




BEGIN_MESSAGE_MAP(CRulerWnd, CWnd)
	//{{AFX_MSG_MAP(CRulerWnd)
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CRulerWnd message handlers

BOOL CRulerWnd::RegisterWindowClass()
{

    WNDCLASS wndcls;

    HINSTANCE hInst = AfxGetResourceHandle();

    if ( !( ::GetClassInfo( hInst, RULERWINDOW_CLASSNAME , &wndcls ) ) )
    {

       // otherwise we need to register a new class
       wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
       wndcls.lpfnWndProc      = ::DefWindowProc;
       wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
       wndcls.hInstance        = hInst;
       wndcls.hIcon            = NULL;
#ifndef _WIN32_WCE_NO_CURSOR
       wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
#else
       wndcls.hCursor          = 0;
#endif										
       wndcls.hbrBackground    = (HBRUSH)( COLOR_3DFACE + 1 );
       wndcls.lpszMenuName     = NULL;
       wndcls.lpszClassName    = RULERWINDOW_CLASSNAME;
        
       if( !AfxRegisterClass( &wndcls ) )
       {

          AfxThrowResourceException();
          return FALSE;

       }

    }

    return TRUE;

}



BOOL CRulerWnd::Create( DWORD dwStyle, const RECT &rect, CWnd *pParentWnd , UINT nID )
{

   ASSERT(pParentWnd->GetSafeHwnd());

   if (!CWnd::Create( RULERWINDOW_CLASSNAME , NULL, dwStyle, rect, pParentWnd, nID ) )
        return FALSE;

   return TRUE;

}

BOOL CRulerWnd::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CMyMemDC memDC( pDC );

	CRect rect;
	GetClientRect( &rect );

	memDC.FillSolidRect( &rect , m_clrBackGround );

    if( m_dwStyle & RWSTYLE_DRAWMODAL )
	   memDC.Draw3dRect( &rect , RGB( 230 , 230 , 230 ) , RGB( 100 ,100 ,100 ) );
	else
    if( m_dwStyle & RWSTYLE_DRAWEDGE )
	   memDC.Draw3dRect( &rect , RGB( 100 ,100 ,100 ) , RGB( 230 , 230 , 230 ) );

	//Drawing Ruler
	if( m_dwStyle & RWSTYLE_VERT )
	   DrawVRuler( &memDC );
	else
	if( m_dwStyle & RWSTYLE_HORZ )
	   DrawHRuler( &memDC );

	return TRUE;//CWnd::OnEraseBkgnd(pDC);
}

void CRulerWnd::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	CWnd::PreSubclassWindow();

    ModifyStyleEx( 0, WS_EX_CLIENTEDGE | /*WS_EX_DLGMODALFRAME |*/ SWP_DRAWFRAME  );

	SetWindowPos( NULL ,  0 ,0 , 0 , 0 , SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER );

}


BOOL CRulerWnd::DrawHRuler(CDC *pDC)
{

  //Ölçüler çiziliyor....
  CRect rectRuler;
  GetClientRect( &rectRuler );


  CPen pen;
  pen.CreatePen( PS_SOLID , 1 , m_clrMilimeterLineColor );
  CPen *pOldPen = pDC->SelectObject( &pen );
  

  int iStartPos = m_nSeperateSize;  

  CFont font;
  font.CreatePointFont( 70 , _T( "Arial" ) );

  CRect rectText;

  int iLLineStart , iLLineEnd;
  int iMLineStart , iMLineEnd;
  int iSLineStart , iSLineEnd;

  if( m_dwStyle & RWSTYLE_BOTTOMALIGN ) {

     rectText.top     = rectRuler.bottom - 20;
	 rectText.bottom  = rectRuler.bottom - 10;

     iLLineStart = rectRuler.bottom - 9;
     iLLineEnd	 = rectRuler.bottom - 2;

     iMLineStart = rectRuler.bottom - 6;
	 iMLineEnd	 = rectRuler.bottom - 2;

     iSLineStart = rectRuler.bottom - 4;
	 iSLineEnd	 = rectRuler.bottom - 2;

  }
  else if( m_dwStyle & RWSTYLE_TOPALIGN ) {

	 rectText.top     = rectRuler.top + 10;
	 rectText.bottom  = rectRuler.top + 20;
     
     iLLineStart = rectRuler.top + 2;
     iLLineEnd	 = rectRuler.top + 9;

     iMLineStart = rectRuler.top + 2;
	 iMLineEnd	 = rectRuler.top + 6;

     iSLineStart = rectRuler.top + 2;
	 iSLineEnd	 = rectRuler.top + 4;

  }


  CFont *pOldFont = pDC->SelectObject( &font );

  CString str;
  
  int iPos;
  int iSel = 0;

  pDC->SetBkMode( TRANSPARENT );
  pDC->SetTextColor( m_clrTextColor );
  
  for( int iSayac = 0 ; iSayac <= (int)m_nRulerMargin; iSayac++ ) {
  
	 iPos = iStartPos + ( iSayac * m_nMilimeterPixel );

	 iPos -= m_lScrolPos;

	 
	 if( iPos < rectRuler.left ) {

	    iSel++;

	    if( iSel == 10 )
		   iSel = 0;

		continue;
	 }

	 if( iPos > rectRuler.right )
		break;



	 if( iSel == 0 ) {

        rectText.left     = iPos - 20;
  	    rectText.right    = iPos + 22;

		str.Format( _T( "%ld" ) , iSayac /*/ 10*/ + m_nOffsetDisplay );

		
		pDC->DrawText( str , &rectText , DT_CENTER /*| DT_VCENTER | DT_SINGLELINE*/ );

        pDC->MoveTo(  iPos , iLLineStart );
        pDC->LineTo(  iPos , iLLineEnd   );
		
	 }
	 else
	 if( iSel == 5 ) {
        pDC->MoveTo( iPos , iMLineStart );
        pDC->LineTo( iPos , iMLineEnd   );
	 }
     else {
        pDC->MoveTo( iPos , iSLineStart );
        pDC->LineTo( iPos , iSLineEnd   );
	 }

	 iSel++;

	 if( iSel == 10 )
		iSel = 0;

  };// for


  
  //Seperatorler çiziliyor...
  SEPERATOR_TYPE *pSep = NULL;

  for( int iSayac = 0 ; iSayac < m_Seperators.GetSize() ; iSayac++ ) {

  	 pSep = (SEPERATOR_TYPE *)m_Seperators.GetAt( iSayac );
	 if( pSep != NULL ) {

	    iPos = iStartPos + ( pSep->iPos * m_nMilimeterPixel );

	    iPos -= m_lScrolPos;


	    if( iPos < rectRuler.left - (int)m_nSeperatorSize )
		   continue;

	    if( iPos > rectRuler.right + (int)m_nSeperatorSize )
		   continue;
		  

		DrawHSeperator( pDC , pSep ); 

	 }

  }; //for


  pDC->SelectObject( pOldFont );
  font.DeleteObject();

  pDC->SelectObject( pOldPen );
  pen.DeleteObject();



  return TRUE;
}



BOOL CRulerWnd::DrawVRuler(CDC *pDC)
{

  //Ölçüler çiziliyor....
  CRect rectRuler;
  GetClientRect( &rectRuler );


  CPen pen;
  pen.CreatePen( PS_SOLID , 1 , m_clrMilimeterLineColor );
  CPen *pOldPen = pDC->SelectObject( &pen );
  

  int iStartPos = m_nSeperateSize;  


  LOGFONT lf;

  memset( &lf , 0 , sizeof( lf ) );

  CFont font;
  font.CreatePointFont( 70 , _T( "Arial" ) );
  font.GetLogFont( &lf );
  font.DeleteObject();

  CRect rectText;

  int iLLineStart , iLLineEnd;
  int iMLineStart , iMLineEnd;
  int iSLineStart , iSLineEnd;

  if( m_dwStyle & RWSTYLE_RIGHTALIGN ) {

     lf.lfOrientation = 900;
     lf.lfEscapement  = 900;

     rectText.left  = rectRuler.right - 21;

     iLLineStart = rectRuler.right - 9;
     iLLineEnd	 = rectRuler.right - 2;

     iMLineStart = rectRuler.right - 6;
	 iMLineEnd	 = rectRuler.right - 2;

     iSLineStart = rectRuler.right - 4;
	 iSLineEnd	 = rectRuler.right - 2;

  }
  else if( m_dwStyle & RWSTYLE_LEFTALIGN ) {

     lf.lfOrientation = -900;
     lf.lfEscapement  = -900;

	 rectText.left  = rectRuler.left + 21;
     


     iLLineStart = rectRuler.left  + 2;
     iLLineEnd	 = rectRuler.left  + 9;

     iMLineStart = rectRuler.left  + 2;
	 iMLineEnd	 = rectRuler.left  + 6;

     iSLineStart = rectRuler.left + 2;
	 iSLineEnd	 = rectRuler.left + 4;


  }


  font.CreateFontIndirect( &lf );

  CFont *pOldFont = pDC->SelectObject( &font );

  CString str;
  
  int iPos;
  int iSel = 0;

  pDC->SetBkMode( TRANSPARENT );
  pDC->SetTextColor( m_clrTextColor );
  
  for( int iSayac = 0 ; iSayac <= (int)m_nRulerMargin; iSayac++ ) {
  
	 iPos = iStartPos + ( iSayac * m_nMilimeterPixel );

	 iPos -= m_lScrolPos;

	 
	 if( iPos < rectRuler.top ) {

	    iSel++;

	    if( iSel == 10 )
		   iSel = 0;

		continue;
	 }

	 if( iPos > rectRuler.bottom )
		break;



	 if( iSel == 0 ) {

		if( m_dwStyle & RWSTYLE_LEFTALIGN )
	       rectText.top    = iPos - 2;
		else
		   rectText.top    = iPos + 3;

		str.Format( _T( "%ld" ) , -iSayac /*/ 10*/ - m_nOffsetDisplay );

		pDC->TextOut( rectText.left , rectText.top  , str );

        pDC->MoveTo( iLLineStart     , iPos );
        pDC->LineTo( iLLineEnd       , iPos );
		
	 }
	 else
	 if( iSel == 5 ) {
        pDC->MoveTo( iMLineStart      , iPos );
        pDC->LineTo( iMLineEnd        , iPos );
	 }
     else {
        pDC->MoveTo( iSLineStart     , iPos );
        pDC->LineTo( iSLineEnd       , iPos );
	 }

	 iSel++;

	 if( iSel == 10 )
		iSel = 0;

  };// for



  //Seperatorler çiziliyor...
  SEPERATOR_TYPE *pSep = NULL;

  for( int iSayac = 0 ; iSayac < m_Seperators.GetSize() ; iSayac++ ) {

  	 pSep = (SEPERATOR_TYPE *)m_Seperators.GetAt( iSayac );
	 if( pSep != NULL ) {
		
	    iPos = iStartPos + ( pSep->iPos * m_nMilimeterPixel );

	    iPos -= m_lScrolPos;


	    if( iPos < rectRuler.top - (int)m_nSeperatorSize )
		   continue;

	    if( iPos > rectRuler.bottom + (int)m_nSeperatorSize )
		   continue;
	  

		DrawVSeperator( pDC , pSep ); 

	 }

  }; //for




  pDC->SelectObject( pOldFont );
  font.DeleteObject();

  pDC->SelectObject( pOldPen );
  pen.DeleteObject();


  return TRUE;
}


CRulerWnd::SEPERATOR_TYPE* 
CRulerWnd::GetSeperator( int iID ) {

   SEPERATOR_TYPE *pSep = NULL;
   if( m_Seperators.GetSize() > 0 ) {

	  for( int iSayac = 0 ; iSayac < m_Seperators.GetSize() ; iSayac++ ) {

		 pSep = (SEPERATOR_TYPE *)m_Seperators.GetAt( iSayac );
		 if( pSep != NULL && pSep->iID == iID ) {
			return pSep;

		 }

	  }; //for

   }

   return NULL;

}


int 
CRulerWnd::DeleteAllSeperator() {

   SEPERATOR_TYPE *pSep = NULL;
   if( m_Seperators.GetSize() > 0 ) {

	  for( int iSayac = 0 ; iSayac < m_Seperators.GetSize() ; iSayac++ ) {

		 pSep = (SEPERATOR_TYPE *)m_Seperators.GetAt( iSayac );
		 if( pSep != NULL )
			delete pSep;

	  }; //for

   }

   m_Seperators.RemoveAll();

   return 0;



}


int 
CRulerWnd::DeleteSeperator( int iID ) {

   SEPERATOR_TYPE *pSep = NULL;
   if( m_Seperators.GetSize() > 0 ) {

	  for( int iSayac = 0 ; iSayac < m_Seperators.GetSize() ; iSayac++ ) {

		 pSep = (SEPERATOR_TYPE *)m_Seperators.GetAt( iSayac );
		 if( pSep != NULL && pSep->iID == iID ) {
			delete pSep;
			m_Seperators.RemoveAt( iSayac );
			return iSayac;

		 }

	  }; //for

   }

   return -1;
 
}



int 
CRulerWnd::AddSeperator( int iPos , int iID , int iType , LPARAM lParam , COLORREF	clrLine , COLORREF clrFill , int iMinMargin , int iMaxMargin ) {

   SEPERATOR_TYPE *pSep = new SEPERATOR_TYPE;
   if( pSep != NULL ) {

	  memset( pSep , 0 , sizeof( *pSep ) );
	  pSep->iPos    = iPos;
	  pSep->iID     = iID;
	  pSep->iType   = iType;
	  pSep->lParam  = lParam;
	  pSep->clrLine = clrLine;
	  pSep->clrFill = clrFill;
	  pSep->iMinMargin = iMinMargin;
	  pSep->iMaxMargin = iMaxMargin;

	  return m_Seperators.Add( pSep );

   }

   return -1;


}
								
int CRulerWnd::UpdateSeperator( int iID, int iPos )
{
	CRulerWnd::SEPERATOR_TYPE *pSep = GetSeperator( iID );
	if( pSep )
	{
		if( m_dwStyle & RWSTYLE_VERT )
			iPos *= -1;
		pSep->iPos = iPos - m_nOffsetDisplay;
		Invalidate();
	}
	return 0;
}

BOOL CRulerWnd::DrawHSeperator( CDC *pDC , SEPERATOR_TYPE *pSep )
{
	

	if( pSep == NULL || pSep->iPos < 0 || pSep->iPos > (int)m_nRulerMargin )
	   return FALSE;
	


	CRect rect , rectClient;
	
	GetClientRect( &rectClient );

    int iNewPos = m_nSeperateSize + ( pSep->iPos * m_nMilimeterPixel );

    iNewPos -= m_lScrolPos;

    CPen pen;
    pen.CreatePen( PS_SOLID , 1 , pSep->clrLine );

    CPen *pOldPen = pDC->SelectObject( &pen );

	rect.left    = iNewPos - m_nSeperatorSize;
	rect.right   = iNewPos + m_nSeperatorSize;

    if( m_dwStyle & RWSTYLE_BOTTOMALIGN ) {

	   rect.top    = rectClient.bottom - ( ( m_nSeperatorSize * 2 ) + 2 ) ;
	   rect.bottom = rectClient.bottom - 2;

	   pDC->FillSolidRect( &rect , pSep->clrFill );

       pDC->MoveTo( rect.left  , rect.top );
       pDC->LineTo( rect.right , rect.top );
	   pDC->LineTo( rect.right , rect.bottom );
	   pDC->LineTo( rect.left  , rect.bottom );
	   pDC->LineTo( rect.left  , rect.top );
	  
       pDC->MoveTo( rect.left  + 2  , rect.top + 2 );
       pDC->LineTo( rect.right - 2  , rect.top + 2 );
	   pDC->LineTo( iNewPos         , rect.bottom - 2 );
	   pDC->LineTo( rect.left + 2   , rect.top + 2 );
     
	}
    else if( m_dwStyle & RWSTYLE_TOPALIGN ) {


	   rect.top    = rectClient.top  + 2;  
	   rect.bottom = rectClient.top  + ( ( m_nSeperatorSize * 2 ) + 2 ) ;

	   pDC->FillSolidRect( &rect , pSep->clrFill );

       pDC->MoveTo( rect.left  , rect.top );
       pDC->LineTo( rect.right , rect.top );
	   pDC->LineTo( rect.right , rect.bottom );
	   pDC->LineTo( rect.left  , rect.bottom );
	   pDC->LineTo( rect.left  , rect.top );
	   

       pDC->MoveTo( rect.right - 2  , rect.bottom - 2 );
       pDC->LineTo( rect.left  + 2  , rect.bottom - 2 );
	   pDC->LineTo( iNewPos         , rect.top + 2 );
	   pDC->LineTo( rect.right - 2  , rect.bottom - 2 );

	}
	 
	pDC->SelectObject( pOldPen );

    return TRUE;
}




BOOL CRulerWnd::DrawVSeperator( CDC *pDC , SEPERATOR_TYPE *pSep )
{

	if( pSep == NULL || pSep->iPos < 0 || pSep->iPos > (int)m_nRulerMargin )
	   return FALSE;

	CRect rect , rectClient;
	
	GetClientRect( &rectClient );

    int iNewPos = m_nSeperateSize + ( pSep->iPos * m_nMilimeterPixel );

    iNewPos -= m_lScrolPos;

    CPen pen;
    pen.CreatePen( PS_SOLID , 1 , pSep->clrLine );

    CPen *pOldPen = pDC->SelectObject( &pen );

	rect.top    = iNewPos - m_nSeperatorSize;
	rect.bottom = iNewPos + m_nSeperatorSize;

    if( m_dwStyle & RWSTYLE_RIGHTALIGN ) {

	   rect.left  = rectClient.right - ( ( m_nSeperatorSize * 2 ) + 2 ) ;
	   rect.right = rectClient.right - 2;

	   pDC->FillSolidRect( &rect , pSep->clrFill );

       pDC->MoveTo( rect.left  , rect.top );
       pDC->LineTo( rect.right , rect.top );
	   pDC->LineTo( rect.right , rect.bottom );
	   pDC->LineTo( rect.left  , rect.bottom );
	   pDC->LineTo( rect.left  , rect.top );
	   
       pDC->MoveTo( rect.left  + 2  , rect.top    + 2 );
       pDC->LineTo( rect.left  + 2  , rect.bottom - 2 );
	   pDC->LineTo( rect.right - 2  , iNewPos );
	   pDC->LineTo( rect.left + 2   , rect.top + 2 );

     
	}
    else if( m_dwStyle & RWSTYLE_LEFTALIGN ) {

	   rect.left  = rectClient.left  + 2 ;
	   rect.right = rectClient.left  + ( ( m_nSeperatorSize * 2 ) + 2 );

	   pDC->FillSolidRect( &rect , pSep->clrFill );

       pDC->MoveTo( rect.left  , rect.top );
       pDC->LineTo( rect.right , rect.top );
	   pDC->LineTo( rect.right , rect.bottom );
	   pDC->LineTo( rect.left  , rect.bottom );
	   pDC->LineTo( rect.left  , rect.top );
	   

       pDC->MoveTo( rect.right - 2  , rect.top    + 2 );
       pDC->LineTo( rect.right - 2  , rect.bottom - 2 );
	   pDC->LineTo( rect.left  + 2  , iNewPos );
	   pDC->LineTo( rect.right - 2  , rect.top + 2 );

	}
	 
	pDC->SelectObject( pOldPen );

    return TRUE;
}








//Set Property
BOOL
CRulerWnd::SetStyle( DWORD dwStyle ) { 
  m_dwStyle = dwStyle;
  return TRUE;
}

BOOL
CRulerWnd::SetBackGroundColor( COLORREF clr ) { 
  m_clrBackGround = clr;
  return TRUE;
}

BOOL
CRulerWnd::SetMilimeterLineColor( COLORREF clr ) { 
  m_clrMilimeterLineColor = clr;
  return TRUE;
}

BOOL
CRulerWnd::SetTextColor( COLORREF clr ) { 
  m_clrTextColor = clr; 
  return TRUE;
}


BOOL
CRulerWnd::SetStartSeperateSize( UINT nSize ) { 
  m_nSeperateSize = nSize; 
  return TRUE;
}

BOOL
CRulerWnd::SetMargin( UINT nMargin ) { 
  m_nRulerMargin = nMargin; 
  return TRUE;
}

BOOL
CRulerWnd::SetMilimeterPixel( UINT nPixel ) { 
  m_nMilimeterPixel = nPixel; 
  return TRUE;
}

BOOL
CRulerWnd::SetSeperatorSize( UINT nSize ) { 
  m_nSeperatorSize = nSize; 
  return TRUE;
}

BOOL
CRulerWnd::SetScrollPos( long lPos ) { 
  m_lScrolPos = lPos; 
  return TRUE;
}


BOOL CRulerWnd::SetOffsetDisplay( long nOffset )
{
	m_nOffsetDisplay = nOffset;
	return TRUE;
}

BOOL     
CRulerWnd::SetMessageTarget( CWnd *pTarget ) {
  m_pMessageTarget = pTarget;
  return TRUE;
}



void CRulerWnd::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	CWnd::OnTimer(nIDEvent);
}

void CRulerWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_bLButtonDown = TRUE;
	
	m_iDragID = SeperatorHitTest( point );
	if( m_iDragID != -1 ) {

       SEPERATOR_TYPE *pSep = GetSeperator( m_iDragID );
	   if( pSep != NULL )
		  m_iDragPos = pSep->iPos;

	}

	

	CWnd::OnLButtonDown(nFlags, point);
}

void CRulerWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if( m_bLButtonDown && !m_bDraging && m_iDragID >= 0 ) {

	   m_bDraging = TRUE;

	   SetCursorClipRect();

	   m_iOldDragingPos = -1;

	}

	if( m_bDraging ) {

	   DraggingSeperator( m_iDragID , point );

	}


	CWnd::OnMouseMove(nFlags, point);

}

void CRulerWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{

	ClipCursor( NULL );

	// TODO: Add your message handler code here and/or call default
	if( m_bDraging && m_iDragID >= 0 ) {

	   int  iNewPos = GetMostPos( point );
	   if( iNewPos >= 0 ) {

          SEPERATOR_TYPE *pSep = GetSeperator( m_iDragID );
		  if( pSep != NULL && pSep->iPos != iNewPos ) {
			 
			 SendParentNotifyMessage( NMSUB_RULER_SEPERATORCHANGE  , pSep->iID , iNewPos , pSep->iPos );

			 pSep->iPos = iNewPos;
		 

		  }

	   }

	   Invalidate();

	};


	m_bLButtonDown = m_bDraging = FALSE;
	m_iDragPos = m_iDragID = -1;

	CWnd::OnLButtonUp(nFlags, point);
}

BOOL CRulerWnd::DraggingSeperator( int iDragID , CPoint point )
{


  CDC *pDC = GetDC();
  if( pDC == NULL )
	 return FALSE;

  CRect rect;
  GetClientRect( &rect );

  int iNewPos = 0;

  if( m_dwStyle & RWSTYLE_VERT ) {

	 int iPos = GetMostPos( point );

	 if( iPos != m_iOldDragingPos ) {

        //Before Erase
        if( m_iOldDragingPos != -1 ) {
	 
           iNewPos = m_nSeperateSize + ( m_iOldDragingPos * m_nMilimeterPixel );
           iNewPos -= m_lScrolPos;

		   rect.top    = iNewPos;
		   rect.bottom = iNewPos + 1;
		   pDC->InvertRect( &rect );

		   m_iOldDragingPos =-1;

		}

        if( iPos != -1 ) {

		   SendParentNotifyMessage( NMSUB_RULER_SEPERATORCHANGING  , iDragID , iPos , m_iDragPos );

	       m_iOldDragingPos = iPos;

	       iNewPos = m_nSeperateSize + ( iPos * m_nMilimeterPixel );
           iNewPos -= m_lScrolPos;

		   rect.top    = iNewPos;
		   rect.bottom = iNewPos + 1;
		   pDC->InvertRect( &rect );

		}
		else
        if( m_iOldDragingPos != -1  ) {
	 
           iNewPos = m_nSeperateSize + ( m_iOldDragingPos * m_nMilimeterPixel );
           iNewPos -= m_lScrolPos;

		   rect.top    = iNewPos;
		   rect.bottom = iNewPos + 1;
		   pDC->InvertRect( &rect );

		   m_iOldDragingPos =-1;

		}
 

		


	 }

  }
  else
  if( m_dwStyle & RWSTYLE_HORZ ) {

	 int iPos = GetMostPos( point );

	 if( iPos != m_iOldDragingPos ) {

        //Before Erase
        if( m_iOldDragingPos != -1 ) {
	 
           iNewPos = m_nSeperateSize + ( m_iOldDragingPos * m_nMilimeterPixel );
           iNewPos -= m_lScrolPos;

		   rect.left    = iNewPos;
		   rect.right   = iNewPos + 1;
		   pDC->InvertRect( &rect );

		}

        if( iPos != -1 ) {

		   SendParentNotifyMessage( NMSUB_RULER_SEPERATORCHANGING  , iDragID , iPos , m_iDragPos );

	       m_iOldDragingPos = iPos;

	       iNewPos = m_nSeperateSize + ( iPos * m_nMilimeterPixel );
           iNewPos -= m_lScrolPos;

		   rect.left    = iNewPos;
		   rect.right   = iNewPos + 1;
		   pDC->InvertRect( &rect );


		}

	 }

  }









  return TRUE;
}



int CRulerWnd::SeperatorHitTest(CPoint point)
{

   CRect rect , rectClient;

   GetClientRect( &rectClient );

   int iNewPos = 0;


   SEPERATOR_TYPE *pSep = NULL;

   for( int iSayac = 0 ; iSayac < m_Seperators.GetSize() ; iSayac++ ) {

  	  pSep = (SEPERATOR_TYPE *)m_Seperators.GetAt( iSayac );
	  if( pSep != NULL && !( pSep->iType & SEPTYPE_NOPOSCHANGE ) ) {

         iNewPos = m_nSeperateSize + ( pSep->iPos * m_nMilimeterPixel );

         iNewPos -= m_lScrolPos;

		 
         if( m_dwStyle & RWSTYLE_VERT ) {

	        rect.top    = iNewPos - m_nSeperatorSize;
	        rect.bottom = iNewPos + m_nSeperatorSize;

			if( rect.bottom < rectClient.top )
			   continue;

			if( rect.top > rectClient.bottom )
			   continue;

            if( m_dwStyle & RWSTYLE_RIGHTALIGN ) {

	           rect.left  = rectClient.right - ( ( m_nSeperatorSize * 2 ) + 2 ) ;
	           rect.right = rectClient.right - 2;

		       if( rect.PtInRect( point ) && pSep->iPos >= 0 && pSep->iPos <= (int)m_nRulerMargin )
     			  return pSep->iID;

			}
            else if( m_dwStyle & RWSTYLE_LEFTALIGN ) {

	           rect.left  = rectClient.left  + 2 ;
	           rect.right = rectClient.left  + ( ( m_nSeperatorSize * 2 ) + 2 );

		       if( rect.PtInRect( point ) && pSep->iPos >= 0 && pSep->iPos <= (int)m_nRulerMargin )
     			  return pSep->iID;


			}

		 }
         else
         if( m_dwStyle & RWSTYLE_HORZ ) {


         	rect.left    = iNewPos - m_nSeperatorSize;
	        rect.right   = iNewPos + m_nSeperatorSize;

			if( rect.right < rectClient.left )
			   continue;

			if( rect.left > rectClient.right )
			   continue;


            if( m_dwStyle & RWSTYLE_BOTTOMALIGN ) {

	           rect.top    = rectClient.bottom - ( ( m_nSeperatorSize * 2 ) + 2 ) ;
	           rect.bottom = rectClient.bottom - 2;

		       if( rect.PtInRect( point ) && pSep->iPos >= 0 && pSep->iPos <= (int)m_nRulerMargin )
     			  return pSep->iID;


     
			}
            else if( m_dwStyle & RWSTYLE_TOPALIGN ) {

	           rect.top    = rectClient.top  + 2;  
	           rect.bottom = rectClient.top  + ( ( m_nSeperatorSize * 2 ) + 2 ) ;

		       if( rect.PtInRect( point ) && pSep->iPos >= 0 && pSep->iPos <= (int)m_nRulerMargin )
     			  return pSep->iID;

			}

		 }

	  }

   } ;//for
   

   return -1;
}


int CRulerWnd::GetMostPos( CPoint point )
{

   double dfToplam , dfKesir;	

   int    iNewPos  = -1;
   
   if( m_dwStyle & RWSTYLE_VERT ) {

      iNewPos = ( point.y - m_nSeperateSize ) + m_lScrolPos;


   }
   else
   if( m_dwStyle & RWSTYLE_HORZ ) {

	 iNewPos = ( point.x - m_nSeperateSize ) + m_lScrolPos;

   }
   

   dfToplam = (double)iNewPos / (double)m_nMilimeterPixel;
   iNewPos  = iNewPos / m_nMilimeterPixel;

   dfKesir = modf( dfToplam , &dfToplam );
   if( dfKesir >= 0.5 )
     iNewPos++;

   if( iNewPos < 0 || iNewPos > (int)m_nRulerMargin )
	  return -1;

   return iNewPos;


  
   return -1;
}


void CRulerWnd::SetCursorClipRect()
{

  CRect rect;
  GetClientRect( &rect );

  int iMinMargin = 0;
  int iMaxMargin = 0xFFFFFFF;

  if( m_iDragID != -1 ) {

     SEPERATOR_TYPE *pSep = GetSeperator( m_iDragID );
     if( pSep != NULL ) {

   	    iMinMargin = pSep->iMinMargin;
        iMaxMargin = pSep->iMaxMargin;
		 
	 }

  }

  
  if( m_dwStyle & RWSTYLE_VERT ) {

     int iTop      = m_nSeperateSize;
     int iBottom   = m_nSeperateSize + ( m_nRulerMargin * m_nMilimeterPixel );

	 int iMinRange = m_nSeperateSize + ( iMinMargin * m_nMilimeterPixel );
	 int iMaxRange = m_nSeperateSize + ( iMaxMargin * m_nMilimeterPixel );

     iTop        -= m_lScrolPos;
	 iBottom     -= m_lScrolPos;
     iMinRange   -= m_lScrolPos;
	 iMaxRange   -= m_lScrolPos;


	 if( rect.top < iTop )
	    rect.top    = iTop;

	 if( rect.top < iMinRange )
		rect.top = iMinRange;

	 if( rect.bottom > iBottom )
	    rect.bottom = iBottom;

	 if( rect.bottom > iMaxRange )
	    rect.bottom = iMaxRange;

     ClientToScreen( &rect );
	 ClipCursor( &rect );

  }
  else
  if( m_dwStyle & RWSTYLE_HORZ ) {


     int iLeft    = m_nSeperateSize;
     int iRight   = m_nSeperateSize + ( m_nRulerMargin * m_nMilimeterPixel );

	 int iMinRange = m_nSeperateSize + ( iMinMargin * m_nMilimeterPixel );
	 int iMaxRange = m_nSeperateSize + ( iMaxMargin * m_nMilimeterPixel );


     iLeft   -= m_lScrolPos;
	 iRight  -= m_lScrolPos;
     iMinRange   -= m_lScrolPos;
	 iMaxRange   -= m_lScrolPos;


	 if( rect.left < iLeft )
	    rect.left  = iLeft;

	 if( rect.left < iMinRange )
	    rect.left  = iMinRange;

	 if( rect.right > iMaxRange )
	    rect.right = iMaxRange;

     ClientToScreen( &rect );
	 ClipCursor( &rect );

  }

}



int CRulerWnd::GetClientPixelFromPos( int iPos )
{
   
  int iClientPos = m_nSeperateSize + ( iPos * m_nMilimeterPixel );

  iClientPos -= m_lScrolPos;

  return iClientPos;
  
}


LRESULT CRulerWnd::SendParentNotifyMessage( UINT nSubMessage, int iSepID , int iNewPos , int iOldPos , int iParam1 , int iParam2 )
{

   memset( &m_NotifyInfo , 0 , sizeof( m_NotifyInfo ) );

   //Header
   m_NotifyInfo.hdr.hwndFrom = m_hWnd;
   m_NotifyInfo.hdr.idFrom   = GetDlgCtrlID();
   m_NotifyInfo.hdr.code     = NM_RULER_NOTIFICATIONMESSAGE;

   //Data
   m_NotifyInfo.nSubMessage  = nSubMessage;
   m_NotifyInfo.iNewPos      = iNewPos;
   m_NotifyInfo.iOldPos      = iOldPos;
   m_NotifyInfo.iSepID       = iSepID;
   m_NotifyInfo.iParam1      = iParam1;
   m_NotifyInfo.iParam2      = iParam2;
   m_NotifyInfo.dwRulerStyle = m_dwStyle;

   CWnd *pWnd = GetParent();

   if( m_pMessageTarget != NULL && IsWindow( m_pMessageTarget->m_hWnd ) )
	  pWnd = m_pMessageTarget;

   if( pWnd != NULL && IsWindow( pWnd->m_hWnd ) )
      return pWnd->SendMessage( WM_NOTIFY , (WPARAM)m_NotifyInfo.hdr.idFrom , (LPARAM)&m_NotifyInfo );

   return -1;
}
