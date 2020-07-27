
// PEViewMFCDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "PEViewMFC.h"
#include "PEViewMFCDlg.h"
#include "afxdialogex.h"
#include "PEView.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPEViewMFCDlg 对话框



CPEViewMFCDlg::CPEViewMFCDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PEVIEWMFC_DIALOG, pParent)
{
   // mShowEdit.SetReadOnly(true);
	//mShowEdit.EnableWindow(FALSE);
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);


}

void CPEViewMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, mEdit1);
	DDX_Control(pDX, IDC_EDIT2, mShowEdit);
	DDX_Control(pDX, IDC_COMBO1, mCombo1);
}

BEGIN_MESSAGE_MAP(CPEViewMFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CPEViewMFCDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CPEViewMFCDlg 消息处理程序

BOOL CPEViewMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	//mShowEdit.SetWindowTextW(L"asdsad");
	//UpdateData(FALSE);

	//pe mype("C:\\Windows\\WinSxS\\wow64_microsoft-windows-user32_31bf3856ad364e35_10.0.18362.959_none_2d5e5441335b7c69\\user32.dll");
	//mype.showDoSHeader(this);
	//mype.showNtFileHeader(this);
	//mype.showOptionaHeader(this);
	//mype.showImportDirectoryTable(this);
	//mype.showImportAddressTable(this);
	//mype.showExportDirectory(this);
	//mype.showBaeRelocationTable(this);
	mCombo1.InsertString(0, L"无");
	mCombo1.InsertString(1, L"DOS头");
	mCombo1.InsertString(2, L"FILE头");
	mCombo1.InsertString(3, L"可选头");
	mCombo1.InsertString(4, L"地址导入表");
	mCombo1.InsertString(5, L"地址导出表");
	mCombo1.InsertString(6, L"基址重定位表");
	mCombo1.SetCurSel(0);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPEViewMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPEViewMFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CPEViewMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CPEViewMFCDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	mEdit1.GetWindowText(str);

	//unicode 转Ascii
	int ansiiLen = WideCharToMultiByte(CP_ACP, 0, str, -1, nullptr, 0, nullptr, nullptr);

	char* filePath = (char*)malloc(sizeof(char) * ansiiLen);

	WideCharToMultiByte(CP_ACP, 0, str, -1, filePath, ansiiLen, nullptr, nullptr);
	
	//创建pe对象
	pe l_pe(filePath);

	if (l_pe.g_DosAddress == NULL) 
	{
		mShowEdit.SetWindowTextW(L"文件路径错误");
		UpdateData(FALSE);
		return;
	}

	int index = mCombo1.GetCurSel();

	switch (index)
	{
	case 0:
		mShowEdit.SetWindowTextW(L"请选择打印表");
		UpdateData(FALSE);
		break;	
	case 1:
		l_pe.showDoSHeader(this);
		break;
	case 2:
		l_pe.showNtFileHeader(this);
		break;
	case 3:
		l_pe.showOptionaHeader(this);
		break;
	case 4:
		l_pe.showImportAddressTable(this);
		break;
	case 5:
		l_pe.showExportDirectory(this);
		break;
	case 6:
		l_pe.showBaeRelocationTable(this);
		break;
	default:
		break;
	}
	free(filePath);
}
