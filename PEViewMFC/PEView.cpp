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

    CRect rect;
    //清空信息
    dlg->mlist.DeleteAllItems();
    int a = dlg->mlist.GetHeaderCtrl()->GetItemCount();
    for (int i = 0; i < a; i++)
    {
        dlg->mlist.DeleteColumn(0);
    }
    //设置表头信息
    dlg->mlist.GetClientRect(&rect);
    dlg->mlist.InsertColumn(0, L"属性", LVCFMT_CENTER, rect.Width() / 3, -1);
    dlg->mlist.InsertColumn(1, L"值", LVCFMT_CENTER, rect.Width() / 3, -1);

     strText.Format(TEXT("%08X"), l_dosHeader->e_magic);
     dlg->mlist.InsertItem(0,_T("Signature"));
     dlg->mlist.SetItemText(0, 1, strText);

     strText.Format(TEXT("%08X"), l_dosHeader->e_lfanew);
     dlg->mlist.InsertItem(1, _T("offset to New Exe Header"));
     dlg->mlist.SetItemText(1, 1, strText);
}


void pe::showNtFileHeader(CPEViewMFCDlg* dlg)
{
    //获取FIle头结构体
    PIMAGE_FILE_HEADER l_fileHeader = (PIMAGE_FILE_HEADER)g_NtFileAddress;
    //打印信息
    CString strText;

    CRect rect;
    //清空信息
    dlg->mlist.DeleteAllItems();
    int a=dlg->mlist.GetHeaderCtrl()->GetItemCount();
    for (int i = 0; i < a; i++)
    {
        dlg->mlist.DeleteColumn(0);
    }
    //设置表头信息
    dlg->mlist.GetClientRect(&rect);
    dlg->mlist.InsertColumn(0, L"属性", LVCFMT_CENTER, rect.Width() / 3, -1);
    dlg->mlist.InsertColumn(1, L"值", LVCFMT_CENTER, rect.Width() / 3, -1);
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
    //获取可选头结构体
    PIMAGE_OPTIONAL_HEADER l_OptionalHeader = (PIMAGE_OPTIONAL_HEADER)g_OptionalAddress;
    //打印信息
    CString strText;

    CRect rect;
    //清空信息
    dlg->mlist.DeleteAllItems();
    int a = dlg->mlist.GetHeaderCtrl()->GetItemCount();
    for (int i = 0; i < a; i++)
    {
        dlg->mlist.DeleteColumn(0);
    }
    //设置表头信息
    dlg->mlist.GetClientRect(&rect);
    dlg->mlist.InsertColumn(0, L"属性", LVCFMT_CENTER, rect.Width() / 3, -1);
    dlg->mlist.InsertColumn(1, L"值", LVCFMT_CENTER, rect.Width() / 3, -1);
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
    CRect rect;
    //清空信息
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

    //获取IID的文件偏移
    int Raw = zzRvaToRaw(l_PDateDirectory[1].VirtualAddress);
    //获取IID数组第一个结构体指针
    l_PImportDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)(g_DosAddress + Raw);
    strText.Format(TEXT("-------------------IMAGE_IMPORT_DESCRIPTOR--------------\r\n"));
    //遍历所有的IID结构体，最后一个结构体为0
    int i = 0;
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

    // 获取IID的文件偏移
    int Raw = zzRvaToRaw(l_PDateDirectory[1].VirtualAddress);
    //获取IID数组第一个结构体指针
    l_PImportDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)(g_DosAddress + Raw);
    strText.Format(TEXT("-------------------IMPORT_ADDRESS_TABLE--------------\r\n"));
    strText.Format(TEXT("%s RVA           DATE             名称\r\n"),strText);

    CRect rect;
    //清空信息
    dlg->mlist.DeleteAllItems();
    int a = dlg->mlist.GetHeaderCtrl()->GetItemCount();
    for (int i = 0; i < a; i++)
    {
        dlg->mlist.DeleteColumn(0);
    }
    dlg->mlist.GetClientRect(&rect);
    dlg->mlist.InsertColumn(0, L"RVA", LVCFMT_CENTER, rect.Width() / 3, -1);
    dlg->mlist.InsertColumn(1, L"DATE", LVCFMT_CENTER, rect.Width() / 3, -1);
    dlg->mlist.InsertColumn(2, L"名称/序号", LVCFMT_CENTER, rect.Width() / 3, -1);

    //遍历所有的IID结构体，最后一个结构体为0
    int i = 0;
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
            //名称导入
            PIMAGE_IMPORT_BY_NAME l_pName = (PIMAGE_IMPORT_BY_NAME)(g_DosAddress + zzRvaToRaw(l_pThunkData->u1.AddressOfData));   
            //ascii转Unicode
            int unicodeLen = MultiByteToWideChar(CP_ACP, 0, l_pName->Name, -1, nullptr, 0);
            wchar_t* pUnicode = (wchar_t*)malloc(sizeof(wchar_t) * unicodeLen);
            MultiByteToWideChar(CP_ACP, 0, l_pName->Name, -1, pUnicode, unicodeLen);
            //打印信息
            //RVA
            strText.Format(TEXT("%08X"), RVA_1);
            dlg->mlist.InsertItem(i, strText);
            //DATE
            strText.Format(TEXT("%08X"), (*(PDWORD)l_pThunkData));
            dlg->mlist.SetItemText(i, 1, strText);
            //导入名称
            strText.Format(TEXT("%s"), pUnicode);
            dlg->mlist.SetItemText(i, 2, strText);
            i++;

            RVA_1 += 4;
            l_pThunkData++;
        }
        
        //ascii转Unicode
        int Name = zzRvaToRaw(l_PImportDescriptor->Name);
        PCHAR l_Name;//默认名称长度不超过50
        l_Name = (PCHAR)(g_DosAddress + Name);
        int unicodeLen = MultiByteToWideChar(CP_ACP, 0, l_Name, -1, nullptr, 0);
        wchar_t* Unicode = (wchar_t*)malloc(sizeof(wchar_t) * unicodeLen);
        MultiByteToWideChar(CP_ACP, 0, l_Name, -1, Unicode, unicodeLen);
        
        //打印信息
        //RVA
        strText.Format(TEXT("%08X"), RVA_1);
        dlg->mlist.InsertItem(i, strText);
        strText.Format(TEXT("%08X"), 0x00000000);
        dlg->mlist.SetItemText(i, 1, strText);
        //dll 名称
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

    //判断是否存在导出表
    if (l_PDateDirectory[0].VirtualAddress == 0x0) 
    {
        dlg->MessageBox(L"没有导出表");
        return ;
    }
    //获取导出表的文件偏移
    int Raw = zzRvaToRaw(l_PDateDirectory[0].VirtualAddress);
    //获取导出表的结构体指针
    l_pExportDirectory = (PIMAGE_EXPORT_DIRECTORY)(g_DosAddress + Raw);

    //名称数组首地址
    int l_nameRva = l_pExportDirectory->AddressOfNames;
    DWORD* l_pNameAddress = (DWORD*)(g_DosAddress + zzRvaToRaw(l_nameRva));
    //地址数组首地址
    int l_funRva = l_pExportDirectory->AddressOfFunctions;
    DWORD* l_pfunAddress = (DWORD*)(g_DosAddress + zzRvaToRaw(l_funRva));
    //序列号数组首地址
    int l_pNameOrdinals = l_pExportDirectory->AddressOfNameOrdinals;
    WORD* l_NameOrdinalsAddress = (WORD*)(g_DosAddress + zzRvaToRaw(l_pNameOrdinals));
  

    CRect rect;
    //清空信息
    dlg->mlist.DeleteAllItems();
    int a = dlg->mlist.GetHeaderCtrl()->GetItemCount();
    for (int i = 0; i < a; i++)
    {
        dlg->mlist.DeleteColumn(0);
    }
    //设置表头
    dlg->mlist.GetClientRect(&rect);
    dlg->mlist.InsertColumn(0, L"序号", LVCFMT_CENTER, rect.Width() / 3, -1);
    dlg->mlist.InsertColumn(1, L"RVA", LVCFMT_CENTER, rect.Width() / 3, -1);
    dlg->mlist.InsertColumn(2, L"名称", LVCFMT_CENTER, rect.Width() / 3, -1);

    //遍历地址数组
    int index = 0;
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
                //打印信息
                //序号
                strText.Format(TEXT("%08X"), i + l_pExportDirectory->Base);
                dlg->mlist.InsertItem(index, strText);
                //RVA
                strText.Format(TEXT("%08X"), l_pfunAddress[i]);
                dlg->mlist.SetItemText(index, 1, strText);
                //名称
                strText.Format(TEXT("%s"), Unicode);
                dlg->mlist.SetItemText(index, 2, strText);
                index++;
                std::free(Unicode);
                flag = true;
            }
        }

        //判断是否序列号的数组中是否有地址数组的下标
        if (flag == false) 
        {
         
            //打印信息
            //序号
            strText.Format(TEXT("%08X"), i + l_pExportDirectory->Base);
            dlg->mlist.InsertItem(index, strText);
            //RVA
            strText.Format(TEXT("%08X"), l_pfunAddress[i]);
            dlg->mlist.SetItemText(index, 1, strText);
            //名称
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

    // 获取IID的文件偏移
    int Raw = zzRvaToRaw(l_PDateDirectory[1].VirtualAddress);
    //获取IID数组第一个结构体指针
    l_PImportDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)(g_DosAddress + Raw);
    strText.Format(TEXT("-------------------IMPORT_ADDRESS_TABLE--------------\r\n"));
    strText.Format(TEXT("%s RVA           DATE             名称\r\n"), strText);

    CRect rect;
    //清空信息
    dlg->mlist.DeleteAllItems();
    int a = dlg->mlist.GetHeaderCtrl()->GetItemCount();
    for (int i = 0; i < a; i++)
    {
        dlg->mlist.DeleteColumn(0);
    }
    dlg->mlist.GetClientRect(&rect);
    dlg->mlist.InsertColumn(0, L"RVA", LVCFMT_CENTER, rect.Width() / 3, -1);
    dlg->mlist.InsertColumn(1, L"DATE", LVCFMT_CENTER, rect.Width() / 3, -1);
    dlg->mlist.InsertColumn(2, L"名称/序号", LVCFMT_CENTER, rect.Width() / 3, -1);

    //遍历所有的IID结构体，最后一个结构体为0
    int i = 0;
    while (l_PImportDescriptor->Name != 0x0)
    {
        //获取地址导入表的内存偏移
        int RVA_1 = l_PImportDescriptor->OriginalFirstThunk;
        //获取地址导入表的文件偏移
        int RAW_1 = zzRvaToRaw(RVA_1);//将RVA转化为RAW
        //地址导入表结构体的指针
        PIMAGE_THUNK_DATA l_pThunkData = (PIMAGE_THUNK_DATA)(g_DosAddress + RAW_1);

        //遍历所有 IMAGE_THUNK_DAT
        while (*(PDWORD)l_pThunkData != 0x0)
        {
            //判断是序号导入还是名称导入
            if (((*(PDWORD)l_pThunkData) >> 31) == 1)
            {
                //序号导入，打印信息

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
            //名称导入
            PIMAGE_IMPORT_BY_NAME l_pName = (PIMAGE_IMPORT_BY_NAME)(g_DosAddress + zzRvaToRaw(l_pThunkData->u1.AddressOfData));
            //ascii转Unicode
            int unicodeLen = MultiByteToWideChar(CP_ACP, 0, l_pName->Name, -1, nullptr, 0);
            wchar_t* pUnicode = (wchar_t*)malloc(sizeof(wchar_t) * unicodeLen);
            MultiByteToWideChar(CP_ACP, 0, l_pName->Name, -1, pUnicode, unicodeLen);
            //打印信息
            //RVA
            strText.Format(TEXT("%08X"), RVA_1);
            dlg->mlist.InsertItem(i, strText);
            //DATE
            strText.Format(TEXT("%08X"), (*(PDWORD)l_pThunkData));
            dlg->mlist.SetItemText(i, 1, strText);
            //导入名称
            strText.Format(TEXT("%s"), pUnicode);
            dlg->mlist.SetItemText(i, 2, strText);
            i++;

            RVA_1 += 4;
            l_pThunkData++;
        }

        //ascii转Unicode
        int Name = zzRvaToRaw(l_PImportDescriptor->Name);
        PCHAR l_Name;
        l_Name = (PCHAR)(g_DosAddress + Name);
        int unicodeLen = MultiByteToWideChar(CP_ACP, 0, l_Name, -1, nullptr, 0);
        wchar_t* Unicode = (wchar_t*)malloc(sizeof(wchar_t) * unicodeLen);
        MultiByteToWideChar(CP_ACP, 0, l_Name, -1, Unicode, unicodeLen);

        //打印信息
        //RVA
        strText.Format(TEXT("%08X"), RVA_1);
        dlg->mlist.InsertItem(i, strText);
        strText.Format(TEXT("%08X"), 0x00000000);
        dlg->mlist.SetItemText(i, 1, strText);
        //dll 名称
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

    //判断是否存在重定位表
    if (l_BaeRelocation == 0) 
    {
        dlg->MessageBox(L"没有重定位表");
        return;
    }
    l_pBaeRelocation =(PIMAGE_BASE_RELOCATION) (g_DosAddress+zzRvaToRaw(l_PDateDirectory[5].VirtualAddress));

    CRect rect;
    //清空信息
    dlg->mlist.DeleteAllItems();
    int a = dlg->mlist.GetHeaderCtrl()->GetItemCount();
    for (int i = 0; i < a; i++)
    {
        dlg->mlist.DeleteColumn(0);
    }
    //设置表头信息
    dlg->mlist.GetClientRect(&rect);
    dlg->mlist.InsertColumn(0, L"RVA", LVCFMT_CENTER, rect.Width() / 3, -1);
    dlg->mlist.InsertColumn(1, L"FOA", LVCFMT_CENTER, rect.Width() / 3, -1);
    dlg->mlist.InsertColumn(2, L"种类", LVCFMT_CENTER, rect.Width() / 3, -1);

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
            //种类
            //重定位类型
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
