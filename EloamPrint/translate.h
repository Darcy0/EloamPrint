#ifndef TRANSLATE_H_
#define TRANSLATE_H_
#include <QString>
class Translate
{
private:
	QString m_path;				//����xml·��
	QString m_countryId;		//����ID
	bool m_isInitSuccess;		//�����Ƿ��ʼ���ɹ�
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

