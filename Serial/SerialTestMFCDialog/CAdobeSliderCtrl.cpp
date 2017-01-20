
#include "StdAfx.h"
#include "CAdobeSliderCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCustomDrawSliderCtrl::CCustomDrawSliderCtrl()
{	
}

CCustomDrawSliderCtrl::~CCustomDrawSliderCtrl()
{
}

BEGIN_MESSAGE_MAP(CCustomDrawSliderCtrl, CSliderCtrl)
	ON_WM_ERASEBKGND()
	ON_NOTIFY_REFLECT( NM_CUSTOMDRAW, OnCustomDraw )	
END_MESSAGE_MAP()

//
// CCustomDrawSliderCtrl message handlers
//
BOOL CCustomDrawSliderCtrl::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CCustomDrawSliderCtrl::OnCustomDraw ( NMHDR* pNMHDR, LRESULT* pResult)
{		
	NMCUSTOMDRAW nmcd = *(LPNMCUSTOMDRAW) pNMHDR;

	UINT drawStage = nmcd.dwDrawStage;
	UINT itemSpec = (UINT)nmcd.dwItemSpec;
	
	switch ( drawStage )
	{
		case CDDS_PREPAINT:
			// most important of the drawing stages must return CDRF_NOTIFYITEMDRAW or else we will not get further 
			// NM_CUSTOMDRAW notifications for this drawing cycle we also return CDRF_NOTIFYPOSTPAINT 
			// so that we will get post-paint notifications		
			*pResult = CDRF_NOTIFYITEMDRAW | CDRF_NOTIFYPOSTPAINT ;
		break;

		case CDDS_PREERASE:		// Before the erase cycle begins
		case CDDS_POSTERASE:	// After the erase cycle is complete
		case CDDS_ITEMPREERASE:	// Before an item is erased
		case CDDS_ITEMPOSTERASE:	// After an item has been erased
			*pResult = CDRF_DODEFAULT;
		break;

		case CDDS_ITEMPREPAINT:
			switch (itemSpec)
			{
				case TBCD_CHANNEL:					
					*pResult = CDRF_SKIPDEFAULT;					
				break;

				case TBCD_TICS:					
					*pResult = CDRF_DODEFAULT;
				break;

				case TBCD_THUMB:
				{										
					CDC* pDC = CDC::FromHandle( nmcd.hdc );
					Draw(pDC, &nmcd);

					*pResult = CDRF_SKIPDEFAULT;
				}
				break;
			}
		break;

		case CDDS_ITEMPOSTPAINT:	// After an item has been drawn
			switch ( itemSpec )
			{				
				case TBCD_TICS:					
					*pResult = CDRF_DODEFAULT;
				break;

				case TBCD_THUMB:
					*pResult = CDRF_DODEFAULT;	
				break;
			}

		break;
	}	
}

//
// CCustomDrawSliderCtrl Methods
//
void CCustomDrawSliderCtrl::Draw(CDC *pDC, LPNMCUSTOMDRAW lpcd)
{	
	CRect rc;
	GetWindowRect(rc);
	GetClientRect(rc);
	// The first drawing is to make sure the thumb area is painted properly with the background
	// and the edges
	OnDraw(pDC, rc);	

	// The next drawing is to make sure the thumb is drawn properly covering the whole client area
	CClientDC dc(this);
	OnDraw(&dc, rc);	
}



///////////////////////////////////////////////////
// CAdobeSliderCtrl codes
/////////////////////////////////////////////////////

CAdobeSliderCtrl::CAdobeSliderCtrl()
{
	m_clrBkgnd = ::GetSysColor(COLOR_BTNFACE);
	m_rcDrawArea.SetRectEmpty();
}

CAdobeSliderCtrl::~CAdobeSliderCtrl()
{
}

// methods
BOOL CAdobeSliderCtrl::GetCrossSectionPoints(POINT ptOuterCrossSec[], POINT ptInnerCrossSec[])
{
	if(m_rcDrawArea.IsRectEmpty())
		return FALSE;

	CRect rcDrawArea = m_rcDrawArea;

	CRect rcThumb;
	GetThumbRect(rcThumb);	

	int nThumbHeight = rcThumb.Height();	
	int nDeviation = rcThumb.Width();	

	rcThumb.top = rcDrawArea.top + nDeviation;
	rcThumb.bottom = rcThumb.top + nThumbHeight;

	CRect rcThumb1 = rcThumb;
	rcThumb1.DeflateRect(4, 4);	

	int nBarSize = 3 * nDeviation / 2;		
	
	CPoint ptCrossSecLeftTop(rcThumb.left, rcThumb.top);
	CPoint ptCrossSecRightTop(rcThumb.right, rcThumb.top - nDeviation);
	CPoint ptCrossSecRightBottom(rcThumb.right, rcThumb.bottom - nDeviation);
	CPoint ptCrossSecLeftBottom(rcThumb.left, rcThumb.bottom);
	
	int nCrossectionPointY = (ptCrossSecRightTop.y + ptCrossSecRightBottom.y)  / 2;

	CPoint ptInnerCrossSecRightTop(ptCrossSecRightTop.x -  (ptCrossSecRightTop.x - ptCrossSecLeftTop.x)/3, nCrossectionPointY);
	CPoint ptInnerCrossSecLeftBottom(ptCrossSecLeftTop.x +  (ptCrossSecRightTop.x - ptCrossSecLeftTop.x)/3, nCrossectionPointY + nBarSize);	

	int nWidth =  (ptInnerCrossSecRightTop.x - ptInnerCrossSecLeftBottom.x);
	CPoint ptInnerCrossSecRightBottom(ptInnerCrossSecRightTop.x, ptInnerCrossSecLeftBottom.y - nWidth);	
	int nHeight = (ptInnerCrossSecRightTop.y - ptInnerCrossSecRightBottom.y);
	CPoint ptInnerCrossSecLeftTop(ptInnerCrossSecLeftBottom.x, ptInnerCrossSecLeftBottom.y + nHeight);			

	ptInnerCrossSecLeftBottom.y-=2;
	ptInnerCrossSecRightBottom.y-=2;

	ptOuterCrossSec[0] = ptCrossSecLeftTop;
	ptOuterCrossSec[1] = ptCrossSecRightTop;
	ptOuterCrossSec[2] = ptCrossSecRightBottom;
	ptOuterCrossSec[3] = ptCrossSecLeftBottom;

	ptInnerCrossSec[0] = ptInnerCrossSecLeftTop;
	ptInnerCrossSec[1] = ptInnerCrossSecRightTop;
	ptInnerCrossSec[2] = ptInnerCrossSecRightBottom;
	ptInnerCrossSec[3] = ptInnerCrossSecLeftBottom;

	return TRUE;
}


// Overriden methods
void CAdobeSliderCtrl::OnDraw(CDC* pDC, CRect rcClient)
{	
	COLORREF clrBkgnd = GetBkgndColor();
	if(clrBkgnd == CLR_NONE)
	{
		clrBkgnd = ::GetSysColor(COLOR_BTNFACE);
	}

	CRect rcBkgnd = rcClient;	

	// Adjust the padding of the slider thumb with respect to the edges
	rcClient.left += 4;
	rcClient.right -= 7;	

	m_rcDrawArea = rcClient;	

	POINT ptOuter[4];
	POINT ptInner[4];
	GetCrossSectionPoints(ptOuter, ptInner);
	
	CPoint ptCrossSecLeftTop(ptOuter[0]);
	CPoint ptCrossSecRightTop(ptOuter[1]);
	CPoint ptCrossSecRightBottom(ptOuter[2]);
	CPoint ptCrossSecLeftBottom(ptOuter[3]);	

	CPoint ptInnerCrossSecLeftTop(ptInner[0]);			
	CPoint ptInnerCrossSecRightTop(ptInner[1]);
	CPoint ptInnerCrossSecRightBottom(ptInner[2]);	
	CPoint ptInnerCrossSecLeftBottom(ptInner[3]);		
	
	// First prepare to draw the left and right bottom half shades
	COLORREF clrShadeLeft = RGB(231, 233, 236);
	COLORREF clrShadeRight = RGB(247, 248, 249);	
	
	CRect rcShadeLeft(rcClient.left, ptInnerCrossSecLeftTop.y + 2, ptInnerCrossSecLeftTop.x, ptInnerCrossSecLeftBottom.y + 3);	
	CRect rcShadeRight(rcShadeLeft.right, ptInnerCrossSecLeftTop.y + 2, rcClient.right, ptInnerCrossSecLeftBottom.y + 3);			

	int nShadeTopLeftX = rcClient.left + 5;
	int nShadeBottomRightX = rcClient.right - 5;
	
	// Construct the top half whitish bar
	int nMargin = 0;

	POINT ptUpper[5];
	ptUpper[0].x = rcClient.right - nMargin;
	ptUpper[0].y = ptInnerCrossSecLeftBottom.y - nMargin;
	ptUpper[1].x = rcClient.left + nMargin;
	ptUpper[1].y = ptInnerCrossSecLeftBottom.y - nMargin;
	ptUpper[2].x = rcClient.left + nMargin;
	ptUpper[2].y = ptInnerCrossSecLeftTop.y + nMargin;
	ptUpper[3].x = nShadeTopLeftX;
	ptUpper[3].y = ptInnerCrossSecRightTop.y + nMargin;
	ptUpper[4].x = rcClient.right - nMargin;
	ptUpper[4].y = ptInnerCrossSecRightTop.y + nMargin;
	
	CRgn rgn;
	rgn.CreatePolygonRgn(ptUpper, 5, WINDING);

	pDC->SelectClipRgn(&rgn, RGN_DIFF);
	pDC->ExcludeClipRect(rcShadeLeft);
	pDC->ExcludeClipRect(rcShadeRight);

	pDC->FillSolidRect(rcBkgnd, clrBkgnd);
	
	// Remove all clipping restrictions
	pDC->SelectClipRgn(NULL, RGN_COPY);		
	
	CBrush brTopHalf(GetBkgndColor() != CLR_NONE ? RGB(255, 255, 255) : clrBkgnd);
	CBrush* const pOldBrush = pDC->SelectObject(&brTopHalf);

	CPen penBlack(PS_SOLID, 1, RGB(0, 0, 0));
	CPen* const pOldPen = pDC->SelectObject(&penBlack);		
		
	if(GetBkgndColor() == CLR_NONE)
	{
		clrShadeLeft = clrShadeRight = clrBkgnd;
	}

	pDC->FillSolidRect(rcShadeLeft, clrShadeLeft);					
	// To avoid some flickering
	rcShadeRight.bottom-=3;
	pDC->FillSolidRect(rcShadeRight, clrShadeRight);		
	rcShadeRight.bottom+=3;

	pDC->ExcludeClipRect(rcShadeLeft);
	pDC->ExcludeClipRect(rcShadeRight);
	
	CPen penBkgnd(PS_SOLID, 1, clrBkgnd);
	pDC->SelectObject(&penBkgnd);
	
	// draw the top half whitish bar
	nMargin = 1; // To make sure we don't draw on the area of the border lines
	
	ptUpper[0].x -= nMargin;
	ptUpper[0].y -= nMargin;
	ptUpper[1].x += nMargin;
	ptUpper[1].y -= nMargin;
	ptUpper[2].x += nMargin;
	ptUpper[2].y += nMargin;
	//ptUpper[3].x = nShadeTopLeftX;
	ptUpper[3].y += nMargin;
	ptUpper[4].x -= nMargin;
	ptUpper[4].y += nMargin;

	pDC->Polygon(ptUpper, 5);		

	// Remove all clipping restrictions
	pDC->SelectClipRgn(NULL, RGN_COPY);		
	
	// Draw the right hand side outline
	CBrush brHollow;
	brHollow.CreateStockObject(HOLLOW_BRUSH);
	
	CPen penOutLine(PS_SOLID, 1, RGB(180, 180, 180));
	
	pDC->SelectObject(&penOutLine);
	pDC->SelectObject(&brHollow);

	POINT ptOutLine[6];
	ptOutLine[0] = ptInnerCrossSecRightTop;
	ptOutLine[1] = CPoint(rcClient.right, ptInnerCrossSecRightTop.y);
	ptOutLine[2] = CPoint(rcClient.right, rcShadeRight.top + 2);
	ptOutLine[3] = CPoint(nShadeBottomRightX, ptInnerCrossSecLeftBottom.y);	
	ptOutLine[4] = ptInnerCrossSecLeftBottom;
	ptOutLine[5] = ptInnerCrossSecLeftTop;
	
	pDC->Polygon(ptOutLine, 6);

	// Complete the small box at the right edge
	CPen penWhite(PS_SOLID, 1, RGB(225, 225, 225));	
	pDC->SelectObject(&penWhite);
	
	pDC->MoveTo(CPoint(ptOutLine[3].x, ptOutLine[3].y - 1));
	pDC->LineTo(CPoint(ptOutLine[3].x, rcShadeRight.top - 2));
	pDC->LineTo(ptOutLine[1]);

	// Paint some not yet painted area on the space below the right hand side bar
	CPen penShadeRight(PS_SOLID, 1, clrShadeRight);	
	pDC->SelectObject(&penShadeRight);
	
	pDC->MoveTo(CPoint(rcClient.right - 1, rcShadeRight.bottom - 1));
	pDC->LineTo(CPoint(rcShadeRight.left, rcShadeRight.bottom - 1));
	pDC->MoveTo(CPoint(rcClient.right - 1, rcShadeRight.bottom - 2));
	pDC->LineTo(CPoint(rcShadeRight.left, rcShadeRight.bottom - 2));
	pDC->MoveTo(CPoint(rcClient.right - 1, rcShadeRight.bottom - 3));
	pDC->LineTo(CPoint(nShadeBottomRightX, rcShadeRight.bottom - 3));
	
	// Construct and draw the bar on the left hand side of the crossection thumb
	pDC->SelectObject(&penBlack);

	POINT ptLeftBar[5];
	ptLeftBar[0].x = ptCrossSecLeftTop.x;
	ptLeftBar[0].y = ptInnerCrossSecLeftBottom.y;
	ptLeftBar[1].x = rcClient.left;
	ptLeftBar[1].y = ptInnerCrossSecLeftBottom.y;
	ptLeftBar[2].x = rcClient.left;
	ptLeftBar[2].y = ptInnerCrossSecLeftTop.y;
	ptLeftBar[3].x = nShadeTopLeftX;
	ptLeftBar[3].y = ptInnerCrossSecRightTop.y;
	ptLeftBar[4].x = ptCrossSecLeftTop.x;
	ptLeftBar[4].y = ptInnerCrossSecRightTop.y;	

	pDC->Polygon(ptLeftBar, 5);	
	
	// Draw the inner cross section outer rect with two different colors	
	CPoint ptLeftBottom = ptCrossSecLeftBottom;
	ptLeftBottom.y++;
	CPoint ptRightBottom = ptCrossSecRightBottom;	
	ptRightBottom.y++;	
	
	CPen penInnerCrossSection1(PS_SOLID, 1, RGB(190, 190, 190));	
	pDC->SelectObject(&penInnerCrossSection1);

	pDC->MoveTo(ptInnerCrossSecLeftTop);
	pDC->LineTo(ptInnerCrossSecRightTop);

	CPen penInnerCrossSection2(PS_SOLID, 1, RGB(80, 80, 80));
	pDC->SelectObject(&penInnerCrossSection2);

	pDC->LineTo(ptInnerCrossSecRightBottom);
	pDC->LineTo(ptInnerCrossSecLeftBottom);

	pDC->SelectObject(&penInnerCrossSection1);

	pDC->LineTo(ptInnerCrossSecLeftTop);	
	
	pDC->MoveTo(ptInnerCrossSecRightTop);
	pDC->LineTo(ptCrossSecLeftTop);

	pDC->MoveTo(ptInnerCrossSecLeftBottom);
	pDC->LineTo(CPoint(ptCrossSecLeftBottom.x, ptInnerCrossSecLeftBottom.y));
	
	// Draw the main crossection thumb
	int nOldRop2 = pDC->SetROP2(R2_MASKPEN);

	POINT ptCrossSection[5];	
    ptCrossSection[0] = ptCrossSecLeftTop;
	ptCrossSection[1] = ptCrossSecRightTop;
	ptCrossSection[2] = ptRightBottom;
	ptCrossSection[3] = ptLeftBottom;
	ptCrossSection[4] = ptCrossSecLeftTop;		
	
	CPen penCrossSection(PS_SOLID, 1, RGB(10, 97, 179));	
	
	COLORREF clrCrossSectionBkgnd = RGB(221, 237, 245);
	CBrush brCrossSection(clrCrossSectionBkgnd);
	
	pDC->SelectObject(&penCrossSection);	
	pDC->SelectObject(&brCrossSection);
	
	pDC->Polygon(ptCrossSection, 5);

	// Draw another inner polygon to give a darkish shading
	ptCrossSection[1].x = ptInnerCrossSecRightTop.x;
	ptCrossSection[1].y = ptCrossSecLeftTop.y;
	ptCrossSection[2].x = ptInnerCrossSecRightTop.x;
	ptCrossSection[2].y = ptInnerCrossSecRightBottom.y;
	ptCrossSection[3].x = ptInnerCrossSecLeftBottom.x;
	ptCrossSection[3].y = ptInnerCrossSecLeftBottom.y;
	ptCrossSection[4].x = ptCrossSecLeftTop.x;
	ptCrossSection[4].y = ptCrossSecRightBottom.y;

	CPen penNull;
	penNull.CreateStockObject(NULL_PEN);
	CBrush brCrossSectionInner(RGB(200, 220, 255));	
	
	pDC->SelectObject(&penNull);	
	pDC->SelectObject(&brCrossSectionInner);	

	pDC->Polygon(ptCrossSection, 5);

	pDC->SetROP2(nOldRop2);

	pDC->SelectObject(&penCrossSection);	
	pDC->SelectObject(&brCrossSection);
	
	pDC->MoveTo(ptCrossSecLeftTop);
	pDC->LineTo(ptCrossSecLeftBottom);	
	
	pDC->SelectObject(pOldBrush);

	pDC->SelectObject(pOldPen);

}