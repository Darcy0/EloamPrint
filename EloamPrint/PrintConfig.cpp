#include <Windows.h>  
#include <QFileDialog>
#include <QDir>
#include "PrintConfig.h"

PrintConfig::PrintConfig(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setLanguage();
	m_currentSavePath=QCoreApplication::applicationDirPath();
	m_configParm.isDeskew=true;
	m_configParm.printSize=PrintSizeA4;
	m_configParm.isIdCard=false;
	m_configParm.isOrignal=true;
	m_configParm.isAutoExposure=true;
	m_configParm.printNum=1;
	m_configParm.savePath="";
	m_configParm.calibrationCard=CalibrationCardA4;
	SetConfigParam(m_configParm);

	this->setWindowFlags(Qt::Dialog);
	this->setWindowModality(Qt::WindowModal);//模态显示
	m_printSizeGroup=new QButtonGroup(this);//打印幅度选择组
	m_printSizeGroup->addButton(ui.radioButton_PrintSizeA4);
	m_printSizeGroup->addButton(ui.radioButton_PrintSizeA3);
	m_checkSizeGroup=new QButtonGroup(this);
	m_checkSizeGroup->addButton(ui.radioButton_CalibrationCardA4);
	m_checkSizeGroup->addButton(ui.radioButton_CalibrationCardA5);
	//曝光值Slider和SpinBox联动
	connect(ui.checkBox_AutoExposure,SIGNAL(stateChanged(int)),this,SLOT(onAutoExposureStateChanged(int)));
	connect(ui.slider_Exposure, SIGNAL(valueChanged(int)), ui.spinBox_Exposure, SLOT(setValue(int)));
	connect(ui.spinBox_Exposure, SIGNAL(valueChanged(int)),ui.slider_Exposure, SLOT(setValue(int)));
	connect(ui.pushButton_SelectSavePath,SIGNAL(clicked()),this,SLOT(onSelectSavePath()));
	connect(ui.pushButton_CheckDPI,SIGNAL(clicked()),this,SLOT(onCheckDPI()));
	connect(ui.pushButton_Restore,SIGNAL(clicked()),this,SLOT(onRestore()));
	connect(ui.pushButton_Help,SIGNAL(clicked()),this,SLOT(onHelp()));
	connect(ui.pushButton_Ok,SIGNAL(clicked()),this,SLOT(onOk()));
	connect(ui.pushButton_Cancel,SIGNAL(clicked()),this,SLOT(onCancel()));
}

PrintConfig::~PrintConfig()
{
	if (NULL!=m_printSizeGroup)
	{
		delete m_printSizeGroup;
		m_printSizeGroup=NULL;
	}
	if (NULL!=m_checkSizeGroup)
	{
		delete m_checkSizeGroup;
		m_checkSizeGroup=NULL;
	}
}

//公共函数
bool PrintConfig::SetExposureValueRange( int minVal,int maxVal )
{
	if (maxVal<minVal)
	{//上限小于下限时，交换上下限
		int temp=minVal;
		minVal=maxVal;
		maxVal=temp;
	}
	if (maxVal>500||maxVal<-500||(maxVal==minVal))
	{//曝光范围异常，我们认为该设备不支持曝光
		ui.label_ExposureValue->hide();
		ui.slider_Exposure->hide();
		ui.spinBox_Exposure->hide();
		return false;
	}
	else
	{
		ui.label_ExposureValue->show();
		ui.slider_Exposure->show();
		ui.spinBox_Exposure->show();
		ui.slider_Exposure->setRange(minVal,maxVal);
		ui.spinBox_Exposure->setRange(minVal,maxVal);
	}
}

bool PrintConfig::SetExposureValue( int expValue )
{
	int minValue=ui.slider_Exposure->minimum();
	int maxValue=ui.slider_Exposure->maximum();
	if (expValue<=maxValue&&expValue>=minValue)
	{
		m_configParm.exposureValue=expValue;
		ui.slider_Exposure->setValue(expValue);
		ui.spinBox_Exposure->setValue(expValue);
		return true;
	}
	else
	{
		ui.label_ExposureValue->hide();
		ui.slider_Exposure->hide();
		ui.spinBox_Exposure->hide();
		return false;
	}
}

void PrintConfig::SetConfigParam( PrintConfigParam &configParm )
{
	//自动纠裁
	m_configParm.isDeskew=configParm.isDeskew;
	ui.radioButton_Deskew->setChecked(m_configParm.isDeskew);
	//打印幅面
	m_configParm.printSize=configParm.printSize;
	if (PrintSizeA3==m_configParm.printSize)
	{
		ui.radioButton_PrintSizeA3->setChecked(true);
	}
	else if (PrintSizeA4==m_configParm.printSize)
	{
		ui.radioButton_PrintSizeA4->setChecked(true);
	}
	//二代证合成
	m_configParm.isIdCard=configParm.isIdCard;
	ui.checkBox_IDCard->setChecked(m_configParm.isIdCard);
	//原图打印
	m_configParm.isOrignal=configParm.isOrignal;
	ui.checkBox_Original->setChecked(m_configParm.isOrignal);
	//自动曝光
	m_configParm.isAutoExposure=configParm.isAutoExposure;
	ui.checkBox_AutoExposure->setChecked(m_configParm.isAutoExposure);
	ui.slider_Exposure->setEnabled(!m_configParm.isAutoExposure);
	ui.spinBox_Exposure->setEnabled(!m_configParm.isAutoExposure);
	//曝光值
	SetExposureValue(configParm.exposureValue);
	//打印份数
	m_configParm.printNum=configParm.printNum;
	ui.spinBox_PrintNum->setValue(m_configParm.printNum);
	//保存路径
	m_configParm.savePath=configParm.savePath;
	ui.lineEdit_SavePath->setText(m_configParm.savePath);
	//标定卡类型尺寸
	m_configParm.calibrationCard=configParm.calibrationCard;
	if (CalibrationCardA4==m_configParm.calibrationCard)
	{
		ui.radioButton_CalibrationCardA4->setChecked(true);
	}
	else if (CalibrationCardA5==m_configParm.calibrationCard)
	{
		ui.radioButton_CalibrationCardA5->setChecked(true);
	}
}

//槽函数
void PrintConfig::onAutoExposureStateChanged(int state)
{
	bool checkState=(Qt::Checked==state);
	ui.slider_Exposure->setEnabled(!checkState);
	ui.spinBox_Exposure->setEnabled(!checkState);
}

void PrintConfig::onSelectSavePath()
{
	QString savePath = QFileDialog::getExistingDirectory(this,QString::fromLocal8Bit("保存路径"),m_currentSavePath);
	if(!savePath.isEmpty())
	{
		m_currentSavePath = savePath;
		ui.lineEdit_SavePath->setText(m_currentSavePath);
	}
}

void PrintConfig::onCheckDPI()
{
	CalibrationCard calibrationCard=CalibrationCardA4;
	if (ui.radioButton_CalibrationCardA5->isChecked())
	{
		calibrationCard=CalibrationCardA5;
	}
	emit StandardizationCheckDPI(calibrationCard);
}

void PrintConfig::onRestore()
{
	emit StandardizationRestore();
}

void PrintConfig::onHelp()
{
	QString path = QCoreApplication::applicationDirPath() + QString::fromLocal8Bit("/EloamPrint.chm");
	ShellExecute(NULL, L"open",(BSTR)(LPCWSTR)path.data(), NULL, NULL, SW_NORMAL);
}

void PrintConfig::onOk()
{
	//自动纠裁
	m_configParm.isDeskew=ui.radioButton_Deskew->isChecked();
	//打印幅面
	if (ui.radioButton_PrintSizeA4->isChecked())
	{
		m_configParm.printSize=PrintSizeA4;
	}
	else if (ui.radioButton_PrintSizeA3->isChecked())
	{
		m_configParm.printSize=PrintSizeA3;
	}
	//二代证合成
	m_configParm.isIdCard=ui.checkBox_IDCard->isChecked();
	//原图打印
	m_configParm.isOrignal=ui.checkBox_Original->isChecked();
	//自动曝光
	m_configParm.isAutoExposure=ui.checkBox_AutoExposure->isChecked();
	//曝光值
	m_configParm.exposureValue=ui.slider_Exposure->value();
	m_configParm.printNum=ui.spinBox_PrintNum->value();
	//保存路径
	QString path=ui.lineEdit_SavePath->text();
	QDir dir(path);
	if (!dir.exists())
	{//所填路径不在
		if (dir.mkpath(path))
		{//创建路径成功
			m_configParm.savePath=path;
		}
		else
		{//创建路径失败,显示路径显示为原来的
			ui.lineEdit_SavePath->setText(m_configParm.savePath);
		}
	}
	else
	{//所填路径存在
		m_configParm.savePath=ui.lineEdit_SavePath->text();
	}	
	//标定卡尺寸类型
	if(ui.radioButton_CalibrationCardA4->isChecked())
	{
		m_configParm.calibrationCard=CalibrationCardA4;
	}
	else if(ui.radioButton_CalibrationCardA5->isChecked())
	{
		m_configParm.calibrationCard=CalibrationCardA5;
	}
	QVariant parm;
	parm.setValue(m_configParm);
	emit newPrintConfigParm(parm);
	this->close();
}

void PrintConfig::onCancel()
{
	this->close();
}

//重新方法
void PrintConfig::showEvent( QShowEvent * event )
{
	SetConfigParam(m_configParm);
}

void PrintConfig::setLanguage()
{
	if (!translate.IsInitSuccess())
	{//语言初始化失败
		return;
	}
	//标题
	setWindowTitle(translate.GetlanguageAbbr("STRING_PRINT_CONFIG"));
	//纠偏裁边
	ui.label_Deskew->setText(translate.GetlanguageAbbr("STRING_PRINT_DESKEW"));
	//自动纠裁
	ui.radioButton_Deskew->setText(translate.GetlanguageAbbr("STRING_PRINT_AUTOCROP"));
	//打印幅面
	ui.label_PrintSize->setText(translate.GetlanguageAbbr("STRING_PRINT_SIZE"));
	//二代证合成
	ui.checkBox_IDCard->setText(translate.GetlanguageAbbr("STRING_PRINT_IDCARDCOMBINE"));
	//原图打印
	ui.checkBox_Original->setText(translate.GetlanguageAbbr("STRING_PRINT_ORIGINAL"));
	//曝光值
	//自动曝光
	//打印份数
	ui.label_PrintNum->setText(translate.GetlanguageAbbr("STRING_PRINT_NUM"));
	//保存路径
	ui.label_SavePath->setText(translate.GetlanguageAbbr("STRING_BAOCUN_LUJING"));
	//选择保存路径
	ui.pushButton_SelectSavePath->setText(translate.GetlanguageAbbr("STRING_PRINT_SELECTPATH"));
	//标定
	ui.groupBox_Standardization->setTitle(translate.GetlanguageAbbr("STRING_PRINT_STANDARDIZATION"));
	//标定卡尺寸
	ui.label_StandardizationSize->setText(translate.GetlanguageAbbr("STRING_PRINT_STANDARDSIZE"));
	//标定
	ui.pushButton_CheckDPI->setText(translate.GetlanguageAbbr("STRING_PRINT_CHECKDPI"));
	//恢复默认
	ui.pushButton_Restore->setText(translate.GetlanguageAbbr("STRING_PRINT_RESTORE"));
	//帮助
	ui.pushButton_Help->setText(translate.GetlanguageAbbr("STRING_PRINT_HELP"));
	//确认
	//取消
}