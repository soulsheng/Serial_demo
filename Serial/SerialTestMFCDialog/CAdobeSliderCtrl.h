
#pragma once

// The base class for CAdobeSliderCtrl 
// which can be used as a base class for any custom draw slider control
class CCustomDrawSliderCtrl : public CSliderCtrl
{	
public:
	// constructor
	CCustomDrawSliderCtrl();
	// destructor
	virtual ~CCustomDrawSliderCtrl();

protected:
	// CCustomDrawSliderCtrl methods
	void Draw(CDC *pDC, LPNMCUSTOMDRAW lpcd);

	//
	// CCustomDrawSliderCtrl message handlers
	//
	afx_msg void OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult);
	BOOL OnEraseBkgnd(CDC* pDC);

	DECLARE_MESSAGE_MAP()
	//
	// CCustomDrawSliderCtrl Overridden Methods
	//
	virtual void OnDraw(CDC* pDC, CRect rcClient) = 0;
};


///////////////////////////////////////////////////
// CAdobeSliderCtrl class
/////////////////////////////////////////////////////

class CAdobeSliderCtrl : public CCustomDrawSliderCtrl
{
public:
	// constructor
	CAdobeSliderCtrl();
	// destructor
	virtual ~CAdobeSliderCtrl();

public:
	
	// public methods
	
	inline void SetBkgndColor(COLORREF clrBkgnd)
	{
		m_clrBkgnd = clrBkgnd;
	}

	inline COLORREF GetBkgndColor()
	{
		return m_clrBkgnd;
	}

	// Gets the cross section thumb points for the outer (big) and inner (small) cross sections
	//	ptOuterCrossSec	: Output parameter to receive the outer crosssection points in the following order in the array:
	//							  left-top, right-top, right-bottom, left-bottom
	//	ptInnerCrossSec	: Output parameter to receive the inner crosssection points in the following order in the array:
	//							  left-top, right-top, right-bottom, left-bottom
	//
	virtual BOOL GetCrossSectionPoints(POINT ptOuterCrossSec[], POINT ptInnerCrossSec[]);

protected:
	// overriden methods
	virtual void OnDraw(CDC* pDC, CRect rcClient);

protected:
	// attributes
	COLORREF m_clrBkgnd;

	// Hold the area where drawing is allowed
	CRect m_rcDrawArea;
};