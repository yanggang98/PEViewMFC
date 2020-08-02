#pragma once
#include <afxwin.h>
class myCEdit :
	public CEdit
{
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};

