#include "pch.h"
#include "myCEdit.h"
BEGIN_MESSAGE_MAP(myCEdit, CEdit)
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


void myCEdit::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CEdit::OnLButtonDblClk(nFlags, point);
	CString strFile = _T("");
	CFileDialog  dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("EXE File (*.exe)|*.exe|Dynamic-link library (*.dll)|*.dll|Any Files (*.*)|*.*||"), NULL);

	if (dlgFile.DoModal())
	{
		strFile = dlgFile.GetPathName();
		SetWindowTextW(strFile);
	}

}
