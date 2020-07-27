#include "pch.h"
#include "PEView.h"
#include <fstream>
#include "PEViewMFCDlg.h"
using namespace std;


DWORD FileSize(CHAR* Filepath)
{
    //TCHAR szFileName[MAX_PATH] = TEXT(Filepath);
    //字符串转化 char* to  LPCWSTR
    WCHAR wszClassName[256];
    MultiByteToWideChar(CP_ACP, 0, Filepath, strlen(Filepath) + 1, wszClassName,
        sizeof(wszClassName) / sizeof(wszClassName[0]));

    HANDLE hFile = CreateFile(wszClassName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (INVALID_HANDLE_VALUE == hFile)
    {
        return 0;
    }
    DWORD dwFileSize = 0;
    dwFileSize = GetFileSize(hFile, NULL);
    CloseHandle(hFile);
    return dwFileSize;
}

pe::pe(PCHAR FilePath)
{
    ifstream l_FileIfstream;
    //计算文件大小
    int size = FileSize(FilePath);
    g_FileSize = size;
    if(g_FileSize<=0)
    {
        g_DosAddress = NULL;
        g_NtFileAddress = NULL;
        g_OptionalAddress = NULL;
    }
    else 
    {
        PBYTE l_byte = (PBYTE)malloc(size);
        l_FileIfstream.open(FilePath, ios::binary);
        l_FileIfstream.seekg(0);
        l_FileIfstream.read((char*)l_byte, size);
        l_FileIfstream.close();
        g_DosAddress = l_byte;
        PIMAGE_DOS_HEADER l_DosAddress = (PIMAGE_DOS_HEADER)g_DosAddress;
        g_NtFileAddress = g_DosAddress + l_DosAddress->e_lfanew + 0x4;
        g_OptionalAddress = g_DosAddress + l_DosAddress->e_lfanew + 0x18;

    }



}

void pe::showDoSHeader(CPEViewMFCDlg *dlg)
{
     PIMAGE_DOS_HEADER l_dosHeader = (PIMAGE_DOS_HEADER)g_DosAddress;
     CString strText;
     strText.Format(TEXT("---------------------- IMAGE_DOS_HEADER-----------------\r\n"));
     strText.Format(TEXT("%sSignature:%x\r\n"),strText, l_dosHeader->e_magic);
     strText.Format(TEXT("%soffset to New Exe Header:%x\r\n"), strText,l_dosHeader->e_lfanew);
     dlg->mShowEdit.SetWindowTextW(strText);
     dlg->UpdateData(FALSE);
}


void pe::showNtFileHeader(CPEViewMFCDlg* dlg)
{
    PIMAGE_FILE_HEADER l_fileHeader = (PIMAGE_FILE_HEADER)g_NtFileAddress;
    CString strText;
    strText.Format(TEXT("----------------------IMAGE_FILE_HEADER-----------------\r\n"));
    strText.Format(TEXT("%sMachine:%x\r\n"), strText, l_fileHeader->Machine);
    strText.Format(TEXT("%sNumberOfSections:%x\r\n"), strText, l_fileHeader->NumberOfSections);
    strText.Format(TEXT("%sSizeOfOptionalHeader:%x\r\n"), strText, l_fileHeader->SizeOfOptionalHeader);
    strText.Format(TEXT("%sCharacteristics:%x\r\n"), strText, l_fileHeader->Characteristics);
    dlg->mShowEdit.SetWindowTextW(strText);
    dlg->UpdateData(FALSE);
}


void pe::showOptionaHeader(CPEViewMFCDlg* dlg)
{
    PIMAGE_OPTIONAL_HEADER l_OptionalHeader = (PIMAGE_OPTIONAL_HEADER)g_OptionalAddress;
    CString strText;
    strText.Format(TEXT("----------------------IMAGE_OPTIONAL_HEADER-----------------\r\n"));
    strText.Format(TEXT("%sMagic:%x\r\n"),strText, l_OptionalHeader->Magic);
    strText.Format(TEXT("%sAddressOfEntryPoint:%x\r\n"), strText, l_OptionalHeader->AddressOfEntryPoint);
    strText.Format(TEXT("%sImageBase:%x\r\n"), strText, l_OptionalHeader->ImageBase);
    strText.Format(TEXT("%sSectionAlignment:%x\r\n"), strText, l_OptionalHeader->SectionAlignment);
    strText.Format(TEXT("%sSizeOfImage:%x\r\n"), strText, l_OptionalHeader->SizeOfImage);
    strText.Format(TEXT("%sSizeOfHeaders:%x\r\n"), strText, l_OptionalHeader->SizeOfHeaders);
    strText.Format(TEXT("%sSubsystem:%x\r\n"), strText, l_OptionalHeader->Subsystem);
    strText.Format(TEXT("%sNumberOfRvaAndSizes:%x\r\n"), strText, l_OptionalHeader->NumberOfRvaAndSizes);
    dlg->mShowEdit.SetWindowTextW(strText);
    dlg->UpdateData(FALSE);
}

typedef struct Sections
{
    DWORD RAW_begin;//文件开始位置
    DWORD RVA_begin;//内存开始位置
} *pSections;


// RVA 到RAW
int pe::zzRvaToRaw(int RVA)
{
    PIMAGE_SECTION_HEADER l_pSectionHeader;//节区头指针
    PIMAGE_FILE_HEADER l_fileHeader;//File头指针
    int l_NumberOfSections = NULL;//节区头的数量
    pSections l_pSection;

    l_pSectionHeader = (PIMAGE_SECTION_HEADER)( g_NtFileAddress + 0xf4);
    l_fileHeader = (PIMAGE_FILE_HEADER)g_NtFileAddress;
    l_NumberOfSections = l_fileHeader->NumberOfSections;

    l_pSection = (pSections)malloc(sizeof(Sections) * l_NumberOfSections);

    for (int i = 0; i < l_NumberOfSections; i++)
    {
        l_pSection[i].RAW_begin = l_pSectionHeader[i].PointerToRawData;
        l_pSection[i].RVA_begin = l_pSectionHeader[i].VirtualAddress;

    }

    for (int i = 0; i < l_NumberOfSections; i++)
    {
        if (RVA < l_pSection[i].RVA_begin)
        {
            int Result = RVA - l_pSection[i - 1].RVA_begin + l_pSection[i - 1].RAW_begin;
            free((void*)l_pSection);
            return Result;
        }
    }
    int Result = RVA - l_pSection[l_NumberOfSections - 1].RVA_begin + l_pSection[l_NumberOfSections - 1].RAW_begin;
    free((void*)l_pSection);
    return  Result;
}

void pe::showImportDirectoryTable(CPEViewMFCDlg* dlg) 
{
    PIMAGE_OPTIONAL_HEADER l_OptionalHeader = (PIMAGE_OPTIONAL_HEADER)g_OptionalAddress;
    PIMAGE_DATA_DIRECTORY  l_PDateDirectory;
    PIMAGE_IMPORT_DESCRIPTOR l_PImportDescriptor;
    CString strText;
    l_PDateDirectory = l_OptionalHeader->DataDirectory;

    //strText.Format(TEXT("%x"), l_PDateDirectory[1].VirtualAddress);
    int Raw = zzRvaToRaw(l_PDateDirectory[1].VirtualAddress);
    l_PImportDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)(g_DosAddress + Raw);
    strText.Format(TEXT("-------------------IMAGE_IMPORT_DESCRIPTOR--------------\r\n"));
    while (l_PImportDescriptor->Name != 0x0)
    {
        int NameIndex = zzRvaToRaw(l_PImportDescriptor->Name);
        PCHAR l_pName;//默认名称长度不超过50
        l_pName = (PCHAR)(g_DosAddress+NameIndex);

        int unicodeLen = MultiByteToWideChar(CP_ACP, 0, l_pName, -1, nullptr, 0);
        wchar_t* pUnicode = (wchar_t*)malloc(sizeof(wchar_t) * unicodeLen);

        MultiByteToWideChar(CP_ACP, 0, l_pName, -1, pUnicode, unicodeLen);
        strText.Format(TEXT("%sOriginalFirstThunk:%x\r\n"), strText, l_PImportDescriptor->OriginalFirstThunk);
        strText.Format(TEXT("%sName:%x                   %s\r\n"), strText, l_PImportDescriptor->Name, pUnicode);
        strText.Format(TEXT("%sFirstThunk:%x\r\n"), strText, l_PImportDescriptor->FirstThunk);
        strText.Format(TEXT("%s\r\n"),strText);
        l_PImportDescriptor++;
    }
    dlg->mShowEdit.SetWindowTextW(strText);
}


void pe::showImportAddressTable(CPEViewMFCDlg* dlg)
{
    PIMAGE_OPTIONAL_HEADER l_OptionalHeader = (PIMAGE_OPTIONAL_HEADER)g_OptionalAddress;
    PIMAGE_DATA_DIRECTORY  l_PDateDirectory;
    PIMAGE_IMPORT_DESCRIPTOR l_PImportDescriptor;
    CString strText;
    l_PDateDirectory = l_OptionalHeader->DataDirectory;

    //strText.Format(TEXT("%x"), l_PDateDirectory[1].VirtualAddress);
    int Raw = zzRvaToRaw(l_PDateDirectory[1].VirtualAddress);
    l_PImportDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)(g_DosAddress + Raw);
    strText.Format(TEXT("-------------------IMPORT_ADDRESS_TABLE--------------\r\n"));
    while (l_PImportDescriptor->Name != 0x0)
    {
        int RVA_1 = l_PImportDescriptor->FirstThunk;//获取地址导入表的RVA
        int RAW_1 = zzRvaToRaw(RVA_1);//将RVA转化为RAW
        DWORD* l_pDword =(DWORD *)(g_DosAddress + RAW_1);

        while (*l_pDword != 0x0) 
        {
            int a = *l_pDword;
            if (zzRvaToRaw(*l_pDword) > g_FileSize)
            {
                RVA_1 += 4;
                l_pDword++;
                continue;
            }
            int NameIndex = zzRvaToRaw(*l_pDword);
            PCHAR l_pName;//默认名称长度不超过50
            l_pName = (PCHAR)(g_DosAddress + NameIndex+2);
            int unicodeLen = MultiByteToWideChar(CP_ACP, 0, l_pName, -1, nullptr, 0);
            wchar_t* pUnicode = (wchar_t*)malloc(sizeof(wchar_t) * unicodeLen);
            MultiByteToWideChar(CP_ACP, 0, l_pName, -1, pUnicode, unicodeLen);
            strText.Format(TEXT("%s%x    %s\r\n"),strText,RVA_1,pUnicode);
            RVA_1 += 4;
            l_pDword++;

            free(pUnicode);

        }
        

        int Name = zzRvaToRaw(l_PImportDescriptor->Name);
        PCHAR l_Name;//默认名称长度不超过50
        l_Name = (PCHAR)(g_DosAddress + Name);

        int unicodeLen = MultiByteToWideChar(CP_ACP, 0, l_Name, -1, nullptr, 0);
        wchar_t* Unicode = (wchar_t*)malloc(sizeof(wchar_t) * unicodeLen);

        MultiByteToWideChar(CP_ACP, 0, l_Name, -1, Unicode, unicodeLen);

        
        strText.Format(TEXT("%s%x    %s\r\n"), strText, RVA_1, Unicode);
        strText.Format(TEXT("%s--------------------------------------\r\n"),strText);
        free(Unicode);
        l_PImportDescriptor++;

    }
    dlg->mShowEdit.SetWindowTextW(strText);
    dlg->UpdateData(FALSE);
}

void pe::showExportDirectory(CPEViewMFCDlg* dlg) 
{
    PIMAGE_OPTIONAL_HEADER l_OptionalHeader = (PIMAGE_OPTIONAL_HEADER)g_OptionalAddress;
    PIMAGE_DATA_DIRECTORY  l_PDateDirectory;
    PIMAGE_EXPORT_DIRECTORY l_pExportDirectory;
    CString strText;
    l_PDateDirectory = l_OptionalHeader->DataDirectory;

    //strText.Format(TEXT("%x"), l_PDateDirectory[1].VirtualAddress);

    if (l_PDateDirectory[0].VirtualAddress == 0x0) 
    {
        strText.Format(TEXT("没有导出表\r\n"));
        dlg->mShowEdit.SetWindowTextW(strText);
        dlg->UpdateData(FALSE);
        return ;
    }
    int Raw = zzRvaToRaw(l_PDateDirectory[0].VirtualAddress);

    l_pExportDirectory = (PIMAGE_EXPORT_DIRECTORY)(g_DosAddress + Raw);

    strText.Format(TEXT("------------------IMAGE_EXPORT_DIRECTORY--------------\r\n"));
    strText.Format(TEXT("%sNumberOfFunctions:%x\r\n"), strText,l_pExportDirectory->NumberOfFunctions);
    strText.Format(TEXT("%sNumberOfNames:%x\r\n"), strText, l_pExportDirectory->NumberOfNames);
    strText.Format(TEXT("%sAddressOfFunctions:%x\r\n"), strText, l_pExportDirectory->AddressOfFunctions);
    strText.Format(TEXT("%sAddressOfNames:%x\r\n"), strText, l_pExportDirectory->AddressOfNames);

    int l_nameRva = l_pExportDirectory->AddressOfNames;//名称数组RVA
    int* l_pNameAddress = (int*)(g_DosAddress + zzRvaToRaw(l_nameRva));//函数名所在RVA

    int l_funRva = l_pExportDirectory->AddressOfFunctions;//导出函数地址数组RVA
    int* l_pfunAddress = (int*)(g_DosAddress + zzRvaToRaw(l_funRva));

    for (int i = 0; i < l_pExportDirectory->NumberOfNames; i++)
    {
        PCHAR l_Name;//默认名称长度不超过50
        l_Name = (PCHAR)(g_DosAddress + zzRvaToRaw(*l_pNameAddress));

        int unicodeLen = MultiByteToWideChar(CP_ACP, 0, l_Name, -1, nullptr, 0);
        wchar_t* Unicode = (wchar_t*)malloc(sizeof(wchar_t) * unicodeLen);

        MultiByteToWideChar(CP_ACP, 0, l_Name, -1, Unicode, unicodeLen);
        strText.Format(TEXT("%s%x    %s\r\n"), strText,*l_pfunAddress, Unicode);
        free(Unicode);
        l_pNameAddress++;
        l_pfunAddress++;
    }

    dlg->mShowEdit.SetWindowTextW(strText);
    dlg->UpdateData(FALSE);
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
    l_pBaeRelocation =(PIMAGE_BASE_RELOCATION) (g_DosAddress+zzRvaToRaw(l_PDateDirectory[5].VirtualAddress));

    strText.Format(TEXT("------------------IMAGE_BASE_RELOCATION--------------\r\n"));
    int l_sum = 0;
    do
    {
        strText.Format(TEXT("%sVirtualAddress:%x\r\n"), strText, l_pBaeRelocation->VirtualAddress);
        l_BaeRelocation = l_BaeRelocation + 4;
        strText.Format(TEXT("%sSizeOfBlock:%x\r\n"), strText, l_pBaeRelocation->SizeOfBlock);
        l_BaeRelocation = l_BaeRelocation + 4;


        short int* l_pValue = (short int*)((PBYTE)l_pBaeRelocation + 8);

        while (l_pValue < (short int*)((PBYTE)l_pBaeRelocation + (l_pBaeRelocation->SizeOfBlock))) 
        {
            strText.Format(TEXT("%s %x %x\r\n"), strText, l_BaeRelocation, *l_pValue);
            l_BaeRelocation = l_BaeRelocation + 2;
            l_pValue++;
        }
        l_sum = l_sum + l_pBaeRelocation->SizeOfBlock;
        l_pBaeRelocation = (PIMAGE_BASE_RELOCATION)((PBYTE)l_pBaeRelocation + (l_pBaeRelocation->SizeOfBlock));


    } while (l_sum < l_BaeRelocationSize);

    dlg->mShowEdit.SetWindowTextW(strText);
    dlg->UpdateData(FALSE);
}


pe::~pe()
{
    g_DosAddress = NULL;
    g_FileSize = NULL;
    g_NtFileAddress = NULL;
    g_OptionalAddress = NULL;
}
