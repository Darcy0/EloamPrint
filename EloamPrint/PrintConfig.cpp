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
	this->setWindowModality(Qt::WindowModal);//ģ̬��ʾ
	m_printSizeGroup=new QButtonGroup(this);//��ӡ����ѡ����
	m_printSizeGroup->addButton(ui.radioButton_PrintSizeA4);
	m_printSizeGroup->addButton(ui.radioButton_PrintSizeA3);
	m_checkSizeGroup=new QButtonGroup(this);
	m_checkSizeGroup->addButton(ui.radioButton_CalibrationCardA4);
	m_checkSizeGroup->addButton(ui.radioButton_CalibrationCardA5);
	//�ع�ֵSlider��SpinBox����
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

//��������
bool PrintConfig::SetExposureValueRange( int minVal,int maxVal )
{
	if (maxVal<minVal)
	{//����С������ʱ������������
		int temp=minVal;
		minVal=maxVal;
		maxVal=temp;
	}
	if (maxVal>500||maxVal<-500||(maxVal==minVal))
	{//�عⷶΧ�쳣��������Ϊ���豸��֧���ع�
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
	//�Զ�����
	m_configParm.isDeskew=configParm.isDeskew;
	ui.radioButton_Deskew->setChecked(m_configParm.isDeskew);
	//��ӡ����
	m_configParm.printSize=configParm.printSize;
	if (PrintSizeA3==m_configParm.printSize)
	{
		ui.radioButton_PrintSizeA3->setChecked(true);
	}
	else if (PrintSizeA4==m_configParm.printSize)
	{
		ui.radioButton_PrintSizeA4->setChecked(true);
	}
	//����֤�ϳ�
	m_configParm.isIdCard=configParm.isIdCard;
	ui.checkBox_IDCard->setChecked(m_configParm.isIdCard);
	//ԭͼ��ӡ
	m_configParm.isOrignal=configParm.isOrignal;
	ui.checkBox_Original->setChecked(m_configParm.isOrignal);
	//�Զ��ع�
	m_configParm.isAutoExposure=configParm.isAutoExposure;
	ui.checkBox_AutoExposure->setChecked(m_configParm.isAutoExposure);
	ui.slider_Exposure->setEnabled(!m_configParm.isAutoExposure);
	ui.spinBox_Exposure->setEnabled(!m_configParm.isAutoExposure);
	//�ع�ֵ
	SetExposureValue(configParm.exposureValue);
	//��ӡ����
	m_configParm.printNum=configParm.printNum;
	ui.spinBox_PrintNum->setValue(m_configParm.printNum);
	//����·��
	m_configParm.savePath=configParm.savePath;
	ui.lineEdit_SavePath->setText(m_configParm.savePath);
	//�궨�����ͳߴ�
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

//�ۺ���
void PrintConfig::onAutoExposureStateChanged(int state)
{
	bool checkState=(Qt::Checked==state);
	ui.slider_Exposure->setEnabled(!checkState);
	ui.spinBox_Exposure->setEnabled(!checkState);
}

void PrintConfig::onSelectSavePath()
{
	QString savePath = QFileDialog::getExistingDirectory(this,QString::fromLocal8Bit("����·��"),m_currentSavePath);
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
	//�Զ�����
	m_configParm.isDeskew=ui.radioButton_Deskew->isChecked();
	//��ӡ����
	if (ui.radioButton_PrintSizeA4->isChecked())
	{
		m_configParm.printSize=PrintSizeA4;
	}
	else if (ui.radioButton_PrintSizeA3->isChecked())
	{
		m_configParm.printSize=PrintSizeA3;
	}
	//����֤�ϳ�
	m_configParm.isIdCard=ui.checkBox_IDCard->isChecked();
	//ԭͼ��ӡ
	m_configParm.isOrignal=ui.checkBox_Original->isChecked();
	//�Զ��ع�
	m_configParm.isAutoExposure=ui.checkBox_AutoExposure->isChecked();
	//�ع�ֵ
	m_configParm.exposureValue=ui.slider_Exposure->value();
	m_configParm.printNum=ui.spinBox_PrintNum->value();
	//����·��
	QString path=ui.lineEdit_SavePath->text();
	QDir dir(path);
	if (!dir.exists())
	{//����·������
		if (dir.mkpath(path))
		{//����·���ɹ�
			m_configParm.savePath=path;
		}
		else
		{//����·��ʧ��,��ʾ·����ʾΪԭ����
			ui.lineEdit_SavePath->setText(m_configParm.savePath);
		}
	}
	else
	{//����·������
		m_configParm.savePath=ui.lineEdit_SavePath->text();
	}	
	//�궨���ߴ�����
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

//���·���
void PrintConfig::showEvent( QShowEvent * event )
{
	SetConfigParam(m_configParm);
}

void PrintConfig::setLanguage()
{
	if (!translate.IsInitSuccess())
	{//���Գ�ʼ��ʧ��
		return;
	}
	//����
	setWindowTitle(translate.GetlanguageAbbr("STRING_PRINT_CONFIG"));
	//��ƫ�ñ�
	ui.label_Deskew->setText(translate.GetlanguageAbbr("STRING_PRINT_DESKEW"));
	//�Զ�����
	ui.radioButton_Deskew->setText(translate.GetlanguageAbbr("STRING_PRINT_AUTOCROP"));
	//��ӡ����
	ui.label_PrintSize->setText(translate.GetlanguageAbbr("STRING_PRINT_SIZE"));
	//����֤�ϳ�
	ui.checkBox_IDCard->setText(translate.GetlanguageAbbr("STRING_PRINT_IDCARDCOMBINE"));
	//ԭͼ��ӡ
	ui.checkBox_Original->setText(translate.GetlanguageAbbr("STRING_PRINT_ORIGINAL"));
	//�ع�ֵ
	//�Զ��ع�
	//��ӡ����
	ui.label_PrintNum->setText(translate.GetlanguageAbbr("STRING_PRINT_NUM"));
	//����·��
	ui.label_SavePath->setText(translate.GetlanguageAbbr("STRING_BAOCUN_LUJING"));
	//ѡ�񱣴�·��
	ui.pushButton_SelectSavePath->setText(translate.GetlanguageAbbr("STRING_PRINT_SELECTPATH"));
	//�궨
	ui.groupBox_Standardization->setTitle(translate.GetlanguageAbbr("STRING_PRINT_STANDARDIZATION"));
	//�궨���ߴ�
	ui.label_StandardizationSize->setText(translate.GetlanguageAbbr("STRING_PRINT_STANDARDSIZE"));
	//�궨
	ui.pushButton_CheckDPI->setText(translate.GetlanguageAbbr("STRING_PRINT_CHECKDPI"));
	//�ָ�Ĭ��
	ui.pushButton_Restore->setText(translate.GetlanguageAbbr("STRING_PRINT_RESTORE"));
	//����
	ui.pushButton_Help->setText(translate.GetlanguageAbbr("STRING_PRINT_HELP"));
	//ȷ��
	//ȡ��
}