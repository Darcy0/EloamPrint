#include <Windows.h>
#include<QCoreApplication>
#include "translate.h"
#include "language\language.h"
Translate::Translate()
{
	m_path=QCoreApplication::applicationDirPath()+QString("/lang.xml");
	m_countryId="0";
	m_isInitSuccess=initLanguages(m_path,m_countryId);
}

Translate::Translate( const QString& path,const QString& countryId )
{
	m_path=path;
	m_countryId=countryId;
	m_isInitSuccess=initLanguages(path,countryId);
}

Translate::~Translate(void)
{
}

void Translate::SetLanguageParm( const QString& path,const QString& countryId )
{
	m_path=path;
	m_countryId=countryId;
	m_isInitSuccess=initLanguages(path,countryId);
}

QString Translate::GetlanguageAbbr( const QString& strID )
{
	QString dStr;
	if (!m_isInitSuccess)
	{//���Գ�ʼ��ʧ��
		dStr =dStr.fromLocal8Bit("���Գ�ʼ��ʧ�ܣ�");
		return dStr;
	}
	QByteArray byteArr=strID.toLatin1();
	char *cStr = byteArr.data();
	char abbr[200];
	memset(abbr,0,200);
	if(getLanguageItem(cStr))
	{
		strcpy(abbr, getLanguageItem(cStr)); //���ݾ���İ�ťid��ȡ��Id����д���ƣ����û����д���ȡȫ�ơ�
	}
	else
	{
		dStr = dStr.fromLocal8Bit("û�д�ID��Item!");
		return dStr;
	}
	if(strlen(abbr)>0)
	{			           
		//ת������  (LPWSTR)pUnicode
		int  unicodeLen = ::MultiByteToWideChar( CP_UTF8,0, abbr,-1,  NULL, 0 );  
		wchar_t *  pUnicode;  
		pUnicode = new  wchar_t[unicodeLen+1];  
		memset(pUnicode,0,(unicodeLen+1)*sizeof(wchar_t));
		::MultiByteToWideChar( CP_UTF8,0,abbr, -1,(LPWSTR)pUnicode ,unicodeLen );  
		dStr =QString((QChar*)pUnicode);		
		return dStr;
			
	}
	else
	{
		return NULL;
	}		
}

QString Translate::GetlanguageFullName( const QString& strID )
{
	QString dStr;
	if (!m_isInitSuccess)
	{//���Գ�ʼ��ʧ��
		dStr =dStr.fromLocal8Bit("���Գ�ʼ��ʧ�ܣ�");
		return dStr;
	}
	char *cStrId =strID.toLatin1().data();
	char getname[200];
	memset(getname,0,200);
	if(getLanguageFname(cStrId))
	{
		strcpy(getname,getLanguageFname(cStrId));
	}
	else
	{
		dStr =dStr.fromLocal8Bit("û�д�ID��Item��");
		return dStr;
	}
	if(strlen(getname)>0)
	{/*ת������  (LPWSTR)pUnicode*/ 	           		
		int  unicodeLen = ::MultiByteToWideChar( CP_UTF8,0, getname,-1,  NULL, 0 );  
		wchar_t *  pUnicode;  
		pUnicode = new  wchar_t[unicodeLen+1];  
		memset(pUnicode,0,(unicodeLen+1)*sizeof(wchar_t));
		::MultiByteToWideChar( CP_UTF8,0,getname, -1,(LPWSTR)pUnicode ,unicodeLen );  
		dStr =QString((QChar*)pUnicode);
		delete pUnicode;
		return dStr;
	}
	else
	{
		return NULL;
	}
}

bool Translate::IsInitSuccess()
{
	return m_isInitSuccess;
}

bool Translate::initLanguages( const QString& path,const QString& countryID )
{
	readXml(path.toLocal8Bit().data());//��ȡxml������
	int flag = getCountryLanguage(countryID.toLocal8Bit().data()); //����Cid��ѡ����Ҫ������һ�����ҵ�����
	if(flag == 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}