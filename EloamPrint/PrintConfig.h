#ifndef PRINTCONFIG_H
#define PRINTCONFIG_H

#include <QWidget>
#include <QVariant>
#include <QButtonGroup>
#include <translate.h>
#include "ui_PrintConfig.h"

enum PrintSize//打印幅面	
{
	PrintSizeA3 = 0,//A3纸幅面
	PrintSizeA4 = 1	//A4纸幅面
};

enum CalibrationCard//标定板大小
{
	CalibrationCardA4 = 0,	//A4标定板
	CalibrationCardA5 = 1	//A5标定板
};

struct PrintConfigParam
{
	bool isDeskew;						//自动纠裁：true=是，false=否
	PrintSize printSize;				//打印幅面
	bool isIdCard;						//二代证合成：true=是，false=否
	bool isOrignal;						//原图打印：true=是，false=否
	bool isAutoExposure;				//是否自动曝光：ture=是，false=否
	int exposureValue;					//曝光值
	int printNum;						//打印份数
	QString savePath;					//保存路径
	CalibrationCard calibrationCard;	//标定卡尺寸类型
};
Q_DECLARE_METATYPE(PrintConfigParam)

class PrintConfig : public QWidget
{
	Q_OBJECT
signals:
	//新的打印参数信号，点击“确认”按钮时发出
	void newPrintConfigParm(QVariant& newParm);
	//恢复默认定标参数信号，点击“恢复默认”按钮时发出
	void StandardizationRestore();
	//标定信号，点击“标定”按钮时发出
	void StandardizationCheckDPI(CalibrationCard size);
public:
	PrintConfig(QWidget *parent = 0);
	~PrintConfig();
	//!设置曝光值的范围，成功返回ture,失败返回false
	bool SetExposureValueRange(int minVal,int maxVal);
	//!设置曝光值
	bool SetExposureValue(int expValue);
	//!设置配置数据
	void SetConfigParam(PrintConfigParam &configParm);
protected:
	//根据当前的m_configParm初始化界面
	virtual void showEvent(QShowEvent * event);
private:
	Ui::PrintConfig ui;
	Translate translate;
	QButtonGroup *m_printSizeGroup;	//打印幅面选择组
	QButtonGroup *m_checkSizeGroup;	//标定尺寸选择组
	QString m_currentSavePath;		//当前选择路径
	PrintConfigParam m_configParm;	//配置参数

	//国际化语言
	void setLanguage();
private slots:
	//!响应自动曝光复选框状态改变事件
	void onAutoExposureStateChanged(int state);
	//!响应“选择路径”按钮
	void onSelectSavePath();
	//!响应“标定”按钮
	void onCheckDPI();
	//!响应“恢复默认”按钮
	void onRestore();
	//!响应“帮助”按钮
	void onHelp();
	//!响应“确认”按钮
	void onOk();
	//!响应“取消”按钮
	void onCancel();
};

#endif // PRINTCONFIG_H
