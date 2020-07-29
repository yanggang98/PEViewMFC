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
     strText.Format(TEXT("%sSignature:%08X\r\n"),strText, l_dosHeader->e_magic);
     strText.Format(TEXT("%soffset to New Exe Header:%08X\r\n"), strText,l_dosHeader->e_lfanew);
     dlg->mShowEdit.SetWindowTextW(strText);
     dlg->UpdateData(FALSE);
}
//typedef struct _IMAGE_FILE_HEADER {
//    WORD    Machine;
//    WORD    NumberOfSections;
//    DWORD   TimeDateStamp;
//    DWORD   PointerToSymbolTable;
//    DWORD   NumberOfSymbols;
//    WORD    SizeOfOptionalHeader;
//    WORD    Characteristics;
//}

void pe::showNtFileHeader(CPEViewMFCDlg* dlg)
{
    PIMAGE_FILE_HEADER l_fileHeader = (PIMAGE_FILE_HEADER)g_NtFileAddress;
    CString strText;
    strText.Format(TEXT("----------------------IMAGE_FILE_HEADER-----------------\r\n"));
    strText.Format(TEXT("%sMachine:%08X\r\n"), strText, l_fileHeader->Machine);
    strText.Format(TEXT("%sNumberOfSections:%08X\r\n"), strText, l_fileHeader->NumberOfSections);
    strText.Format(TEXT("%sTimeDateStamp:%08X\r\n"), strText, l_fileHeader->TimeDateStamp);
    strText.Format(TEXT("%sPointerToSymbolTable:%08X\r\n"), strText, l_fileHeader->PointerToSymbolTable);
    strText.Format(TEXT("%sNumberOfSymbols:%08X\r\n"), strText, l_fileHeader->NumberOfSymbols);
    strText.Format(TEXT("%sSizeOfOptionalHeader:%08X\r\n"), strText, l_fileHeader->SizeOfOptionalHeader);
    strText.Format(TEXT("%sCharacteristics:%08X\r\n"), strText, l_fileHeader->Characteristics);
    dlg->mShowEdit.SetWindowTextW(strText);
    dlg->UpdateData(FALSE);
}


void pe::showOptionaHeader(CPEViewMFCDlg* dlg)
{
    PIMAGE_OPTIONAL_HEADER l_OptionalHeader = (PIMAGE_OPTIONAL_HEADER)g_OptionalAddress;
    CString strText;
    strText.Format(TEXT("----------------------IMAGE_OPTIONAL_HEADER-----------------\r\n"));
    strText.Format(TEXT("%sMagic:%08X\r\n"),strText, l_OptionalHeader->Magic);
    strText.Format(TEXT("%sMajorLinkerVersion:%08X\r\n"), strText, l_OptionalHeader->MajorLinkerVersion);
    strText.Format(TEXT("%sMinorLinkerVersion:%08X\r\n"), strText, l_OptionalHeader->MinorLinkerVersion);
    strText.Format(TEXT("%sSizeOfCode:%08X\r\n"), strText, l_OptionalHeader->SizeOfCode);
    strText.Format(TEXT("%sSizeOfInitializedData:%08X\r\n"), strText, l_OptionalHeader->SizeOfInitializedData);
    strText.Format(TEXT("%sSizeOfUninitializedData:%08X\r\n"), strText, l_OptionalHeader->SizeOfUninitializedData);
    strText.Format(TEXT("%sAddressOfEntryPoint:%08X\r\n"), strText, l_OptionalHeader->AddressOfEntryPoint);
    strText.Format(TEXT("%sBaseOfCode:%08X\r\n"), strText, l_OptionalHeader->BaseOfCode);
    strText.Format(TEXT("%sBaseOfData:%08X\r\n"), strText, l_OptionalHeader->BaseOfData);
    strText.Format(TEXT("%sImageBase:%08X\r\n"), strText, l_OptionalHeader->ImageBase);
    strText.Format(TEXT("%sSectionAlignment:%08X\r\n"), strText, l_OptionalHeader->SectionAlignment);
    strText.Format(TEXT("%sFileAlignment:%08X\r\n"), strText, l_OptionalHeader->FileAlignment);
    strText.Format(TEXT("%sMajorOperatingSystemVersion:%08X\r\n"), strText, l_OptionalHeader->MajorOperatingSystemVersion);
    strText.Format(TEXT("%sMinorOperatingSystemVersion:%08X\r\n"), strText, l_OptionalHeader->MinorOperatingSystemVersion);
    strText.Format(TEXT("%sMajorImageVersion:%08X\r\n"), strText, l_OptionalHeader->MajorImageVersion);
    strText.Format(TEXT("%sMinorImageVersion:%08X\r\n"), strText, l_OptionalHeader->MinorImageVersion);
    strText.Format(TEXT("%sMajorSubsystemVersion:%08X\r\n"), strText, l_OptionalHeader->MajorSubsystemVersion);
    strText.Format(TEXT("%sMinorSubsystemVersion:%08X\r\n"), strText, l_OptionalHeader->MinorSubsystemVersion);
    strText.Format(TEXT("%sWin32VersionValue:%08X\r\n"), strText, l_OptionalHeader->Win32VersionValue);
    strText.Format(TEXT("%sSizeOfImage:%08X\r\n"), strText, l_OptionalHeader->SizeOfImage);
    strText.Format(TEXT("%sSizeOfHeaders:%08X\r\n"), strText, l_OptionalHeader->SizeOfHeaders);
    strText.Format(TEXT("%sCheckSum:%08X\r\n"), strText, l_OptionalHeader->CheckSum);
    strText.Format(TEXT("%sSubsystem:%08X\r\n"), strText, l_OptionalHeader->Subsystem);
    strText.Format(TEXT("%sDllCharacteristics:%08X\r\n"), strText, l_OptionalHeader->DllCharacteristics);
    strText.Format(TEXT("%sSizeOfStackReserve:%08X\r\n"), strText, l_OptionalHeader->SizeOfStackReserve);
    strText.Format(TEXT("%sSizeOfStackCommit:%08X\r\n"), strText, l_OptionalHeader->SizeOfStackCommit);
    strText.Format(TEXT("%sSizeOfHeapReserve:%08X\r\n"), strText, l_OptionalHeader->SizeOfHeapReserve);
    strText.Format(TEXT("%sSizeOfHeapCommit:%08X\r\n"), strText, l_OptionalHeader->SizeOfHeapCommit);
    strText.Format(TEXT("%sLoaderFlags:%08X\r\n"), strText, l_OptionalHeader->LoaderFlags);
    strText.Format(TEXT("%sNumberOfRvaAndSizes:%08X\r\n"), strText, l_OptionalHeader->NumberOfRvaAndSizes); 
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

    l_pSectionHeader = (PIMAGE_SECTION_HEADER)( g_OptionalAddress + ((PIMAGE_FILE_HEADER)g_NtFileAddress)->SizeOfOptionalHeader);
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
        strText.Format(TEXT("%sOriginalFirstThunk:%08X\r\n"), strText, l_PImportDescriptor->OriginalFirstThunk);
        strText.Format(TEXT("%sName:%08X                   %s\r\n"), strText, l_PImportDescriptor->Name, pUnicode);
        strText.Format(TEXT("%sFirstThunk:%08X\r\n"), strText, l_PImportDescriptor->FirstThunk);
        strText.Format(TEXT("%s---------------------------------------------------------------\r\n"),strText);
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

    strText.Format(TEXT("%s RVA           DATE             名称\r\n"),strText);
    while (l_PImportDescriptor->Name != 0x0)
    {
        int RVA_1 = l_PImportDescriptor->FirstThunk;//获取地址导入表的RVA
        int RAW_1 = zzRvaToRaw(RVA_1);//将RVA转化为RAW
        PIMAGE_THUNK_DATA l_pThunkData =(PIMAGE_THUNK_DATA)(g_DosAddress + RAW_1);

        while (*(PDWORD)l_pThunkData!=0x0)
        {
            if (((*(PDWORD)l_pThunkData) >> 31) == 1) 
            {
                strText.Format(TEXT("%s%08X  %08X  %08X\r\n"), strText, RVA_1,(*(PDWORD)l_pThunkData), (*(PDWORD)l_pThunkData)&0x7fffffff);
                RVA_1 += 4;
                l_pThunkData++;
                continue;
            }
            //int a = *l_pDword;
            //if (zzRvaToRaw(*l_pDword) > g_FileSize)
            //{
            //    RVA_1 += 4;
            //    l_pDword++;
            //    continue;
            //}
            //if()l_pThunkData

            //int NameIndex = zzRvaToRaw(*l_pDword);
            //PCHAR l_pName;//默认名称长度不超过50
            //l_pName = (PCHAR)(g_DosAddress + NameIndex+2);
            //int unicodeLen = MultiByteToWideChar(CP_ACP, 0, l_pName, -1, nullptr, 0);
            //wchar_t* pUnicode = (wchar_t*)malloc(sizeof(wchar_t) * unicodeLen);
            //MultiByteToWideChar(CP_ACP, 0, l_pName, -1, pUnicode, unicodeLen);
            //strText.Format(TEXT("%s%x    %s\r\n"),strText,RVA_1,pUnicode);
            //RVA_1 += 4;
            //l_pDword++;
            //free(pUnicode);


            PIMAGE_IMPORT_BY_NAME l_pName = (PIMAGE_IMPORT_BY_NAME)(g_DosAddress + zzRvaToRaw(l_pThunkData->u1.AddressOfData));
            
            int unicodeLen = MultiByteToWideChar(CP_ACP, 0, l_pName->Name, -1, nullptr, 0);
            wchar_t* pUnicode = (wchar_t*)malloc(sizeof(wchar_t) * unicodeLen);
            MultiByteToWideChar(CP_ACP, 0, l_pName->Name, -1, pUnicode, unicodeLen);
            strText.Format(TEXT("%s%08X  %08X  %s\r\n"), strText, RVA_1, (*(PDWORD)l_pThunkData), pUnicode);
            RVA_1 += 4;
            l_pThunkData++;
        }
        

        int Name = zzRvaToRaw(l_PImportDescriptor->Name);
        PCHAR l_Name;//默认名称长度不超过50
        l_Name = (PCHAR)(g_DosAddress + Name);

        int unicodeLen = MultiByteToWideChar(CP_ACP, 0, l_Name, -1, nullptr, 0);
        wchar_t* Unicode = (wchar_t*)malloc(sizeof(wchar_t) * unicodeLen);

        MultiByteToWideChar(CP_ACP, 0, l_Name, -1, Unicode, unicodeLen);
        
        strText.Format(TEXT("%s%08X  %08X  %s\r\n"), strText, RVA_1,0x00000000, Unicode);
        strText.Format(TEXT("%s------------------------------------------------------------\r\n"),strText);
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
    //typedef struct _IMAGE_EXPORT_DIRECTORY {
    //    DWORD   Characteristics;
    //    DWORD   TimeDateStamp;
    //    WORD    MajorVersion;
    //    WORD    MinorVersion;
    //    DWORD   Name;
    //    DWORD   Base;
    //    DWORD   NumberOfFunctions;
    //    DWORD   NumberOfNames;
    //    DWORD   AddressOfFunctions;     // RVA from base of image
    //    DWORD   AddressOfNames;         // RVA from base of image
    //    DWORD   AddressOfNameOrdinals;  // RVA from base of image
    //} IMAGE_EXPORT_DIRECTORY, * PIMAGE_EXPORT_DIRECTORY;

    strText.Format(TEXT("------------------IMAGE_EXPORT_DIRECTORY--------------\r\n"));

    strText.Format(TEXT("%sCharacteristics:%X\r\n"), strText, l_pExportDirectory->Characteristics);
    strText.Format(TEXT("%sTimeDateStamp:%X\r\n"), strText, l_pExportDirectory->TimeDateStamp);
    strText.Format(TEXT("%sMajorVersion:%X\r\n"), strText, l_pExportDirectory->MajorVersion);
    strText.Format(TEXT("%sMinorVersion:%X\r\n"), strText, l_pExportDirectory->MinorVersion);
    strText.Format(TEXT("%sName:%X\r\n"), strText, l_pExportDirectory->Name);
    strText.Format(TEXT("%sBase:%X\r\n"), strText, l_pExportDirectory->Base);
    strText.Format(TEXT("%sNumberOfFunctions:%X\r\n"), strText,l_pExportDirectory->NumberOfFunctions);
    strText.Format(TEXT("%sNumberOfNames:%X\r\n"), strText, l_pExportDirectory->NumberOfNames);
    strText.Format(TEXT("%sAddressOfFunctions:%X\r\n"), strText, l_pExportDirectory->AddressOfFunctions);
    strText.Format(TEXT("%sAddressOfNames:%X\r\n"), strText, l_pExportDirectory->AddressOfNames);
    strText.Format(TEXT("%sAddressOfNameOrdinals:%X\r\n"), strText, l_pExportDirectory->AddressOfNameOrdinals);
    strText.Format(TEXT("%s----------------------------------------------------\r\n"),strText);
    strText.Format(TEXT("%s 序号               RVA           RAW           名称\r\n"), strText);
    //名称数组首地址
    int l_nameRva = l_pExportDirectory->AddressOfNames;
    int* l_pNameAddress = (int*)(g_DosAddress + zzRvaToRaw(l_nameRva));
    //地址数组首地址
    int l_funRva = l_pExportDirectory->AddressOfFunctions;
    int* l_pfunAddress = (int*)(g_DosAddress + zzRvaToRaw(l_funRva));
    //序列号数组首地址
    int l_pNameOrdinals = l_pExportDirectory->AddressOfNameOrdinals;
    WORD* l_NameOrdinalsAddress = (WORD*)(g_DosAddress + zzRvaToRaw(l_pNameOrdinals));


    for (int i = 0; i < l_pExportDirectory->NumberOfFunctions; i++)
    {
        
        bool flag = false;
        if (l_pfunAddress[i] == 0x0) 
        {
            continue;
        }
        for (int j = 0; j < l_pExportDirectory->NumberOfNames; j++)
        {
            if (l_NameOrdinalsAddress[j] == i)
            {
                PCHAR l_Name;
                l_Name = (PCHAR)(g_DosAddress + zzRvaToRaw(l_pNameAddress[j]));
                int unicodeLen = MultiByteToWideChar(CP_ACP, 0, l_Name, -1, nullptr, 0);
                wchar_t* Unicode = (wchar_t*)malloc(sizeof(wchar_t) * unicodeLen);

                MultiByteToWideChar(CP_ACP, 0, l_Name, -1, Unicode, unicodeLen);
                strText.Format(TEXT("%s%08X  %08X  %08X  %s\r\n"), strText,i+l_pExportDirectory->Base,l_pfunAddress[i],zzRvaToRaw(l_pfunAddress[i]), Unicode);
                free(Unicode);
                flag = true;

            }
        }
        if (flag == false) 
        {
            strText.Format(TEXT("%s%08X  %08X  %08X  \r\n"), strText, i + l_pExportDirectory->Base, l_pfunAddress[i],zzRvaToRaw(l_pfunAddress[i]));
        }
        
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
    if (l_BaeRelocation == 0) 
    {
        strText.Format(TEXT("没有重定位表\r\n"));
        dlg->mShowEdit.SetWindowTextW(strText);
        dlg->UpdateData(FALSE);
        return;
    }
    l_pBaeRelocation =(PIMAGE_BASE_RELOCATION) (g_DosAddress+zzRvaToRaw(l_PDateDirectory[5].VirtualAddress));

    strText.Format(TEXT("------------------IMAGE_BASE_RELOCATION--------------\r\n"));
    int l_sum = 0;
    do
    {
        strText.Format(TEXT("%sVirtualAddress:%08X\r\n"), strText, l_pBaeRelocation->VirtualAddress);
        l_BaeRelocation = l_BaeRelocation + 4;
        strText.Format(TEXT("%sSizeOfBlock:%08X\r\n"), strText, l_pBaeRelocation->SizeOfBlock);
        l_BaeRelocation = l_BaeRelocation + 4;


        short int* l_pValue = (short int*)((PBYTE)l_pBaeRelocation + 8);

        while (l_pValue < (short int*)((PBYTE)l_pBaeRelocation + (l_pBaeRelocation->SizeOfBlock))) 
        {
            

            strText.Format(TEXT("%sRVA:%08X  DATA:%08X     TYPE:IMAGE_REL_BASED_HIGHLOW\r\n"), strText, l_BaeRelocation, *l_pValue);
            l_BaeRelocation = l_BaeRelocation + 2;
            l_pValue++;
        }
        l_sum = l_sum + l_pBaeRelocation->SizeOfBlock;
        l_pBaeRelocation = (PIMAGE_BASE_RELOCATION)((PBYTE)l_pBaeRelocation + (l_pBaeRelocation->SizeOfBlock));
        strText.Format(TEXT("%s----------------------------------------------------------------\r\n"), strText);

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
