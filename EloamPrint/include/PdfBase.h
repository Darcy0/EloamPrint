// PdfBase.h : PdfBase DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

#include <WTypes.h>
// CPdfBaseApp
// �йش���ʵ�ֵ���Ϣ������� PdfBase.cpp
//

/*class __declspec(dllexport)CPdfBaseApp : public CWinApp
{
public:
	CPdfBaseApp();
	void AddFile(LPCTSTR lpszFilePath);
	// ��ȡ�б����ļ�����
	DWORD GetFileCount(void);
	// ��ȡ�б���ָ���������ļ���
	BSTR GetFilePath(DWORD dwIndex);
	// ����PDF�ļ�
	BOOL CreatePDF(LPCTSTR lpszPDFPath);
	// ����б��е��ļ�
	void ClearFile(void);
	// ���ٸ�ģ��
	void Release(void);
private:
	// �ļ��б�
	
// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};*/

void AddFile(LPCSTR lpszFilePath);
	// ��ȡ�б����ļ�����
	DWORD GetFileCount(void);
	// ��ȡ�б���ָ���������ļ���
	BSTR GetFilePath(DWORD dwIndex);
	// ����PDF�ļ�
	BOOL CreatePDF(LPCSTR lpszPDFPath);
	// ����б��е��ļ�
	void ClearFile(void);