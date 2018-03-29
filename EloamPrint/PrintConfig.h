#ifndef PRINTCONFIG_H
#define PRINTCONFIG_H

#include <QWidget>
#include <QVariant>
#include <QButtonGroup>
#include <translate.h>
#include "ui_PrintConfig.h"

enum PrintSize//��ӡ����	
{
	PrintSizeA3 = 0,//A3ֽ����
	PrintSizeA4 = 1	//A4ֽ����
};

enum CalibrationCard//�궨���С
{
	CalibrationCardA4 = 0,	//A4�궨��
	CalibrationCardA5 = 1	//A5�궨��
};

struct PrintConfigParam
{
	bool isDeskew;						//�Զ����ã�true=�ǣ�false=��
	PrintSize printSize;				//��ӡ����
	bool isIdCard;						//����֤�ϳɣ�true=�ǣ�false=��
	bool isOrignal;						//ԭͼ��ӡ��true=�ǣ�false=��
	bool isAutoExposure;				//�Ƿ��Զ��ع⣺ture=�ǣ�false=��
	int exposureValue;					//�ع�ֵ
	int printNum;						//��ӡ����
	QString savePath;					//����·��
	CalibrationCard calibrationCard;	//�궨���ߴ�����
};
Q_DECLARE_METATYPE(PrintConfigParam)

class PrintConfig : public QWidget
{
	Q_OBJECT
signals:
	//�µĴ�ӡ�����źţ������ȷ�ϡ���ťʱ����
	void newPrintConfigParm(QVariant& newParm);
	//�ָ�Ĭ�϶�������źţ�������ָ�Ĭ�ϡ���ťʱ����
	void StandardizationRestore();
	//�궨�źţ�������궨����ťʱ����
	void StandardizationCheckDPI(CalibrationCard size);
public:
	PrintConfig(QWidget *parent = 0);
	~PrintConfig();
	//!�����ع�ֵ�ķ�Χ���ɹ�����ture,ʧ�ܷ���false
	bool SetExposureValueRange(int minVal,int maxVal);
	//!�����ع�ֵ
	bool SetExposureValue(int expValue);
	//!������������
	void SetConfigParam(PrintConfigParam &configParm);
protected:
	//���ݵ�ǰ��m_configParm��ʼ������
	virtual void showEvent(QShowEvent * event);
private:
	Ui::PrintConfig ui;
	Translate translate;
	QButtonGroup *m_printSizeGroup;	//��ӡ����ѡ����
	QButtonGroup *m_checkSizeGroup;	//�궨�ߴ�ѡ����
	QString m_currentSavePath;		//��ǰѡ��·��
	PrintConfigParam m_configParm;	//���ò���

	//���ʻ�����
	void setLanguage();
private slots:
	//!��Ӧ�Զ��ع⸴ѡ��״̬�ı��¼�
	void onAutoExposureStateChanged(int state);
	//!��Ӧ��ѡ��·������ť
	void onSelectSavePath();
	//!��Ӧ���궨����ť
	void onCheckDPI();
	//!��Ӧ���ָ�Ĭ�ϡ���ť
	void onRestore();
	//!��Ӧ����������ť
	void onHelp();
	//!��Ӧ��ȷ�ϡ���ť
	void onOk();
	//!��Ӧ��ȡ������ť
	void onCancel();
};

#endif // PRINTCONFIG_H
