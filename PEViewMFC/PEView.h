#pragma once
#include "windows.h"
#include "PEViewMFCDlg.h"
class pe
{
public:
	pe(CString FilePath);
	~pe();
	//��ʾdosͷ
	void showDoSHeader(CPEViewMFCDlg *dlg);
	//��ʾ�ļ�ͷ
	void showNtFileHeader(CPEViewMFCDlg* dlg);
	//��ʾ��ѡͷ
	void showOptionaHeader(CPEViewMFCDlg* dlg);
	//��ʾDirectoryTable
	void showImportDirectoryTable(CPEViewMFCDlg* dlg);
	//��ʾIAT
	void showImportAddressTable(CPEViewMFCDlg* dlg);
	//��ʾINT 
	void showImportNameTable(CPEViewMFCDlg* dlg);
	//��ʾ������
	void showExportDirectory(CPEViewMFCDlg* dlg);
	//��ʾ�ض�λ��
	void showBaeRelocationTable(CPEViewMFCDlg* dlg);

	//�����ļ�ƫ��
	int zzRvaToRaw(int Rva);
	//dosͷ�ĵ�ַ
	BYTE* g_DosAddress;
	//�ļ�ͷ�ĵ�ַ
	BYTE* g_NtFileAddress;
	//��ѡͷ�ĵ�ַ
	BYTE* g_OptionalAddress;
	//�ļ���С
	int g_FileSize;
};

