﻿
// PEViewMFCDlg.h: 头文件
//

#pragma once
#include "myCEdit.h"


// CPEViewMFCDlg 对话框
class CPEViewMFCDlg : public CDialogEx
{
// 构造
public:
	CPEViewMFCDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PEVIEWMFC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	myCEdit mEdit1;
	CComboBox mCombo1;
	CFont m_editFont;
	afx_msg void OnBnClickedButton1();
	CListCtrl mlist;
};
