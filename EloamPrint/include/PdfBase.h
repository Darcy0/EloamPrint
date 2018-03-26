// PdfBase.h : PdfBase DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号

#include <WTypes.h>
// CPdfBaseApp
// 有关此类实现的信息，请参阅 PdfBase.cpp
//

/*class __declspec(dllexport)CPdfBaseApp : public CWinApp
{
public:
	CPdfBaseApp();
	void AddFile(LPCTSTR lpszFilePath);
	// 获取列表中文件个数
	DWORD GetFileCount(void);
	// 获取列表中指定索引的文件名
	BSTR GetFilePath(DWORD dwIndex);
	// 创建PDF文件
	BOOL CreatePDF(LPCTSTR lpszPDFPath);
	// 清除列表中的文件
	void ClearFile(void);
	// 销毁该模块
	void Release(void);
private:
	// 文件列表
	
// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};*/

void AddFile(LPCSTR lpszFilePath);
	// 获取列表中文件个数
	DWORD GetFileCount(void);
	// 获取列表中指定索引的文件名
	BSTR GetFilePath(DWORD dwIndex);
	// 创建PDF文件
	BOOL CreatePDF(LPCSTR lpszPDFPath);
	// 清除列表中的文件
	void ClearFile(void);