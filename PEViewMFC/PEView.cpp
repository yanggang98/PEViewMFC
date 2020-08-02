#include "pch.h"
#include "PEView.h"
#include <fstream>
#include "PEViewMFCDlg.h"
#include <thread>
using namespace std;


DWORD FileSize(CString Filepath)
{

    //�����ļ�
    HANDLE hFile = CreateFile(Filepath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    //�ж��ļ��Ƿ񴴽�ʧ�ܣ�ʧ���򷵻�
    if (INVALID_HANDLE_VALUE == hFile)
    {
        return 0;
    }
    DWORD dwFileSize = 0;
    dwFileSize = GetFileSize(hFile, NULL);
    //�ر��ļ����
    CloseHandle(hFile);
    //�����ļ���С
    return dwFileSize;
}


//pe���캯�������ļ���Ϣ���뻺����
pe::pe(CString FilePath)
{
    
    //�����ļ���С
    int size = FileSize(FilePath);

    //����ļ���СΪ0���ļ�·�����󴴽�ʧ��
    if(size <=0)
    {
        g_FileSize = 0;
        g_DosAddress = NULL;
        g_NtFileAddress = NULL;
        g_OptionalAddress = NULL;
    }
    else 
    {
        //�����ļ���С�Ļ�����
        PBYTE l_buffer = (PBYTE)malloc(size);
        ifstream l_FileIfstream;
        //����
        l_FileIfstream.open(FilePath, ios::binary);
        l_FileIfstream.seekg(0);
        //���ļ����뻺����
        l_FileIfstream.read((char*)l_buffer, size);
        //�ر���
        l_FileIfstream.close();
        //��ȫ�ֱ�����ֵ
        g_DosAddress = l_buffer;
        g_FileSize = size;
        PIMAGE_DOS_HEADER l_DosAddress = (PIMAGE_DOS_HEADER)g_DosAddress;
        g_NtFileAddress = g_DosAddress + l_DosAddress->e_lfanew + 0x4;
        g_OptionalAddress = g_DosAddress + l_DosAddress->e_lfanew + 0x18;

    }

}


void pe::showDoSHeader(CPEViewMFCDlg *dlg)
{


    //��ȡDOSͷ�ṹ��
    PIMAGE_DOS_HEADER l_dosHeader = (PIMAGE_DOS_HEADER)g_DosAddress;
    //��ӡ��Ϣ
    CString strText;

    CRect rect;
    //�����Ϣ
    dlg->mlist.DeleteAllItems();
    int a = dlg->mlist.GetHeaderCtrl()->GetItemCount();
    for (int i = 0; i < a; i++)
    {
        dlg->mlist.DeleteColumn(0);
    }
    //���ñ�ͷ��Ϣ
    dlg->mlist.GetClientRect(&rect);
    dlg->mlist.InsertColumn(0, L"����", LVCFMT_CENTER, rect.Width() / 3, -1);
    dlg->mlist.InsertColumn(1, L"ֵ", LVCFMT_CENTER, rect.Width() / 3, -1);

     strText.Format(TEXT("%08X"), l_dosHeader->e_magic);
     dlg->mlist.InsertItem(0,_T("Signature"));
     dlg->mlist.SetItemText(0, 1, strText);

     strText.Format(TEXT("%08X"), l_dosHeader->e_lfanew);
     dlg->mlist.InsertItem(1, _T("offset to New Exe Header"));
     dlg->mlist.SetItemText(1, 1, strText);
}


void pe::showNtFileHeader(CPEViewMFCDlg* dlg)
{
    //��ȡFIleͷ�ṹ��
    PIMAGE_FILE_HEADER l_fileHeader = (PIMAGE_FILE_HEADER)g_NtFileAddress;
    //��ӡ��Ϣ
    CString strText;

    CRect rect;
    //�����Ϣ
    dlg->mlist.DeleteAllItems();
    int a=dlg->mlist.GetHeaderCtrl()->GetItemCount();
    for (int i = 0; i < a; i++)
    {
        dlg->mlist.DeleteColumn(0);
    }
    //���ñ�ͷ��Ϣ
    dlg->mlist.GetClientRect(&rect);
    dlg->mlist.InsertColumn(0, L"����", LVCFMT_CENTER, rect.Width() / 3, -1);
    dlg->mlist.InsertColumn(1, L"ֵ", LVCFMT_CENTER, rect.Width() / 3, -1);
    //Machine
    strText.Format(TEXT("%08X"), l_fileHeader->Machine);
    dlg->mlist.InsertItem(0, _T("Machine"));
    dlg->mlist.SetItemText(0, 1, strText);
    //NumberOfSections
    strText.Format(TEXT("%08X"), l_fileHeader->NumberOfSections);
    dlg->mlist.InsertItem(1, _T("NumberOfSections"));
    dlg->mlist.SetItemText(1, 1, strText);
    //TimeDateStamp
    strText.Format(TEXT("%08X"), l_fileHeader->TimeDateStamp);
    dlg->mlist.InsertItem(2, _T("TimeDateStamp"));
    dlg->mlist.SetItemText(2, 1, strText);
    //PointerToSymbolTable
    strText.Format(TEXT("%08X"), l_fileHeader->PointerToSymbolTable);
    dlg->mlist.InsertItem(3, _T("PointerToSymbolTable"));
    dlg->mlist.SetItemText(3, 1, strText);
    //NumberOfSymbols
    strText.Format(TEXT("%08X"), l_fileHeader->NumberOfSymbols);
    dlg->mlist.InsertItem(4, _T("NumberOfSymbols"));
    dlg->mlist.SetItemText(4, 1, strText);
    //SizeOfOptionalHeader
    strText.Format(TEXT("%08X"), l_fileHeader->SizeOfOptionalHeader);
    dlg->mlist.InsertItem(5, _T("SizeOfOptionalHeader"));
    dlg->mlist.SetItemText(5, 1, strText);
    //Characteristics
    strText.Format(TEXT("%08X"), l_fileHeader->Characteristics);
    dlg->mlist.InsertItem(6, _T("Characteristics"));
    dlg->mlist.SetItemText(6, 1, strText);
}


void pe::showOptionaHeader(CPEViewMFCDlg* dlg)
{
    //��ȡ��ѡͷ�ṹ��
    PIMAGE_OPTIONAL_HEADER l_OptionalHeader = (PIMAGE_OPTIONAL_HEADER)g_OptionalAddress;
    //��ӡ��Ϣ
    CString strText;

    CRect rect;
    //�����Ϣ
    dlg->mlist.DeleteAllItems();
    int a = dlg->mlist.GetHeaderCtrl()->GetItemCount();
    for (int i = 0; i < a; i++)
    {
        dlg->mlist.DeleteColumn(0);
    }
    //���ñ�ͷ��Ϣ
    dlg->mlist.GetClientRect(&rect);
    dlg->mlist.InsertColumn(0, L"����", LVCFMT_CENTER, rect.Width() / 3, -1);
    dlg->mlist.InsertColumn(1, L"ֵ", LVCFMT_CENTER, rect.Width() / 3, -1);
    //Magic
    strText.Format(TEXT("%08X"), l_OptionalHeader->Magic);
    dlg->mlist.InsertItem(0, _T("Magic"));
    dlg->mlist.SetItemText(0, 1, strText);
    //SizeOfCode
    strText.Format(TEXT("%08X"), l_OptionalHeader->SizeOfCode);
    dlg->mlist.InsertItem(1, _T("SizeOfCode"));
    dlg->mlist.SetItemText(1, 1, strText);
    //AddressOfEntryPoint
    strText.Format(TEXT("%08X"), l_OptionalHeader->AddressOfEntryPoint);
    dlg->mlist.InsertItem(2, _T("AddressOfEntryPoint"));
    dlg->mlist.SetItemText(2, 1, strText);
    //ImageBase
    strText.Format(TEXT("%08X"), l_OptionalHeader->ImageBase);
    dlg->mlist.InsertItem(3, _T("ImageBase"));
    dlg->mlist.SetItemText(3, 1, strText);
    //SectionAlignment
    strText.Format(TEXT("%08X"), l_OptionalHeader->SectionAlignment);
    dlg->mlist.InsertItem(4, _T("SectionAlignment"));
    dlg->mlist.SetItemText(4, 1, strText);
    //FileAlignment
    strText.Format(TEXT("%08X"), l_OptionalHeader->FileAlignment);
    dlg->mlist.InsertItem(5, _T("FileAlignment"));
    dlg->mlist.SetItemText(5, 1, strText);
    //SizeOfImage
    strText.Format(TEXT("%08X"), l_OptionalHeader->SizeOfImage);
    dlg->mlist.InsertItem(6, _T("SizeOfImage"));
    dlg->mlist.SetItemText(6, 1, strText);
    //SizeOfHeaders
    strText.Format(TEXT("%08X"), l_OptionalHeader->SizeOfHeaders);
    dlg->mlist.InsertItem(7, _T("SizeOfHeaders"));
    dlg->mlist.SetItemText(7, 1, strText);
}



// �ڴ�ƫ�Ƶ��ļ�ƫ�Ƶ�ת��
int pe::zzRvaToRaw(int RVA)
{

    PIMAGE_SECTION_HEADER l_pSectionHeader;//����ͷָ��
    PIMAGE_FILE_HEADER l_fileHeader;//Fileͷָ��
    int l_NumberOfSections = NULL;//����ͷ������

    //��ȡ��һ�������Ľṹ��
    l_pSectionHeader = (PIMAGE_SECTION_HEADER)( g_OptionalAddress + ((PIMAGE_FILE_HEADER)g_NtFileAddress)->SizeOfOptionalHeader);
    //��ȡFileͷ�ṹ��
    l_fileHeader = (PIMAGE_FILE_HEADER)g_NtFileAddress;
    //��ȡ����������
    l_NumberOfSections = l_fileHeader->NumberOfSections;

    //�������н���
    for (int i = 0; i < l_NumberOfSections; i++)
    {
        //�����������ʼλ�õ��ڴ�ƫ�� ���� RVA ,˵��RVA ����һ������
        if (RVA < l_pSectionHeader[i].VirtualAddress)
        {
            //�����ļ�ƫ�ƣ�RVA��ȥ���ڽ�����ʼλ�õ��ڴ�ƫ��,�������ڽ������ļ�ƫ��
            int Result = RVA - l_pSectionHeader[i - 1].VirtualAddress + l_pSectionHeader[i - 1].PointerToRawData;
            return Result;
        }
    }

    //��RVA ���� ���һ����������ʼ���ڴ�ƫ�� 
    int Result = RVA - l_pSectionHeader[l_NumberOfSections - 1].VirtualAddress + l_pSectionHeader[l_NumberOfSections - 1].PointerToRawData;
    return  Result;
}

void pe::showImportDirectoryTable(CPEViewMFCDlg* dlg) 
{
    //��ȡ��ѡͷ�ṹ��
    PIMAGE_OPTIONAL_HEADER l_OptionalHeader = (PIMAGE_OPTIONAL_HEADER)g_OptionalAddress;
    PIMAGE_DATA_DIRECTORY  l_PDateDirectory;
    PIMAGE_IMPORT_DESCRIPTOR l_PImportDescriptor;
    CString strText;
    l_PDateDirectory = l_OptionalHeader->DataDirectory;
    CRect rect;
    //�����Ϣ
    dlg->mlist.DeleteAllItems();
    int a = dlg->mlist.GetHeaderCtrl()->GetItemCount();
    for (int i = 0; i < a; i++)
    {
        dlg->mlist.DeleteColumn(0);
    }
    dlg->mlist.GetClientRect(&rect);
    dlg->mlist.InsertColumn(0, L"OriginalFirstThunk", LVCFMT_CENTER, rect.Width() / 3, -1);
    dlg->mlist.InsertColumn(1, L"FirstThunk", LVCFMT_CENTER, rect.Width() / 3, -1);
    dlg->mlist.InsertColumn(2, L"Name", LVCFMT_CENTER, rect.Width() / 3, -1);

    //��ȡIID���ļ�ƫ��
    int Raw = zzRvaToRaw(l_PDateDirectory[1].VirtualAddress);
    //��ȡIID�����һ���ṹ��ָ��
    l_PImportDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)(g_DosAddress + Raw);
    strText.Format(TEXT("-------------------IMAGE_IMPORT_DESCRIPTOR--------------\r\n"));
    //�������е�IID�ṹ�壬���һ���ṹ��Ϊ0
    int i = 0;
    while (l_PImportDescriptor->Name != 0x0)
    {
        //��ȡ�ṹ���Աname���ļ�ƫ��
        int NameIndex = zzRvaToRaw(l_PImportDescriptor->Name);
        PCHAR l_pName;
        //��ȡname��ʵ�ʵ�ַ
        l_pName = (PCHAR)(g_DosAddress+NameIndex);
        //��ascii��ΪUnicode
        int unicodeLen = MultiByteToWideChar(CP_ACP, 0, l_pName, -1, nullptr, 0);
        wchar_t* pUnicode = (wchar_t*)malloc(sizeof(wchar_t) * unicodeLen);
        MultiByteToWideChar(CP_ACP, 0, l_pName, -1, pUnicode, unicodeLen);
        //��ӡ��Ϣ
        //OriginalFirstThunk
        strText.Format(TEXT("%08X"), l_PImportDescriptor->OriginalFirstThunk);
        dlg->mlist.InsertItem(i, strText);
        //FirstThunk
        strText.Format(TEXT("%s"), l_PImportDescriptor->FirstThunk);
        dlg->mlist.SetItemText(i,1, strText);
        //name
        strText.Format(TEXT("%08X"), pUnicode);
        dlg->mlist.SetItemText(i, 2, strText);

        l_PImportDescriptor++;
        i++;
    }
   
}


void pe::showImportAddressTable(CPEViewMFCDlg* dlg)
{
    PIMAGE_OPTIONAL_HEADER l_OptionalHeader = (PIMAGE_OPTIONAL_HEADER)g_OptionalAddress;
    PIMAGE_DATA_DIRECTORY  l_PDateDirectory;
    PIMAGE_IMPORT_DESCRIPTOR l_PImportDescriptor;
    CString strText;
    l_PDateDirectory = l_OptionalHeader->DataDirectory;

    // ��ȡIID���ļ�ƫ��
    int Raw = zzRvaToRaw(l_PDateDirectory[1].VirtualAddress);
    //��ȡIID�����һ���ṹ��ָ��
    l_PImportDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)(g_DosAddress + Raw);
    strText.Format(TEXT("-------------------IMPORT_ADDRESS_TABLE--------------\r\n"));
    strText.Format(TEXT("%s RVA           DATE             ����\r\n"),strText);

    CRect rect;
    //�����Ϣ
    dlg->mlist.DeleteAllItems();
    int a = dlg->mlist.GetHeaderCtrl()->GetItemCount();
    for (int i = 0; i < a; i++)
    {
        dlg->mlist.DeleteColumn(0);
    }
    dlg->mlist.GetClientRect(&rect);
    dlg->mlist.InsertColumn(0, L"RVA", LVCFMT_CENTER, rect.Width() / 3, -1);
    dlg->mlist.InsertColumn(1, L"DATE", LVCFMT_CENTER, rect.Width() / 3, -1);
    dlg->mlist.InsertColumn(2, L"����/���", LVCFMT_CENTER, rect.Width() / 3, -1);

    //�������е�IID�ṹ�壬���һ���ṹ��Ϊ0
    int i = 0;
    while (l_PImportDescriptor->Name != 0x0)
    {
        //��ȡ��ַ�������ڴ�ƫ��
        int RVA_1 = l_PImportDescriptor->FirstThunk;
        //��ȡ��ַ�������ļ�ƫ��
        int RAW_1 = zzRvaToRaw(RVA_1);//��RVAת��ΪRAW
        //��ַ�����ṹ���ָ��
        PIMAGE_THUNK_DATA l_pThunkData =(PIMAGE_THUNK_DATA)(g_DosAddress + RAW_1);

        //�������� IMAGE_THUNK_DAT
        while (*(PDWORD)l_pThunkData!=0x0)
        {
            //�ж�����ŵ��뻹�����Ƶ���
            if (((*(PDWORD)l_pThunkData) >> 31) == 1) 
            {
                //��ŵ��룬��ӡ��Ϣ

                strText.Format(TEXT("%08X"), RVA_1);
                dlg->mlist.InsertItem(i, strText);
                strText.Format(TEXT("%08X"), (*(PDWORD)l_pThunkData));
                dlg->mlist.SetItemText(i, 1, strText);
                strText.Format(TEXT("%08X"), (*(PDWORD)l_pThunkData) & 0x7fffffff);
                dlg->mlist.SetItemText(i, 2, strText);
                i++;
                RVA_1 += 4;
                l_pThunkData++;
                continue;
            }
            //���Ƶ���
            PIMAGE_IMPORT_BY_NAME l_pName = (PIMAGE_IMPORT_BY_NAME)(g_DosAddress + zzRvaToRaw(l_pThunkData->u1.AddressOfData));   
            //asciiתUnicode
            int unicodeLen = MultiByteToWideChar(CP_ACP, 0, l_pName->Name, -1, nullptr, 0);
            wchar_t* pUnicode = (wchar_t*)malloc(sizeof(wchar_t) * unicodeLen);
            MultiByteToWideChar(CP_ACP, 0, l_pName->Name, -1, pUnicode, unicodeLen);
            //��ӡ��Ϣ
            //RVA
            strText.Format(TEXT("%08X"), RVA_1);
            dlg->mlist.InsertItem(i, strText);
            //DATE
            strText.Format(TEXT("%08X"), (*(PDWORD)l_pThunkData));
            dlg->mlist.SetItemText(i, 1, strText);
            //��������
            strText.Format(TEXT("%s"), pUnicode);
            dlg->mlist.SetItemText(i, 2, strText);
            i++;

            RVA_1 += 4;
            l_pThunkData++;
        }
        
        //asciiתUnicode
        int Name = zzRvaToRaw(l_PImportDescriptor->Name);
        PCHAR l_Name;//Ĭ�����Ƴ��Ȳ�����50
        l_Name = (PCHAR)(g_DosAddress + Name);
        int unicodeLen = MultiByteToWideChar(CP_ACP, 0, l_Name, -1, nullptr, 0);
        wchar_t* Unicode = (wchar_t*)malloc(sizeof(wchar_t) * unicodeLen);
        MultiByteToWideChar(CP_ACP, 0, l_Name, -1, Unicode, unicodeLen);
        
        //��ӡ��Ϣ
        //RVA
        strText.Format(TEXT("%08X"), RVA_1);
        dlg->mlist.InsertItem(i, strText);
        strText.Format(TEXT("%08X"), 0x00000000);
        dlg->mlist.SetItemText(i, 1, strText);
        //dll ����
        strText.Format(TEXT("%s"), Unicode);
        dlg->mlist.SetItemText(i, 2, strText);
        i++;
        dlg->mlist.InsertItem(i, L"");
        i++;
        std::free(Unicode);
        l_PImportDescriptor++;

    }

}

void pe::showExportDirectory(CPEViewMFCDlg* dlg) 
{
    PIMAGE_OPTIONAL_HEADER l_OptionalHeader = (PIMAGE_OPTIONAL_HEADER)g_OptionalAddress;
    PIMAGE_DATA_DIRECTORY  l_PDateDirectory;
    PIMAGE_EXPORT_DIRECTORY l_pExportDirectory;
    CString strText;
    l_PDateDirectory = l_OptionalHeader->DataDirectory;

    //�ж��Ƿ���ڵ�����
    if (l_PDateDirectory[0].VirtualAddress == 0x0) 
    {
        dlg->MessageBox(L"û�е�����");
        return ;
    }
    //��ȡ��������ļ�ƫ��
    int Raw = zzRvaToRaw(l_PDateDirectory[0].VirtualAddress);
    //��ȡ������Ľṹ��ָ��
    l_pExportDirectory = (PIMAGE_EXPORT_DIRECTORY)(g_DosAddress + Raw);

    //���������׵�ַ
    int l_nameRva = l_pExportDirectory->AddressOfNames;
    DWORD* l_pNameAddress = (DWORD*)(g_DosAddress + zzRvaToRaw(l_nameRva));
    //��ַ�����׵�ַ
    int l_funRva = l_pExportDirectory->AddressOfFunctions;
    DWORD* l_pfunAddress = (DWORD*)(g_DosAddress + zzRvaToRaw(l_funRva));
    //���к������׵�ַ
    int l_pNameOrdinals = l_pExportDirectory->AddressOfNameOrdinals;
    WORD* l_NameOrdinalsAddress = (WORD*)(g_DosAddress + zzRvaToRaw(l_pNameOrdinals));
  

    CRect rect;
    //�����Ϣ
    dlg->mlist.DeleteAllItems();
    int a = dlg->mlist.GetHeaderCtrl()->GetItemCount();
    for (int i = 0; i < a; i++)
    {
        dlg->mlist.DeleteColumn(0);
    }
    //���ñ�ͷ
    dlg->mlist.GetClientRect(&rect);
    dlg->mlist.InsertColumn(0, L"���", LVCFMT_CENTER, rect.Width() / 3, -1);
    dlg->mlist.InsertColumn(1, L"RVA", LVCFMT_CENTER, rect.Width() / 3, -1);
    dlg->mlist.InsertColumn(2, L"����", LVCFMT_CENTER, rect.Width() / 3, -1);

    //������ַ����
    int index = 0;
    for (int i = 0; i < l_pExportDirectory->NumberOfFunctions; i++)
    {
        
        bool flag = false;
        //�ж������д�ŵ�ֵ�Ƿ���0
        if (l_pfunAddress[i] == 0x0) 
        {
            continue;
        }

        //�������к�����
        for (int j = 0; j < l_pExportDirectory->NumberOfNames; j++)
        {
            //�Ƚ����к������е�ֵ�͵�ַ������±��Ƿ����
            if (l_NameOrdinalsAddress[j] == i)
            {

                PCHAR l_Name;
                l_Name = (PCHAR)(g_DosAddress + zzRvaToRaw(l_pNameAddress[j]));
                int unicodeLen = MultiByteToWideChar(CP_ACP, 0, l_Name, -1, nullptr, 0);
                wchar_t* Unicode = (wchar_t*)malloc(sizeof(wchar_t) * unicodeLen);
                MultiByteToWideChar(CP_ACP, 0, l_Name, -1, Unicode, unicodeLen);
                //��ӡ��Ϣ
                //���
                strText.Format(TEXT("%08X"), i + l_pExportDirectory->Base);
                dlg->mlist.InsertItem(index, strText);
                //RVA
                strText.Format(TEXT("%08X"), l_pfunAddress[i]);
                dlg->mlist.SetItemText(index, 1, strText);
                //����
                strText.Format(TEXT("%s"), Unicode);
                dlg->mlist.SetItemText(index, 2, strText);
                index++;
                std::free(Unicode);
                flag = true;
            }
        }

        //�ж��Ƿ����кŵ��������Ƿ��е�ַ������±�
        if (flag == false) 
        {
         
            //��ӡ��Ϣ
            //���
            strText.Format(TEXT("%08X"), i + l_pExportDirectory->Base);
            dlg->mlist.InsertItem(index, strText);
            //RVA
            strText.Format(TEXT("%08X"), l_pfunAddress[i]);
            dlg->mlist.SetItemText(index, 1, strText);
            //����
            strText.Format(TEXT("%s"), "");
            dlg->mlist.SetItemText(index, 2, strText);
            index++;
        }
        
    }

}

void pe::showImportNameTable(CPEViewMFCDlg* dlg) 
{
    PIMAGE_OPTIONAL_HEADER l_OptionalHeader = (PIMAGE_OPTIONAL_HEADER)g_OptionalAddress;
    PIMAGE_DATA_DIRECTORY  l_PDateDirectory;
    PIMAGE_IMPORT_DESCRIPTOR l_PImportDescriptor;
    CString strText;
    l_PDateDirectory = l_OptionalHeader->DataDirectory;

    // ��ȡIID���ļ�ƫ��
    int Raw = zzRvaToRaw(l_PDateDirectory[1].VirtualAddress);
    //��ȡIID�����һ���ṹ��ָ��
    l_PImportDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)(g_DosAddress + Raw);
    strText.Format(TEXT("-------------------IMPORT_ADDRESS_TABLE--------------\r\n"));
    strText.Format(TEXT("%s RVA           DATE             ����\r\n"), strText);

    CRect rect;
    //�����Ϣ
    dlg->mlist.DeleteAllItems();
    int a = dlg->mlist.GetHeaderCtrl()->GetItemCount();
    for (int i = 0; i < a; i++)
    {
        dlg->mlist.DeleteColumn(0);
    }
    dlg->mlist.GetClientRect(&rect);
    dlg->mlist.InsertColumn(0, L"RVA", LVCFMT_CENTER, rect.Width() / 3, -1);
    dlg->mlist.InsertColumn(1, L"DATE", LVCFMT_CENTER, rect.Width() / 3, -1);
    dlg->mlist.InsertColumn(2, L"����/���", LVCFMT_CENTER, rect.Width() / 3, -1);

    //�������е�IID�ṹ�壬���һ���ṹ��Ϊ0
    int i = 0;
    while (l_PImportDescriptor->Name != 0x0)
    {
        //��ȡ��ַ�������ڴ�ƫ��
        int RVA_1 = l_PImportDescriptor->OriginalFirstThunk;
        //��ȡ��ַ�������ļ�ƫ��
        int RAW_1 = zzRvaToRaw(RVA_1);//��RVAת��ΪRAW
        //��ַ�����ṹ���ָ��
        PIMAGE_THUNK_DATA l_pThunkData = (PIMAGE_THUNK_DATA)(g_DosAddress + RAW_1);

        //�������� IMAGE_THUNK_DAT
        while (*(PDWORD)l_pThunkData != 0x0)
        {
            //�ж�����ŵ��뻹�����Ƶ���
            if (((*(PDWORD)l_pThunkData) >> 31) == 1)
            {
                //��ŵ��룬��ӡ��Ϣ

                strText.Format(TEXT("%08X"), RVA_1);
                dlg->mlist.InsertItem(i, strText);
                strText.Format(TEXT("%08X"), (*(PDWORD)l_pThunkData));
                dlg->mlist.SetItemText(i, 1, strText);
                strText.Format(TEXT("%08X"), (*(PDWORD)l_pThunkData) & 0x7fffffff);
                dlg->mlist.SetItemText(i, 2, strText);
                i++;
                RVA_1 += 4;
                l_pThunkData++;
                continue;
            }
            //���Ƶ���
            PIMAGE_IMPORT_BY_NAME l_pName = (PIMAGE_IMPORT_BY_NAME)(g_DosAddress + zzRvaToRaw(l_pThunkData->u1.AddressOfData));
            //asciiתUnicode
            int unicodeLen = MultiByteToWideChar(CP_ACP, 0, l_pName->Name, -1, nullptr, 0);
            wchar_t* pUnicode = (wchar_t*)malloc(sizeof(wchar_t) * unicodeLen);
            MultiByteToWideChar(CP_ACP, 0, l_pName->Name, -1, pUnicode, unicodeLen);
            //��ӡ��Ϣ
            //RVA
            strText.Format(TEXT("%08X"), RVA_1);
            dlg->mlist.InsertItem(i, strText);
            //DATE
            strText.Format(TEXT("%08X"), (*(PDWORD)l_pThunkData));
            dlg->mlist.SetItemText(i, 1, strText);
            //��������
            strText.Format(TEXT("%s"), pUnicode);
            dlg->mlist.SetItemText(i, 2, strText);
            i++;

            RVA_1 += 4;
            l_pThunkData++;
        }

        //asciiתUnicode
        int Name = zzRvaToRaw(l_PImportDescriptor->Name);
        PCHAR l_Name;
        l_Name = (PCHAR)(g_DosAddress + Name);
        int unicodeLen = MultiByteToWideChar(CP_ACP, 0, l_Name, -1, nullptr, 0);
        wchar_t* Unicode = (wchar_t*)malloc(sizeof(wchar_t) * unicodeLen);
        MultiByteToWideChar(CP_ACP, 0, l_Name, -1, Unicode, unicodeLen);

        //��ӡ��Ϣ
        //RVA
        strText.Format(TEXT("%08X"), RVA_1);
        dlg->mlist.InsertItem(i, strText);
        strText.Format(TEXT("%08X"), 0x00000000);
        dlg->mlist.SetItemText(i, 1, strText);
        //dll ����
        strText.Format(TEXT("%s"), Unicode);
        dlg->mlist.SetItemText(i, 2, strText);
        i++;
        dlg->mlist.InsertItem(i, L"");
        i++;
        std::free(Unicode);
        l_PImportDescriptor++;

    }

}

void pe::showBaeRelocationTable(CPEViewMFCDlg* dlg)
{
    PIMAGE_OPTIONAL_HEADER l_OptionalHeader = (PIMAGE_OPTIONAL_HEADER)g_OptionalAddress;
    PIMAGE_DATA_DIRECTORY  l_PDateDirectory;
    PIMAGE_EXPORT_DIRECTORY l_pExportDirectory;
    CString strText;
    PIMAGE_BASE_RELOCATION  l_pBaeRelocation;

    l_PDateDirectory = l_OptionalHeader->DataDirectory;
    int l_BaeRelocation = l_PDateDirectory[5].VirtualAddress;
    int l_BaeRelocationSize = l_PDateDirectory[5].Size;

    //�ж��Ƿ�����ض�λ��
    if (l_BaeRelocation == 0) 
    {
        dlg->MessageBox(L"û���ض�λ��");
        return;
    }
    l_pBaeRelocation =(PIMAGE_BASE_RELOCATION) (g_DosAddress+zzRvaToRaw(l_PDateDirectory[5].VirtualAddress));

    CRect rect;
    //�����Ϣ
    dlg->mlist.DeleteAllItems();
    int a = dlg->mlist.GetHeaderCtrl()->GetItemCount();
    for (int i = 0; i < a; i++)
    {
        dlg->mlist.DeleteColumn(0);
    }
    //���ñ�ͷ��Ϣ
    dlg->mlist.GetClientRect(&rect);
    dlg->mlist.InsertColumn(0, L"RVA", LVCFMT_CENTER, rect.Width() / 3, -1);
    dlg->mlist.InsertColumn(1, L"FOA", LVCFMT_CENTER, rect.Width() / 3, -1);
    dlg->mlist.InsertColumn(2, L"����", LVCFMT_CENTER, rect.Width() / 3, -1);

    int l_sum = 0;
    int index = 0;
    do
    {
        l_BaeRelocation = l_BaeRelocation + 8;

        short int* l_pValue = (short int*)((PBYTE)l_pBaeRelocation + 8);

        while (l_pValue < (short int*)((PBYTE)l_pBaeRelocation + (l_pBaeRelocation->SizeOfBlock))) 
        {           
            //RVA
            strText.Format(TEXT("%08X"), (*l_pValue & 0xfff) + l_pBaeRelocation->VirtualAddress);
            dlg->mlist.InsertItem(index, strText);
            //FOA
            strText.Format(TEXT("%08X"), zzRvaToRaw((*l_pValue & 0xFFF) + l_pBaeRelocation->VirtualAddress));
            dlg->mlist.SetItemText(index, 1, strText);
            //����
            //�ض�λ����
            switch ((*l_pValue & 0xf000) >> 12)
            {
            case 0:
                strText.Format(TEXT("IMAGE_REL_BASED_ABSOLUTE"));
                break;
            case 3:
                strText.Format(TEXT("IMAGE_REL_BASED_HIGHLOW"));
                break;
            default:
                strText.Format(TEXT("IMAGE_REL_BASED_DIR64"));
                break;
            }
            
            dlg->mlist.SetItemText(index, 2, strText);

            index++;
            l_BaeRelocation = l_BaeRelocation + 2;
            l_pValue++;
        }
        l_sum = l_sum + l_pBaeRelocation->SizeOfBlock;
        l_pBaeRelocation = (PIMAGE_BASE_RELOCATION)((PBYTE)l_pBaeRelocation + (l_pBaeRelocation->SizeOfBlock));
    
    } while (l_sum < l_BaeRelocationSize);
}


pe::~pe()
{
    g_DosAddress = NULL;
    g_FileSize = NULL;
    g_NtFileAddress = NULL;
    g_OptionalAddress = NULL;
}
