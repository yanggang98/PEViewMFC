#include "pch.h"
#include "PEView.h"
#include <fstream>
#include "PEViewMFCDlg.h"
#include <thread>
using namespace std;


DWORD FileSize(CString Filepath)
{

    //创建文件
    HANDLE hFile = CreateFile(Filepath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    //判断文件是否创建失败，失败则返回
    if (INVALID_HANDLE_VALUE == hFile)
    {
        return 0;
    }
    DWORD dwFileSize = 0;
    dwFileSize = GetFileSize(hFile, NULL);
    //关闭文件句柄
    CloseHandle(hFile);
    //返回文件大小
    return dwFileSize;
}


//pe构造函数，将文件信息读入缓冲区
pe::pe(CString FilePath)
{
    
    //计算文件大小
    int size = FileSize(FilePath);

    //如果文件大小为0，文件路径错误创建失败
    if(size <=0)
    {
        g_FileSize = 0;
        g_DosAddress = NULL;
        g_NtFileAddress = NULL;
        g_OptionalAddress = NULL;
    }
    else 
    {
        //创建文件大小的缓冲区
        PBYTE l_buffer = (PBYTE)malloc(size);
        ifstream l_FileIfstream;
        //打开流
        l_FileIfstream.open(FilePath, ios::binary);
        l_FileIfstream.seekg(0);
        //将文件存入缓冲区
        l_FileIfstream.read((char*)l_buffer, size);
        //关闭流
        l_FileIfstream.close();
        //给全局变量赋值
        g_DosAddress = l_buffer;
        g_FileSize = size;
        PIMAGE_DOS_HEADER l_DosAddress = (PIMAGE_DOS_HEADER)g_DosAddress;
        g_NtFileAddress = g_DosAddress + l_DosAddress->e_lfanew + 0x4;
        g_OptionalAddress = g_DosAddress + l_DosAddress->e_lfanew + 0x18;

    }

}


void pe::showDoSHeader(CPEViewMFCDlg *dlg)
{
     //获取DOS头结构体
     PIMAGE_DOS_HEADER l_dosHeader = (PIMAGE_DOS_HEADER)g_DosAddress;
     //打印信息
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
    //获取FIle头结构体
    PIMAGE_FILE_HEADER l_fileHeader = (PIMAGE_FILE_HEADER)g_NtFileAddress;
    //打印信息
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
    //获取可选头结构体
    PIMAGE_OPTIONAL_HEADER l_OptionalHeader = (PIMAGE_OPTIONAL_HEADER)g_OptionalAddress;
    //打印信息
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



// 内存偏移到文件偏移的转换
int pe::zzRvaToRaw(int RVA)
{

    PIMAGE_SECTION_HEADER l_pSectionHeader;//节区头指针
    PIMAGE_FILE_HEADER l_fileHeader;//File头指针
    int l_NumberOfSections = NULL;//节区头的数量

    //获取第一个节区的结构体
    l_pSectionHeader = (PIMAGE_SECTION_HEADER)( g_OptionalAddress + ((PIMAGE_FILE_HEADER)g_NtFileAddress)->SizeOfOptionalHeader);
    //获取File头结构体
    l_fileHeader = (PIMAGE_FILE_HEADER)g_NtFileAddress;
    //获取节区的数量
    l_NumberOfSections = l_fileHeader->NumberOfSections;

    //遍历所有节区
    for (int i = 0; i < l_NumberOfSections; i++)
    {
        //如果节区的起始位置的内存偏移 大于 RVA ,说明RVA 在上一个节区
        if (RVA < l_pSectionHeader[i].VirtualAddress)
        {
            //计算文件偏移，RVA减去所在节区起始位置的内存偏移,加上所在节区的文件偏移
            int Result = RVA - l_pSectionHeader[i - 1].VirtualAddress + l_pSectionHeader[i - 1].PointerToRawData;
            return Result;
        }
    }


    //当RVA 大于 最后一个节区的起始的内存偏移 
    int Result = RVA - l_pSectionHeader[l_NumberOfSections - 1].VirtualAddress + l_pSectionHeader[l_NumberOfSections - 1].PointerToRawData;
    return  Result;
}

void pe::showImportDirectoryTable(CPEViewMFCDlg* dlg) 
{
    //获取可选头结构体
    PIMAGE_OPTIONAL_HEADER l_OptionalHeader = (PIMAGE_OPTIONAL_HEADER)g_OptionalAddress;
    PIMAGE_DATA_DIRECTORY  l_PDateDirectory;
    PIMAGE_IMPORT_DESCRIPTOR l_PImportDescriptor;
    CString strText;
    l_PDateDirectory = l_OptionalHeader->DataDirectory;

    //获取IID的文件偏移
    int Raw = zzRvaToRaw(l_PDateDirectory[1].VirtualAddress);
    //获取IID数组第一个结构体指针
    l_PImportDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)(g_DosAddress + Raw);
    strText.Format(TEXT("-------------------IMAGE_IMPORT_DESCRIPTOR--------------\r\n"));
    //遍历所有的IID结构体，最后一个结构体为0
    while (l_PImportDescriptor->Name != 0x0)
    {
        //获取结构体成员name的文件偏移
        int NameIndex = zzRvaToRaw(l_PImportDescriptor->Name);
        PCHAR l_pName;
        //获取name的实际地址
        l_pName = (PCHAR)(g_DosAddress+NameIndex);
        //将ascii变为Unicode
        int unicodeLen = MultiByteToWideChar(CP_ACP, 0, l_pName, -1, nullptr, 0);
        wchar_t* pUnicode = (wchar_t*)malloc(sizeof(wchar_t) * unicodeLen);
        MultiByteToWideChar(CP_ACP, 0, l_pName, -1, pUnicode, unicodeLen);
        //打印信息
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

    // 获取IID的文件偏移
    int Raw = zzRvaToRaw(l_PDateDirectory[1].VirtualAddress);
    //获取IID数组第一个结构体指针
    l_PImportDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)(g_DosAddress + Raw);
    strText.Format(TEXT("-------------------IMPORT_ADDRESS_TABLE--------------\r\n"));
    strText.Format(TEXT("%s RVA           DATE             名称\r\n"),strText);
    //遍历所有的IID结构体，最后一个结构体为0
    while (l_PImportDescriptor->Name != 0x0)
    {
        //获取地址导入表的内存偏移
        int RVA_1 = l_PImportDescriptor->FirstThunk;
        //获取地址导入表的文件偏移
        int RAW_1 = zzRvaToRaw(RVA_1);//将RVA转化为RAW
        //地址导入表结构体的指针
        PIMAGE_THUNK_DATA l_pThunkData =(PIMAGE_THUNK_DATA)(g_DosAddress + RAW_1);

        //遍历所有 IMAGE_THUNK_DAT
        while (*(PDWORD)l_pThunkData!=0x0)
        {
            //判断是序号导入还是名称导入
            if (((*(PDWORD)l_pThunkData) >> 31) == 1) 
            {
                //序号导入，打印信息
                strText.Format(TEXT("%s%08X  %08X  %08X\r\n"), strText, RVA_1,(*(PDWORD)l_pThunkData), (*(PDWORD)l_pThunkData)&0x7fffffff);
                RVA_1 += 4;
                l_pThunkData++;
                continue;
            }
            //名称导入
            PIMAGE_IMPORT_BY_NAME l_pName = (PIMAGE_IMPORT_BY_NAME)(g_DosAddress + zzRvaToRaw(l_pThunkData->u1.AddressOfData));   
            //ascii转Unicode
            int unicodeLen = MultiByteToWideChar(CP_ACP, 0, l_pName->Name, -1, nullptr, 0);
            wchar_t* pUnicode = (wchar_t*)malloc(sizeof(wchar_t) * unicodeLen);
            MultiByteToWideChar(CP_ACP, 0, l_pName->Name, -1, pUnicode, unicodeLen);
            //打印信息
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

    //判断是否存在导出表
    if (l_PDateDirectory[0].VirtualAddress == 0x0) 
    {
        strText.Format(TEXT("没有导出表\r\n"));
        dlg->mShowEdit.SetWindowTextW(strText);
        dlg->UpdateData(FALSE);
        return ;
    }
    //获取导出表的文件偏移
    int Raw = zzRvaToRaw(l_PDateDirectory[0].VirtualAddress);
    //获取导出表的结构体指针
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
    //打印导出表的信息
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
    DWORD* l_pNameAddress = (DWORD*)(g_DosAddress + zzRvaToRaw(l_nameRva));
    //地址数组首地址
    int l_funRva = l_pExportDirectory->AddressOfFunctions;
    DWORD* l_pfunAddress = (DWORD*)(g_DosAddress + zzRvaToRaw(l_funRva));
    //序列号数组首地址
    int l_pNameOrdinals = l_pExportDirectory->AddressOfNameOrdinals;
    WORD* l_NameOrdinalsAddress = (WORD*)(g_DosAddress + zzRvaToRaw(l_pNameOrdinals));
    //遍历地址数组
    for (int i = 0; i < l_pExportDirectory->NumberOfFunctions; i++)
    {
        
        bool flag = false;
        //判断数组中存放的值是否是0
        if (l_pfunAddress[i] == 0x0) 
        {
            continue;
        }

        //遍历序列号数组
        for (int j = 0; j < l_pExportDirectory->NumberOfNames; j++)
        {
            //比较序列号数组中的值和地址数组的下标是否相等
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

        //判断是否序列号的数组中是否有地址数组的下标
        if (flag == false) 
        {
            strText.Format(TEXT("%s%08X  %08X  %08X  \r\n"), strText, i + l_pExportDirectory->Base, l_pfunAddress[i],zzRvaToRaw(l_pfunAddress[i]));
        }
        
    }

    dlg->mShowEdit.SetWindowTextW(strText);
    dlg->UpdateData(FALSE);
}

void pe::showImportNameTable(CPEViewMFCDlg* dlg) 
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

    strText.Format(TEXT("%s RVA           DATE             名称\r\n"), strText);
    while (l_PImportDescriptor->Name != 0x0)
    {
        int RVA_1 = l_PImportDescriptor->OriginalFirstThunk;//获取地址导入表的RVA
        int RAW_1 = zzRvaToRaw(RVA_1);//将RVA转化为RAW
        PIMAGE_THUNK_DATA l_pThunkData = (PIMAGE_THUNK_DATA)(g_DosAddress + RAW_1);

        while (*(PDWORD)l_pThunkData != 0x0)
        {
            if (((*(PDWORD)l_pThunkData) >> 31) == 1)
            {
                strText.Format(TEXT("%s%08X  %08X  %08X\r\n"), strText, RVA_1, (*(PDWORD)l_pThunkData), (*(PDWORD)l_pThunkData) & 0x7fffffff);
                RVA_1 += 4;
                l_pThunkData++;
                continue;
            }
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

        strText.Format(TEXT("%s%08X  %08X  %s\r\n"), strText, RVA_1, 0x00000000, Unicode);
        strText.Format(TEXT("%s------------------------------------------------------------\r\n"), strText);
        free(Unicode);
        l_PImportDescriptor++;

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
        strText.Format(TEXT("%sVirtualAddress:%X SizeOfBlock:%X \r\n"), strText, l_pBaeRelocation->VirtualAddress,l_pBaeRelocation->SizeOfBlock);
        l_BaeRelocation = l_BaeRelocation + 8;


        short int* l_pValue = (short int*)((PBYTE)l_pBaeRelocation + 8);

        while (l_pValue < (short int*)((PBYTE)l_pBaeRelocation + (l_pBaeRelocation->SizeOfBlock))) 
        {           
            strText.Format(TEXT("%sRVA%X FOA:%X \r\n"), strText, (*l_pValue & 0xfff) + l_pBaeRelocation->VirtualAddress, zzRvaToRaw((*l_pValue & 0xFFF) + l_pBaeRelocation->VirtualAddress));
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
