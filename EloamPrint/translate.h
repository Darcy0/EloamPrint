#ifndef TRANSLATE_H_
#define TRANSLATE_H_
#include <QString>
class Translate
{
private:
	QString m_path;				//语言xml路径
	QString m_countryId;		//国家ID
	bool m_isInitSuccess;		//语言是否初始化成功
public:
	Translate();
	Translate(const QString& path,const QString& countryId);
	~Translate(void);
	void SetLanguageParm(const QString& path,const QString& countryId);
	QString GetlanguageAbbr(const QString& strID);
	QString GetlanguageFullName(const QString& strID);
	bool IsInitSuccess();
private:
	bool initLanguages(const QString& path,const QString& countryID);
};
#endif

