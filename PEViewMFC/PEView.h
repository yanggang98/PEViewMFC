#pragma once
#include "windows.h"
#include "PEViewMFCDlg.h"
class pe
{
public:
	pe(CString FilePath);
	~pe();
	//显示dos头
	void showDoSHeader(CPEViewMFCDlg *dlg);
	//显示文件头
	void showNtFileHeader(CPEViewMFCDlg* dlg);
	//显示可选头
	void showOptionaHeader(CPEViewMFCDlg* dlg);
	//显示DirectoryTable
	void showImportDirectoryTable(CPEViewMFCDlg* dlg);
	//显示IAT
	void showImportAddressTable(CPEViewMFCDlg* dlg);
	//显示INT 
	void showImportNameTable(CPEViewMFCDlg* dlg);
	//显示导出表
	void showExportDirectory(CPEViewMFCDlg* dlg);
	//显示重定位表
	void showBaeRelocationTable(CPEViewMFCDlg* dlg);

	//计算文件偏移
	int zzRvaToRaw(int Rva);
	//dos头的地址
	BYTE* g_DosAddress;
	//文件头的地址
	BYTE* g_NtFileAddress;
	//可选头的地址
	BYTE* g_OptionalAddress;
	//文件大小
	int g_FileSize;
};

