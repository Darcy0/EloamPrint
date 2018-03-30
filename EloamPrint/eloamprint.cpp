#include "StdAfx.h"
#include<qformlayout.h>
#include<QtGui\qmovie.h>
#include <QMouseEvent>
#include<qfileinfo.h>
#include<qfiledialog.h>
#include<qdatetime.h>
#include<qfontdialog.h>
#include<qcolordialog.h>
#include<qtimer.h>
#include <QTextCodec>
#include <QTranslator>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdio.h>
#include <assert.h>
#include "atlconv.h"
#include<tchar.h>
#include<Windows.h>
#include "EloamPrint.h"
#include "language\language.h"
#include  <qfileinfo.h>
#include <qmessagebox.h>
#include  <qdir.h>
#include <qdatetime.h>
HWND g_hParent = NULL;
EloamPrint * g_TheDilgAPP = NULL;

#define FILE_EXISTS(FILE_NAME) ((0xFFFFFFFF==GetFileAttributes(FILE_NAME))?FALSE:TRUE)

EloamPrint::EloamPrint(QWidget *parent)
	: QWidget(parent)
	,m_hMainDev(NULL)
	,m_hMainVideo(NULL)
	,m_hMainView(NULL)
	,m_hThumbPrint(NULL)
	,m_hPrintPreviewShowImage(NULL)
	,m_nThresholdValue(40)
	,m_A3DefDpi(0)
	,m_A4DefDpi(0)
	,m_A4Width(0)
	,m_A4Height(0)
	,m_A3Width(0)
	,m_A3Height(0)
	,m_bIsVideoView(true)
	,m_ischangeDPI(0)					
{
	g_TheDilgAPP = this;
	ui.setupUi(this);
	this->setMouseTracking(true);
	SetButtonStyle();	//���ð�ť��ʽ
	SetLanguage();		//���ʻ���������
	//Ĭ�϶�ֵ����ֵ���ü����ɼ�
	ui.label_Threshold->setVisible(false);
	ui.slider_Threshold->setVisible(false);
	ui.spinBox_Threshold->setVisible(false);
	ui.pushButton_Cut->setVisible(false);
	ui.label_Cut->setVisible(false);
	
	ReadConfig();//��ȡ���ò���
	
	//���ö�ֵ����ֵ
	ui.slider_Threshold->setValue(m_nThresholdValue);
	ui.spinBox_Threshold->setValue(m_nThresholdValue);

	//���ý����ʼ��
	m_printConfigUi=new PrintConfig(this);
	m_printConfigUi->SetConfigParam(m_printConfigParam);
	connect(m_printConfigUi,SIGNAL(newPrintConfigParm(QVariant&)),this,SLOT(onNewPrintConfig(QVariant&)));
	connect(m_printConfigUi,SIGNAL(StandardizationRestore()),this,SLOT(onStandardizationRestore()));
	connect(m_printConfigUi,SIGNAL(StandardizationCheckDPI(CalibrationCard,bool)),this,SLOT(onStandardizationCheckDPI(CalibrationCard,bool)));

	//������������ͼ����
	m_hThumbPrint = EloamGlobal_CreateThumbnail((HWND)ui.listWidget_Thumbnail->winId(),0, 1);
	EloamThumbnail_SetMenuItem(m_hThumbPrint, THUMBNAIL_MENU_NULL, 0x01);
	EloamThumbnail_SetMenuItem(m_hThumbPrint, THUMBNAIL_MENU_FILE, THUMBNAIL_ITEM_OPENFILEPOS | THUMBNAIL_ITEM_BLEND
		| THUMBNAIL_ITEM_CREATEPDF |THUMBNAIL_ITEM_PRINT| THUMBNAIL_ITEM_DELETE | THUMBNAIL_ITEM_EDIT | THUMBNAIL_ITEM_PROPERTY
		);
	if(m_bLanguageIndex == 0)
	{
		EloamThumbnail_SetLanguage(m_hThumbPrint, 1);
	}
	else if(m_bLanguageIndex == 1)
	{
		EloamThumbnail_SetLanguage(m_hThumbPrint, 3);
	}	

	//������Ƶ����
	m_vDevices.clear();	
	HWND videoDlgHwnd  = (HWND)ui.widget_VideoDlg->winId();
	m_hMainView = EloamGlobal_CreateView(videoDlgHwnd ,NULL,0);
	EloamGlobal_InitDevs(DevChangeCallback,this);
	if (NULL==m_hMainDev)
	{
		EloamView_SetText(m_hMainView, L"û�������ź�...", RGB(255, 255, 255));
	}
	//�����źŲ�
	connect(ui.pushButton_Config, SIGNAL(clicked()), this, SLOT(OnConfigDlg()));//����
	connect(ui.pushButton_Cut, SIGNAL(clicked()), this, SLOT(OnCut()));//�����ü�
	connect(ui.pushButton_Capture, SIGNAL(clicked()), this, SLOT(OnCapture()));//����
	connect(ui.pushButton_SwitchView, SIGNAL(clicked()), this, SLOT(OnSwitchView()));//��ӡԤ��
	connect(ui.pushButton_Print,SIGNAL(clicked()),this,SLOT(OnPrintPic()));//��ӡ
	connect(ui.checkBox_SelectAll,SIGNAL(clicked()),this,SLOT(OnCbSelectAll()));//ͼƬȫѡ
	connect(ui.slider_Threshold, SIGNAL(valueChanged(int)), ui.spinBox_Threshold, SLOT(setValue(int)));
	connect(ui.spinBox_Threshold, SIGNAL(valueChanged(int)),ui.slider_Threshold, SLOT(setValue(int)));
	connect(ui.slider_Threshold, SIGNAL(valueChanged(int)),this, SLOT(OnThresholdValueChanged(int)));
	connect(ui.spinBox_Threshold, SIGNAL(valueChanged(int)),this, SLOT(OnThresholdValueChanged(int)));
	
	if(!m_ischangeDPI)
	{
		m_fCurXDpi = m_A4DefDpi;
		m_fCurYDpi = m_A4DefDpi;
		m_A4Width  = m_A4HA4WidthDef;
		m_A4Height = m_A4HA4HeightDef;
		m_A3Width  = m_A4HA3WidthDef;
		m_A3Height = m_A4HA3HeightDef;
	}
}

EloamPrint::~EloamPrint()
{
	if ( NULL != m_hPrintPreviewShowImage)//������ʾ��ֵ��Ч����ͼƬ����
	{
		EloamImage_Release(m_hPrintPreviewShowImage);
		m_hPrintPreviewShowImage = NULL;
	}
	if ( NULL != m_hThumbPrint)//����֤�ϳɵĵ�һ��ͼƬ
	{
		EloamThumbnail_Release(m_hThumbPrint);
		m_hThumbPrint = NULL;
	}
	if(m_hMainVideo)
	{
		EloamVideo_Release(m_hMainVideo);
		m_hMainVideo = NULL;
	}
	if(m_hMainView)
	{
		EloamView_Release(m_hMainView);
		m_hMainView = NULL;
	}
	int size = m_vDevices.size();
	for (int i = 0; i < size; i++)
	{
		HELOAMDEVICE hDev = m_vDevices[i];
		EloamDevice_Release(hDev);
	}
	m_vDevices.clear();
	EloamGlobal_DeinitDevs();
	WriteConfig();
}

void EloamPrint::SendMsgToParent(QString sFilePath)
{
	if(NULL != g_hParent)
	{
		COPYDATASTRUCT cds;
		cds.dwData = (unsigned long)this->winId();
		cds.cbData = sFilePath.size()*2+2;	
		cds.lpData = (PVOID)sFilePath.unicode();		
		::SendMessage(g_hParent, WM_COPYDATA, NULL, (LPARAM)&cds);
	}
}

void EloamPrint::SetLanguage()
{
	if (!translate.IsInitSuccess())
	{//���Գ�ʼ��ʧ��
		return;
	}
	setWindowTitle(translate.GetlanguageAbbr("STRING_DAYING"));//���ڱ���
	ui.label_Config->setText(translate.GetlanguageAbbr("STRING_PRINT_CONFIG"));
	ui.label_Cut->setText(translate.GetlanguageAbbr("STRING_PRINT_CUT"));	
	ui.label_Threshold->setText(translate.GetlanguageAbbr("STRING_PRINT_ADJUSTMENT"));
	ui.label_Capture->setText(translate.GetlanguageAbbr("STRING_PRINT_SCAN"));	
	ui.label_SwitchView->setText(translate.GetlanguageAbbr("STRING_DAYING_YULAN"));
	ui.label_Print->setText(translate.GetlanguageAbbr("STRING_DAYING"));
	ui.checkBox_SelectAll->setText(translate.GetlanguageAbbr("STRING_PRINT_SELECTALL"));
	//��ť��ʾ
	QString fontPre = QString::fromLocal8Bit("<font color='blue' size='5' weight ='200'>");
	QString fontBack = QString::fromLocal8Bit("</font>");
	ui.pushButton_Config->setToolTip(fontPre+translate.GetlanguageAbbr("STRING_PRINT_CONFIG")+fontBack); 
	ui.pushButton_Cut->setToolTip(fontPre+translate.GetlanguageAbbr("STRING_PRINT_CUT")+fontBack); 
	ui.pushButton_Capture->setToolTip(fontPre+translate.GetlanguageAbbr("STRING_PRINT_SCAN")+fontBack); 	
	ui.pushButton_SwitchView->setToolTip(fontPre+translate.GetlanguageAbbr("STRING_DAYING_YULAN")+fontBack); 
	ui.pushButton_Print->setToolTip(fontPre+translate.GetlanguageAbbr("STRING_DAYING")+fontBack); 
}

void EloamPrint::SetButtonStyle()
{
	ui.pushButton_Cut->setStyleSheet("QPushButton{border-image: url(:/EloamPrint/Resources/pic/pB_Cut.png);}"
		"QPushButton:hover{border-image: url(:/EloamPrint/Resources/pic/pB_Cut.png); background-color: rgb(255, 85, 127);}"
		"QPushButton:pressed{border-image: url(:/EloamPrint/Resources/pic/pB_Cut.png);  background-color: rgb(0, 170, 255);}");

	ui.pushButton_Config->setStyleSheet("QPushButton{border-image: url(:/EloamPrint/Resources/pic/paramset.png);}"
		"QPushButton:hover{border-image: url(:/EloamPrint/Resources/pic/paramset.png); background-color: rgb(255, 85, 127);}"
		"QPushButton:pressed{border-image: url(:/EloamPrint/Resources/pic/paramset.png);  background-color: rgb(0, 170, 255);}");

	ui.pushButton_Print->setStyleSheet("QPushButton{border-image: url(:/EloamPrint/Resources/pic/print.png);}"
		"QPushButton:hover{border-image: url(:/EloamPrint/Resources/pic/print.png); background-color: rgb(255, 85, 127);}"
		"QPushButton:pressed{border-image: url(:/EloamPrint/Resources/pic/print.png);  background-color: rgb(0, 170, 255);}");

	ui.pushButton_Capture->setStyleSheet("QPushButton{border-image: url(:/EloamPrint/Resources/pic/DnBar_scan.png);}"
		"QPushButton:hover{border-image: url(:/EloamPrint/Resources/pic/DnBar_scan.png); background-color: rgb(255, 85, 127);}"
		"QPushButton:pressed{border-image: url(:/EloamPrint/Resources/pic/DnBar_scan.png);  background-color: rgb(0, 170, 255);}");

	ui.pushButton_SwitchView->setStyleSheet("QPushButton{border-image: url(:/EloamPrint/Resources/pic/pB_Scrn.png);}"
		"QPushButton:hover{border-image: url(:/EloamPrint/Resources/pic/pB_Scrn.png); background-color: rgb(255, 85, 127);}"
		"QPushButton:pressed{border-image: url(:/EloamPrint/Resources/pic/pB_Scrn.png);  background-color: rgb(0, 170, 255);}");
}

void  EloamPrint::ReadConfig()
{
	QString configPath = QCoreApplication::applicationDirPath() + QString::fromLocal8Bit("/QDilgCameraConfig.ini");
	QFile file(configPath);
	if(file.exists())
	{
		QString strDefImageSavePath=QCoreApplication::applicationDirPath() + QString::fromLocal8Bit("/image");
		std::string strDef = strDefImageSavePath.toStdString(); //��QStringת��Ϊstring
		std::wstring wstrDef(strDef.length(), L' ');//��ʼ�����ֽ�wstr
		std::copy(strDef.begin(), strDef.end(), wstrDef.begin());//��str���Ƶ�wstr
		LPCWSTR DefPath = wstrDef.c_str();//��wstrת��ΪC�ַ�����ָ��,Ȼ��ֵ��path


		//ͼƬɨ��·��
		TCHAR buf[MAX_PATH] = {0};
		GetPrivateProfileString(_T("Print param"),_T("m_CurPrintImageSavePath"),DefPath,buf,MAX_PATH,configPath.toStdWString().c_str());
		m_printConfigParam.savePath =QString::fromWCharArray(buf);
		//����������ʽ
		m_iImgSaveType = GetPrivateProfileInt(_T("DigCamera driver param"), _T("m_iFileSaveType"), 0, configPath.toStdWString().c_str());
		//ͼƬ����
		m_iJPEGQulitVal = GetPrivateProfileInt(_T("DigCamera driver param"), _T("m_iJPEGQulitVal"), 0, configPath.toStdWString().c_str());
		//��ǰ��������
		m_bLanguageIndex = GetPrivateProfileInt(_T("DigCamera driver param"), _T("m_bLanguageIndex"), 0, configPath.toStdWString().c_str());
		//��ֵ����ֵ
		m_nThresholdValue = GetPrivateProfileInt(_T("Print param"), _T("m_nThresholdValue"), 0,configPath.toStdWString().c_str());
		//�ع�ֵ
		m_printConfigParam.isAutoExposure = (GetPrivateProfileInt(_T("Print param"), _T("m_nExposureValue"), 0,configPath.toStdWString().c_str())==1);//1�Զ��ع�
		m_printConfigParam.exposureValue = GetPrivateProfileInt(_T("Print param"), _T("m_nIsAutoExposure"), 0,configPath.toStdWString().c_str());//�Զ��ع�ֵ
		//X��YDPI
		m_fCurXDpi = GetPrivateProfileInt(_T("Print param"), _T("CurXDpi"), 0,configPath.toStdWString().c_str());
		m_fCurYDpi = GetPrivateProfileInt(_T("Print param"), _T("CurYDpi"), 0,configPath.toStdWString().c_str());
		//A4ֽ���ؿ���
		m_A4Width=GetPrivateProfileInt(_T("Print param"), _T("A4Width"), 0,configPath.toStdWString().c_str());
		m_A4Height=GetPrivateProfileInt(_T("Print param"), _T("A4Height"), 0,configPath.toStdWString().c_str());
		//A3ֽ���ؿ���
		m_A3Width=GetPrivateProfileInt(_T("Print param"), _T("A3Width"), 0,configPath.toStdWString().c_str());
		m_A3Height=GetPrivateProfileInt(_T("Print param"), _T("A3Height"), 0,configPath.toStdWString().c_str());
		//Ĭ��DPI
		m_A4DefDpi= GetPrivateProfileInt(_T("Print param"), _T("DefaultA4DPI"), 0, configPath.toStdWString().c_str());
		m_A3DefDpi= GetPrivateProfileInt(_T("Print param"), _T("DefaultA3DPI"), 0, configPath.toStdWString().c_str());
		//Ĭ��A4�߶���  A3�ߴ����س��� A4�ߴ����س���
		m_A4HA4WidthDef= GetPrivateProfileInt(_T("Print param"), _T("m_A4HA4WidthDef"), 0, configPath.toStdWString().c_str());;
		m_A4HA4HeightDef= GetPrivateProfileInt(_T("Print param"), _T("m_A4HA4HeightDef"), 0, configPath.toStdWString().c_str());;
		m_A4HA3WidthDef= GetPrivateProfileInt(_T("Print param"), _T("m_A4HA3WidthDef"), 0, configPath.toStdWString().c_str());;
		m_A4HA3HeightDef= GetPrivateProfileInt(_T("Print param"), _T("m_A4HA3HeightDef"), 0, configPath.toStdWString().c_str());;
		//Ĭ��A3�߶���  A3�ߴ����س��� A4�ߴ����س���
		m_A3HA4WidthDef= GetPrivateProfileInt(_T("Print param"), _T("m_A3HA4WidthDef"), 0, configPath.toStdWString().c_str());;
		m_A3HA4HeightDef= GetPrivateProfileInt(_T("Print param"), _T("m_A3HA4HeightDef"), 0, configPath.toStdWString().c_str());;
		m_A3HA3WidthDef= GetPrivateProfileInt(_T("Print param"), _T("m_A3HA3WidthDef"), 0, configPath.toStdWString().c_str());;
		m_A3HA3HeightDef= GetPrivateProfileInt(_T("Print param"), _T("m_A3HA3HeightDef"), 0, configPath.toStdWString().c_str());;
		//DPI�Ƿ����±궨��
		m_ischangeDPI = GetPrivateProfileInt(_T("Print param"), _T("m_ischangeDPI"), 0, configPath.toStdWString().c_str());

		//�Ƿ����þ�ƫ
		m_printConfigParam.isDeskew	= (GetPrivateProfileInt(_T("Print param"), _T("m_nIsDeskew"), 0, configPath.toStdWString().c_str())==1);//0��ʾ����ƫ,1��ʾ��ƫ
		//��ӡ�ߴ�
		m_printConfigParam.printSize =(PrintSize) GetPrivateProfileInt(_T("Print param"), _T("m_nPrintSize"), 0, configPath.toStdWString().c_str());//0��ʾA3,1��ʾA4
		//�Ƿ����������ϳ�	
		m_printConfigParam.isIdCard = (GetPrivateProfileInt(_T("Print param"), _T("m_nIsCombine"), 0, configPath.toStdWString().c_str())==1);//0��ʾ��������,1��ʾ���źϳ�
		//�Ƿ��ӡԭͼ
		m_printConfigParam.isOrignal = (GetPrivateProfileInt(_T("Print param"), _T("m_nIsOrignal"), 0, configPath.toStdWString().c_str())==1);//0��ʾ��ֵ��ͼ,1��ʾԭͼ
		//��ӡ����
		m_printConfigParam.printNum = GetPrivateProfileInt(_T("Print param"), _T("m_PrintNum"), 0, configPath.toStdWString().c_str());
	}

}

void  EloamPrint::WriteConfig()
{
	QString configPath = QCoreApplication::applicationDirPath() + QString::fromLocal8Bit("/QDilgCameraConfig.ini");
	QFile file(configPath);
	if(!file.exists())
	{
		FILE *fp = fopen(configPath.toStdString().c_str(),"wb");
		if(NULL != fp)
		{
			fclose(fp);
		}
	}
	QString para;
	//ͼƬɨ��·�� m_CurPrintImageSavePath
	para = m_printConfigParam.savePath;	
	WritePrivateProfileString(_T("Print param"), _T("m_CurPrintImageSavePath"), para.toStdWString().c_str(), configPath.toStdWString().c_str());

	//���û���Ĭ��dpi 1:1 ��ӡʹ��
	para = QString("%10").arg(m_nThresholdValue);
	WritePrivateProfileString(_T("Print param"), _T("m_nThresholdValue"), para.toStdWString().c_str(), configPath.toStdWString().c_str());
	//�ع�ֵ
	para = QString("%10").arg(m_printConfigParam.exposureValue);
	WritePrivateProfileString(_T("Print param"), _T("m_nExposureValue"), para.toStdWString().c_str(), configPath.toStdWString().c_str());
	//�Ƿ��Զ��ع�
	para = QString("%10").arg(m_printConfigParam.isAutoExposure?1:0);
	WritePrivateProfileString(_T("Print param"), _T("m_nIsAutoExposure"), para.toStdWString().c_str(), configPath.toStdWString().c_str());
	//m_CurXDpi
	para = QString("%10").arg(m_fCurXDpi);
	WritePrivateProfileString(_T("Print param"), _T("CurXDpi"), para.toStdWString().c_str(), configPath.toStdWString().c_str());
	//m_CurYDpi
	para = QString("%10").arg(m_fCurYDpi);
	WritePrivateProfileString(_T("Print param"), _T("CurYDpi"), para.toStdWString().c_str(), configPath.toStdWString().c_str());
	//A4ֽ���ؿ�
	para = QString("%10").arg(m_A4Width);
	WritePrivateProfileString(_T("Print param"), _T("A4Width"), para.toStdWString().c_str(), configPath.toStdWString().c_str());

	//A4ֽ���ظ�
	para = QString("%10").arg(m_A4Height);
	WritePrivateProfileString(_T("Print param"), _T("A4Height"), para.toStdWString().c_str(), configPath.toStdWString().c_str());

	//A3ֽ���ؿ�
	para = QString("%10").arg(m_A3Width);
	WritePrivateProfileString(_T("Print param"), _T("A3Width"), para.toStdWString().c_str(), configPath.toStdWString().c_str());

	//A3ֽ���ظ�
	para = QString("%10").arg(m_A3Height);
	WritePrivateProfileString(_T("Print param"), _T("A3Height"), para.toStdWString().c_str(), configPath.toStdWString().c_str());

	para = QString("%10").arg(m_A4DefDpi);
	WritePrivateProfileString(_T("Print param"), _T("DefaultA4DPI"), para.toStdWString().c_str(), configPath.toStdWString().c_str());
	para = QString("%10").arg(m_A3DefDpi);
	WritePrivateProfileString(_T("Print param"), _T("DefaultA3DPI"), para.toStdWString().c_str(), configPath.toStdWString().c_str());
	para = QString("%10").arg(m_ischangeDPI);
	WritePrivateProfileString(_T("Print param"), _T("m_ischangeDPI"), para.toStdWString().c_str(), configPath.toStdWString().c_str());

	para = QString("%10").arg(m_A4HA4WidthDef);
	WritePrivateProfileString(_T("Print param"), _T("m_A4HA4WidthDef"), para.toStdWString().c_str(), configPath.toStdWString().c_str());
	para = QString("%10").arg(m_A4HA4HeightDef);
	WritePrivateProfileString(_T("Print param"), _T("m_A4HA4HeightDef"), para.toStdWString().c_str(), configPath.toStdWString().c_str());
	para = QString("%10").arg(m_A4HA3WidthDef);
	WritePrivateProfileString(_T("Print param"), _T("m_A4HA3WidthDef"), para.toStdWString().c_str(), configPath.toStdWString().c_str());
	para = QString("%10").arg(m_A4HA3HeightDef);
	WritePrivateProfileString(_T("Print param"), _T("m_A4HA3HeightDef"), para.toStdWString().c_str(), configPath.toStdWString().c_str());
	para = QString("%10").arg(m_A3HA4WidthDef);
	WritePrivateProfileString(_T("Print param"), _T("m_A3HA4WidthDef"), para.toStdWString().c_str(), configPath.toStdWString().c_str());
	para = QString("%10").arg(m_A3HA4HeightDef);
	WritePrivateProfileString(_T("Print param"), _T("m_A3HA4HeightDef"), para.toStdWString().c_str(), configPath.toStdWString().c_str());
	para = QString("%10").arg(m_A3HA3WidthDef);
	WritePrivateProfileString(_T("Print param"), _T("m_A3HA3WidthDef"), para.toStdWString().c_str(), configPath.toStdWString().c_str());
	para = QString("%10").arg(m_A3HA3HeightDef);
	WritePrivateProfileString(_T("Print param"), _T("m_A3HA3HeightDef"), para.toStdWString().c_str(), configPath.toStdWString().c_str());

	para = QString("%1").arg(m_printConfigParam.isDeskew?1:0);
	WritePrivateProfileString(_T("Print param"), _T("m_nIsDeskew"), para.toStdWString().c_str(), configPath.toStdWString().c_str());
	para = QString("%1").arg((int)m_printConfigParam.printSize);
	WritePrivateProfileString(_T("Print param"), _T("m_nPrintSize"), para.toStdWString().c_str(), configPath.toStdWString().c_str());
	para = QString("%1").arg(m_printConfigParam.isIdCard?1:0);
	WritePrivateProfileString(_T("Print param"), _T("m_nIsCombine"), para.toStdWString().c_str(), configPath.toStdWString().c_str());
	para = QString("%1").arg(m_printConfigParam.isOrignal?1:0);
	WritePrivateProfileString(_T("Print param"), _T("m_nIsOrignal"), para.toStdWString().c_str(), configPath.toStdWString().c_str());
	para = QString("%10").arg(m_printConfigParam.printNum);
	WritePrivateProfileString(_T("Print param"), _T("m_PrintNum"), para.toStdWString().c_str(), configPath.toStdWString().c_str());
}

QString EloamPrint::GetImageSaveSuffix(int imgType)
{
	QString suffix="";
	switch(m_iImgSaveType)
	{
	case 0:
		suffix = QString::fromLocal8Bit(".bmp");
		break;
	case 1:
		suffix = QString::fromLocal8Bit(".jpg");
		break;
	case 2:
		suffix = QString::fromLocal8Bit(".png");
		break;
	case 3:
		suffix = QString::fromLocal8Bit(".tif");
		break;
	case 4:
		suffix = QString::fromLocal8Bit(".tga");
		break;
	case 5:
		suffix = QString::fromLocal8Bit(".pcx");
		break;
	case 6:
		suffix = QString::fromLocal8Bit(".ras");
		break;
	case 7:
		suffix = QString::fromLocal8Bit(".pdf");
		break;
	default:
		suffix = QString::fromLocal8Bit(".bmp");
	}
	return suffix;
}

QString EloamPrint::GetImageSaveFileName(QString flag/*=""*/)
{
	QString dateTime=QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
	QString saveFileName=m_printConfigParam.savePath + QString::fromLocal8Bit("/") + dateTime + flag + GetImageSaveSuffix(m_iImgSaveType);
	return saveFileName;
}

void EloamPrint::InitDeskew()
{
	if (NULL==m_hMainVideo)
	{//��Ƶ���󲻴���
		return;
	}
	if(m_printConfigParam.isDeskew)
	{//�ñ߾�ƫ
		EloamVideo_EnableDeskew(m_hMainVideo,0);
	}
}

void EloamPrint::InitExposure()
{
	if (NULL==m_hMainDev)
	{//�豸���󲻴���
		return;
	}
	int minExposure = EloamDevice_GetCameraControl(m_hMainDev,1,1);//�����ع�ֵ
	int maxExposure = EloamDevice_GetCameraControl(m_hMainDev,1,2);//����ع�ֵ
	bool isSuccess=m_printConfigUi->SetExposureValueRange(minExposure,maxExposure);
	if (!isSuccess)
	{
		return;
	}
	int exposureVal = EloamDevice_GetCameraControl(m_hMainDev,1,5);
	if (!m_printConfigUi->SetExposureValue(exposureVal))
	{
		return;
	}
	m_printConfigParam.exposureValue=exposureVal;
	if (m_printConfigParam.isAutoExposure)
	{
		EloamDevice_SetCameraControl(m_hMainDev, 0x01, 0, TRUE);
	}
	else
	{		
		EloamDevice_SetCameraControl(m_hMainDev, 0x01, m_printConfigParam.exposureValue, false);
		EloamDevice_SetCameraControl(m_hMainDev,0x04,0,false);
	}	
}

void EloamPrint::OpenVideo()
{		
	for(int i = 0;i< m_vDevices.size();i++)
	{	
		HELOAMDEVICE hDev = m_vDevices[i];
		if(1 == EloamDevice_GetEloamType(hDev))//��ȡ����ͷ���ͣ�1:������ͷ��2��3:������ͷ��-1ʧ��
		{	
			int subType = EloamDevice_GetSubtype(hDev);//1:֧��YUY2��ʽ��2:֧��MJPG��ʽ��3֧��YUY2��MJPG��ʽ
			int type=1;
			if( 0 != (subType & 2) )
			{//���Ȳ���MJPGģʽ��
				type = 2;
			}
			m_hMainVideo = EloamDevice_CreateVideo(hDev, 0, type, ArivlaCallback, NULL, EloamTouchCallback, this);
			if (m_hMainVideo) 
			{
				EloamView_SelectVideo(m_hMainView, m_hMainVideo, AttachCallback, this);  
				EloamView_SetText(m_hMainView, L"����Ƶ�У���ȴ�...", RGB(255, 255, 255));
				EloamView_SetCallback(m_hMainView, ZoomChange, this);					
				break;
			}			
		}
	}	
}

HELOAMIMAGE EloamPrint::ReversePrintImage(HELOAMIMAGE printImage)
{		
	if (NULL==printImage)
	{
		return NULL;
	}
	int printImgWidth = EloamImage_GetWidth(printImage);
	int printImgHeight = EloamImage_GetHeight(printImage);
	bool bResize = EloamImage_Resize(printImage,printImgWidth/1.038,printImgHeight/1.038,0);
	if (!bResize)
	{//��ӡͼƬ�ߴ�����ʧ�ܷ���ԭͼ
		return printImage;
	}

	HELOAMIMAGE newPrintImage=NULL;//�µĴ�ӡͼƬ
	int newPrintImgWidth=m_A4Width,newPrintImgHeight=m_A4Height;//�µĴ�ӡͼƬ�ߴ�,Ĭ��A4�ߴ�
	if (PrintSizeA3==m_printConfigParam.printSize)
	{//��ѡ�����A3��ƪ��
		newPrintImgWidth=m_A3Width;
		newPrintImgHeight=m_A3Height;
	}
	newPrintImage = EloamGlobal_CreateImage(newPrintImgWidth, newPrintImgHeight,3);	
	if (NULL==newPrintImage)
	{//�����µĴ�ӡͼƬʧ��
		return printImage;
	}
	//������ͼƬDpi
	EloamImage_SetXDPI(newPrintImage,m_fCurXDpi);
	EloamImage_SetYDPI(newPrintImage,m_fCurYDpi);
		
	HELOAMRECT rectDest = EloamGlobal_CreateRect((newPrintImgWidth-printImgWidth)/2+20, (newPrintImgHeight-printImgHeight)/2+20, newPrintImgWidth-40,printImgWidth-40);
	HELOAMRECT rectSrc = EloamGlobal_CreateRect(20, 20, printImgWidth-40, printImgHeight-40);
	EloamImage_Blend(newPrintImage, rectDest, printImage, rectSrc, 0, 0);
	EloamRect_Release(rectDest);
	EloamRect_Release(rectSrc);
	return newPrintImage;
}

void EloamPrint::OnStopPrint()
{
	QString m_printInfo = translate.GetlanguageAbbr("STRING_PRINT_TIPS");

	//m_bDeleteBk = false;
	EloamVideo_DisableDelBkColor(m_hMainVideo);
	//ui.m_DeleteNoise->setChecked(false);
	if(m_hMainView)
	{
		EloamView_Release(m_hMainView);
		m_hMainView=NULL;
	}
	m_printConfigParam.printNum=1;
	//m_PrintNum=1;
	
}

HELOAMIMAGE EloamPrint::GeneratePrintImage( HELOAMIMAGE hInputImg,PrintSize printSize )
{
	if (NULL==hInputImg)
	{
		return NULL;
	}
	//��ӡֽ�ߴ�
	int printWidth=m_A4Width;		//��ӡ���Ĭ��A4���
	int printHeight=m_A4Height;		//��ӡ�߶�Ĭ��A4�߶�
	if (PrintSizeA3==printSize)
	{//��ӡ�ߴ���A3ֽ
		printWidth=m_A3Width;		//��ӡ���A3���
		printHeight=m_A3Height;		//��ӡ�߶�A3�߶�
	}

	//ɫ��ͨ��
	int channel=1;//Ĭ��ɫ��ͨ������3,��Ϊ�Ҷ�ͼƬ
	if (m_printConfigParam.isOrignal)
	{//��ɫͼƬ
		channel=3;
	}

	//��ӡͼƬ�ߴ�
	int inputImgWidth=EloamImage_GetWidth(hInputImg);
	int inputImgHeight=EloamImage_GetHeight(hInputImg);
	if(EloamImage_Resize(hInputImg,inputImgWidth*1.038,inputImgHeight*1.038,0))
	{//���ݾ���ֵ����������ͼƬ�ߴ�
		inputImgWidth = EloamImage_GetWidth(hInputImg);
		inputImgHeight = EloamImage_GetHeight(hInputImg);
	}

	//����ͼƬ����
	if (inputImgHeight>inputImgWidth)
	{//ͼƬ���ţ�����ֽ�ų�����
		int temp=printWidth;
		printWidth=printHeight;
		printHeight=printWidth;
	}

	//����Ŀ�ġ�Դͼ���������Χ��������ӡͼƬ
	HELOAMRECT rectDest=NULL;		//Ŀ�ľ�������
	HELOAMRECT rectSrc=NULL;		//Դ��������
	HELOAMIMAGE hPrintImage=NULL;	//��ӡ��ͼƬ
	hPrintImage=EloamGlobal_CreateImage(printWidth, printHeight,channel);
	int rectDestTop=0,rectDestLeft=0,rectDestWidth=0,rectDestHeight=0;
	int rectSrcTop=0,rectSrcLeft=0,rectSrcWidth=0,rectSrcHeight=0;
	//�����
	if (inputImgWidth>=printWidth)
	{//ͼƬ�����ֽ�ſ�
		rectDestLeft=0;
		rectDestWidth=printWidth;
		rectSrcLeft=(inputImgWidth-printWidth)/2;
		rectSrcWidth=printWidth;
	}
	else
	{//ֽ�ſ����ͼƬ��
		rectDestLeft=(printWidth-inputImgWidth)/2;
		rectDestWidth=inputImgWidth;
		rectSrcLeft=0;
		rectSrcWidth=inputImgWidth;
	}
	//�����
	if (inputImgHeight>=printHeight)
	{//ͼƬ�ߴ���ֽ�Ÿ�
		rectDestTop=0;
		rectDestHeight=printHeight;
		rectSrcTop=(inputImgHeight-printHeight)/2;
		rectSrcHeight=printHeight;
	}
	else
	{//ֽ�Ÿߴ���ͼƬ��
		rectDestTop=(printHeight-inputImgHeight)/2;
		rectDestHeight=inputImgHeight;
		rectSrcTop=0;
		rectSrcHeight=inputImgHeight;
	}
	rectDest = EloamGlobal_CreateRect(rectDestLeft,rectDestTop,rectDestWidth,rectDestHeight);
	rectSrc = EloamGlobal_CreateRect(rectSrcLeft,rectSrcTop,rectSrcWidth,rectSrcHeight);
	if (NULL==hPrintImage)
	{
		return NULL;
	}
	EloamImage_SetXDPI(hPrintImage,m_fCurXDpi);
	EloamImage_SetYDPI(hPrintImage,m_fCurYDpi);
	EloamImage_Blend(hPrintImage, rectDest, hInputImg, rectSrc, 0, 0);
	EloamRect_Release(rectDest);
	EloamRect_Release(rectSrc);
	return hPrintImage;		
}

HELOAMIMAGE EloamPrint::ImageCombine( HELOAMIMAGE hImg1,HELOAMIMAGE hImg2,int space/*=0*/,bool isVertical/*=true*/ )
{
	HELOAMIMAGE combineImg=NULL;
	if (NULL==hImg1||NULL==hImg2)
	{//�ϳ���Ƭ����ȫ����
		return combineImg;
	}
	if (1==EloamImage_GetChannels(hImg1)||1==EloamImage_GetChannels(hImg2))
	{//ֻ�ϳɲ�ɫͼƬ
		return combineImg;
	}	
	LONG hImg1Width = EloamImage_GetWidth(hImg1);
	LONG hImg2Width = EloamImage_GetWidth(hImg2);
	LONG hImg1Height = EloamImage_GetHeight(hImg1);
	LONG hImg2Height = EloamImage_GetHeight(hImg2);

	LONG combineImgWidth=0,combineImgHeight=0;
	if (isVertical)
	{//����ϳ�
		combineImgWidth=(hImg1Width>hImg2Width)?hImg1Width:hImg2Width;
		combineImgHeight=hImg1Height+hImg2Height+space;
	}
	else
	{//����ϳ�
		combineImgWidth=hImg1Width+hImg2Width+space;
		combineImgHeight=(hImg1Height>hImg2Height)?hImg1Height:hImg2Height;
	}
	combineImg = EloamGlobal_CreateImage(combineImgWidth, combineImgHeight, 3);//�����ϳ�ͼƬ
	if (NULL==combineImg)
	{//�����ϲ�ͼƬʧ��
		return combineImg;
	}
	HELOAMRECT rectSrc1=EloamGlobal_CreateRect(0,0,hImg1Width,hImg1Height);
	HELOAMRECT rectSrc2 = EloamGlobal_CreateRect(0, 0, hImg2Width, hImg2Height);
	HELOAMRECT rectDest1 = EloamGlobal_CreateRect(0, 0, hImg1Width, hImg1Height);
	HELOAMRECT rectDest2=isVertical?EloamGlobal_CreateRect(0, hImg1Height+space, hImg2Width, hImg2Height)
		:EloamGlobal_CreateRect(hImg1Width+space, 0, hImg2Width, hImg2Height);
	//�ϳ�
	EloamImage_Blend(combineImg, rectDest1, hImg1, rectSrc1, 0, 0);
	EloamImage_Blend(combineImg, rectDest2, hImg2, rectSrc2, 0, 0);
	//�ͷźϳ�����
	EloamRect_Release(rectDest1);
	EloamRect_Release(rectSrc1);
	EloamRect_Release(rectDest2);
	EloamRect_Release(rectSrc2);
	return combineImg;
}


//����
void EloamPrint::OnConfigDlg()
{
	if (m_printConfigUi!=NULL)
	{
		m_printConfigUi->show();
	}
}
//�����ü�
void EloamPrint::OnCut()
{
	if (NULL==m_hPrintPreviewShowImage)
	{//��ӡԤ��ԭʼͼ�񲻴���
		return;
	}
	if(!m_bIsCut)
	{
		m_bIsCut= true;
		EloamView_SetState(m_hMainView, 2);//���ô���״̬Ϊ�Զ���
		ui.label_Cut->setText(translate.GetlanguageAbbr("STRING_PRINT_SURECUT"));
		ui.pushButton_Cut->setToolTip(translate.GetlanguageAbbr("STRING_PRINT_SURECUT"));
	}
	else
	{
		ui.label_Cut->setText(translate.GetlanguageAbbr("STRING_PRINT_CUT"));
		ui.pushButton_Cut->setToolTip(translate.GetlanguageAbbr("STRING_PRINT_CUT"));
		
		HELOAMRECT hSelectRect=EloamView_GetSelectedRect(m_hMainView);
		if (NULL==hSelectRect)
		{
			m_bIsCut= false;
			EloamView_SetSelectedRect(m_hMainView, NULL);//����ѡ������NULL
			EloamView_SetState(m_hMainView, 1);	
			return;
		}
		bool isCrop=EloamImage_Crop(m_hPrintPreviewShowImage,hSelectRect);
		if (!isCrop)
		{
			return;
		}
		EloamView_SelectImage(m_hMainView,m_hPrintPreviewShowImage);
		QString imgPath=GetImageSaveFileName();
		HELOAMIMAGE printImg=GeneratePrintImage(m_hPrintPreviewShowImage,m_printConfigParam.printSize);
		if (printImg)
		{
			if (EloamImage_Save(printImg, (BSTR)(LPCWSTR)imgPath.data(), m_iJPEGQulitVal))
			{
				EloamThumbnail_Add(m_hThumbPrint,(BSTR)(LPCWSTR)imgPath.data());
			}
			EloamImage_Release(printImg);
			printImg = NULL;
		}
		
		//�ͷ��������
		EloamRect_Release(hSelectRect);
		hSelectRect= NULL;
		m_bIsCut= false;
		EloamView_SetSelectedRect(m_hMainView, NULL);
		EloamView_SetState(m_hMainView, 1);	
	}
}
//����
void EloamPrint::OnCapture()
{
	HELOAMIMAGE printImg=NULL;//��ӡ��ͼƬ
	QString imgPath=GetImageSaveFileName();//ͼƬ����·��
	
	HELOAMIMAGE firstImg = EloamVideo_CreateImage(m_hMainVideo, 0, m_hMainView);//��ǰ��Ƭ
	if (NULL==firstImg)
	{//��ȡ��һ����Ƭʧ��
		return;
	}
	EloamGlobal_PlayCaptureVoice();//�������յ�����
	EloamView_PlayCaptureEffect(m_hMainView);//�������ն���Ч��

	if (!m_printConfigParam.isIdCard)
	{//�����֤����ģʽ
		if(!m_printConfigParam.isOrignal)
		{
			EloamImage_AdaptiveThreshold(firstImg,m_nThresholdValue);
		}   
		printImg=GeneratePrintImage(firstImg,m_printConfigParam.printSize);
	}
	else
	{//֤������ģʽ		
		QString tip = translate.GetlanguageAbbr("STRING_PRINT_TIPS");//��ʾ:		
		QString tipIdCardCombineStart = translate.GetlanguageAbbr("STRING_PRINT_IDCARDCOMBINE_START");//֤�������ϳɿ�ʼ��		
		QString tipIdCardCombineEnd =translate.GetlanguageAbbr("STRING_PRINT_IDCARDCOMBINE_END");//֤�������ϳ�����ɣ�
		QMessageBox::information(this,tip,tipIdCardCombineStart);//��ʾ֤�������ϳɿ�ʼ	
		EloamView_SetMessage(m_hMainView,(BSTR)(LPCWSTR)(translate.GetlanguageAbbr("STRING_PRINT_OTHERHAND").data()));//��ʾ��ҳ
		HELOAMIMAGE secondImg=EloamVideo_CreateImage(m_hMainVideo, 0, m_hMainView);//�ٻ�ȡһֱ��Ƭ
		if (NULL==secondImg)
		{
			return;
		}
		EloamGlobal_PlayCaptureVoice();//�������յ�����
		EloamView_PlayCaptureEffect(m_hMainView);//�������ն���Ч��
		HELOAMIMAGE combineImg=ImageCombine(firstImg,secondImg);
		if (NULL==combineImg)
		{
			return;
		}
		if(!m_printConfigParam.isOrignal)
		{
			EloamImage_AdaptiveThreshold(combineImg,m_nThresholdValue);
		} 
		printImg=GeneratePrintImage(combineImg,m_printConfigParam.printSize);
		if (printImg)
		{
			QMessageBox::information(this,tip,tipIdCardCombineEnd);
		}
	}
	if (EloamImage_Save(printImg, (BSTR)(LPCWSTR)imgPath.data(), m_iJPEGQulitVal))
	{//������Ҫ��ӡͼƬ
		
		EloamThumbnail_Add(m_hThumbPrint,(BSTR)(LPCWSTR)imgPath.data());
		SendMsgToParent(imgPath);
	}
	if (NULL!=printImg)
	{
		EloamImage_Release( printImg );
		printImg = NULL;
	}
}
//��ӡԤ������ƵԤ���л�
void EloamPrint::OnSwitchView()            //�л���Ƶ����
{	
	m_bIsVideoView=!m_bIsVideoView;
	if(m_bIsVideoView)
	{//��ƵԤ��	
		ui.label_Threshold->setVisible(false);
		ui.slider_Threshold->setVisible(false);
		ui.spinBox_Threshold->setVisible(false);
		ui.pushButton_Cut->setVisible(false);
		ui.label_Cut->setVisible(false);
		ui.pushButton_Capture->setVisible(true);
		ui.label_Capture->setVisible(true);

		ui.label_SwitchView->setText(translate.GetlanguageAbbr("STRING_DAYING_YULAN"));
		ui.pushButton_SwitchView->setToolTip(translate.GetlanguageAbbr("STRING_DAYING_YULAN"));

		EloamView_SelectVideo(m_hMainView,m_hMainVideo,NULL,this);
		m_bIsCut = false;
		ui.pushButton_Cut->setToolTip(translate.GetlanguageAbbr("STRING_PRINT_CUT"));
	
		if(m_hPrintPreviewShowImage != NULL)
		{
			EloamImage_Release(m_hPrintPreviewShowImage);
			m_hPrintPreviewShowImage = NULL; 
		}
	}
	else
	{//��ӡԤ��
		m_bIsCut=false;
		//������ؿؼ���ʾ������
		if (!m_printConfigParam.isOrignal)
		{
			ui.label_Threshold->setVisible(true);
			ui.slider_Threshold->setVisible(true);
			ui.spinBox_Threshold->setVisible(true);
		}
		ui.label_Cut->setVisible(true);
		ui.pushButton_Cut->setVisible(true);
		ui.label_Capture->setVisible(false);
		ui.pushButton_Capture->setVisible(false);
		//������ؿؼ���������
		ui.label_SwitchView->setText(translate.GetlanguageAbbr("STRING_PRINT_VIDEOPREVIEW"));
		ui.pushButton_SwitchView->setToolTip(translate.GetlanguageAbbr("STRING_PRINT_VIDEOPREVIEW"));	
		ui.label_Cut->setText(translate.GetlanguageAbbr("STRING_PRINT_CUT"));
		
		
		if(NULL!=m_hPrintPreviewShowImage)
		{
			EloamImage_Release(m_hPrintPreviewShowImage);
			m_hPrintPreviewShowImage = NULL; 
		}

		m_sPrintPreviewOriImagePath = GetImageSaveFileName("ori");
		m_hPrintPreviewShowImage = EloamVideo_CreateImage(m_hMainVideo,0,m_hMainView);
		EloamImage_Save(m_hPrintPreviewShowImage,(BSTR)(LPCWSTR)m_sPrintPreviewOriImagePath.data(),0);//����ԭʼͼ��
		
		if(!m_printConfigParam.isOrignal)
		{
			EloamImage_AdaptiveThreshold(m_hPrintPreviewShowImage,m_nThresholdValue); 			
		}
		HELOAMIMAGE printImg = GeneratePrintImage(m_hPrintPreviewShowImage,m_printConfigParam.printSize);
		QString imgPath = GetImageSaveFileName();	
		if (EloamImage_Save(printImg, (BSTR)(LPCWSTR)imgPath.data(), m_iJPEGQulitVal))
		{
			EloamThumbnail_Add(m_hThumbPrint,(BSTR)(LPCWSTR)imgPath.data());
			SendMsgToParent(imgPath);
		}
		EloamImage_Release( printImg );
		printImg = NULL;
		EloamView_SelectImage(m_hMainView,m_hPrintPreviewShowImage);	
	}
}
//��ӡ
void EloamPrint::OnPrintPic()										
{    
	HELOAMIMAGE printImg=NULL;
	if(!m_bIsVideoView)
	{//�����ǰΪ��ӡԤ��״̬��ֱ�Ӵ�ӡ��ǰԤ����ͼƬ
		HELOAMIMAGE newPrintImage=ReversePrintImage(m_hPrintPreviewShowImage);
		EloamImage_AdaptivePrintByDPI(newPrintImage,NULL);	
		EloamImage_Release(printImg);		
		EloamImage_Release(newPrintImage);
		newPrintImage = NULL;
		printImg = NULL;
	}
	else
	{//��ƵԤ��ʱ		
		if (m_printConfigParam.printNum<=0)
		{//��ӡ����Ϊ0ʱ
			return;
		}
		QString tip = translate.GetlanguageAbbr("STRING_PRINT_TIPS");//��ʾ
		QString tipNoSelect =  translate.GetlanguageAbbr("STRING_PRINT_CHECK_SELECTED");//�����Ƿ�ѡ��ͼƬ
		HELOAMIMAGELIST imgList = EloamGlobal_CreateImageList();	
		if (NULL==imgList)
		{//����ͼ���б�ʧ��
			return;
		}
		int count = EloamThumbnail_GetCount(m_hThumbPrint);
		for (int i = 0; i < count; i++)
		{
			if(EloamThumbnail_GetCheck(m_hThumbPrint,i))
			{//�����ѡ
				BSTR imagePath = EloamThumbnail_GetFileName(m_hThumbPrint,i);
				HELOAMIMAGE tempImage = EloamGlobal_CreateImageFromFile(imagePath, 0);
				EloamImageList_Add(imgList, tempImage);
				EloamImage_Release(tempImage);
			}
		}
		int listCount =EloamImageList_GetCount(imgList);
		if (listCount<=0)
		{
			QMessageBox::information(this,tip,tipNoSelect);	
			return;
		}
		for(int i=0;i<listCount;i++)
		{
			//��ԭ�����Ŵ��ͼƬ
			if(NULL != printImg)
			{
				EloamImage_Release(printImg);
				printImg = NULL;
			}
			printImg = EloamImageList_GetImage(imgList,i);
			HELOAMIMAGE newPrintImg=ReversePrintImage(printImg);
			if (NULL==newPrintImg)
			{
				continue;
			}
			for(int i = 0 ; i < m_printConfigParam.printNum ; i++)
			{
				EloamImage_AdaptivePrintByDPI(newPrintImg,NULL);	
			}
			EloamImage_Release(newPrintImg);
			newPrintImg = NULL;
		}
		//�ͷ�ͼ���б�
		EloamImageList_Release(imgList);
		imgList = NULL;
	}
}
//ͼƬѡȫ
void EloamPrint::OnCbSelectAll()
{
	bool setCheck = ui.checkBox_SelectAll->isChecked();
	int count = EloamThumbnail_GetCount(m_hThumbPrint);
	for (int i = 0; i < count; i++)
	{
		EloamThumbnail_SetCheck(m_hThumbPrint,i,setCheck);
	}
}
//���ö�ֵ����ֵ
void EloamPrint::OnThresholdValueChanged(int Value)
{
	if (m_sPrintPreviewOriImagePath.isEmpty())
	{
		return;
	}
	m_nThresholdValue = ui.slider_Threshold->value();
	if(m_hPrintPreviewShowImage != NULL)
	{
		EloamImage_Release(m_hPrintPreviewShowImage);
		m_hPrintPreviewShowImage = NULL;
	}
	m_hPrintPreviewShowImage = EloamGlobal_CreateImageFromFile((BSTR)(LPCWSTR)m_sPrintPreviewOriImagePath.data(),0);
	EloamImage_AdaptiveThreshold(m_hPrintPreviewShowImage,m_nThresholdValue);

	HELOAMIMAGE cutImg=GeneratePrintImage(m_hPrintPreviewShowImage,m_printConfigParam.printSize);
	if (cutImg)
	{
		EloamImage_Save(cutImg, (BSTR)(LPCWSTR)m_sCutImagePath.data(), m_iJPEGQulitVal);
		EloamImage_Release(cutImg);
		cutImg = NULL;
	}
	EloamView_SelectImage(m_hMainView,m_hPrintPreviewShowImage);
}
//��Ӧ���ò����ı�
void EloamPrint::onNewPrintConfig( QVariant& newParm )
{
	PrintConfigParam configparm=newParm.value<PrintConfigParam>();  
	if(m_printConfigParam.isDeskew!=configparm.isDeskew)
	{//�Զ�����
		if(configparm.isDeskew)
		{
			EloamVideo_EnableDeskew(m_hMainVideo,true);
		}
		else
		{	
			EloamVideo_DisableDeskew(m_hMainVideo);
		}
		m_printConfigParam.isDeskew=configparm.isDeskew;
	}
	if(m_printConfigParam.printSize != configparm.printSize)
	{
		m_printConfigParam.printSize = configparm.printSize;
		if (PrintSizeA4==m_printConfigParam.printSize)
		{
			if(m_ischangeDPI == 0)
			{
				m_fCurXDpi = m_A4DefDpi;
				m_fCurYDpi = m_A4DefDpi;
				m_A4Width = m_A4HA4WidthDef;
				m_A4Height =  m_A4HA4HeightDef;
				m_A3Width = m_A4HA3WidthDef;
				m_A3Height = m_A4HA3HeightDef;
			}
		}
		else if (PrintSizeA3==m_printConfigParam.printSize)
		{
			if(m_ischangeDPI == 0)
			{
				m_fCurXDpi = m_A3DefDpi;
				m_fCurYDpi = m_A3DefDpi;
				m_A4Width = m_A3HA4WidthDef;
				m_A4Height= m_A3HA4HeightDef;
				m_A3Width = m_A3HA3WidthDef;
				m_A3Height = m_A3HA3HeightDef;
			}
		}
	}
	if(m_printConfigParam.isIdCard != configparm.isIdCard)
	{
		m_printConfigParam.isIdCard = configparm.isIdCard;
	}
	if(m_printConfigParam.isOrignal != configparm.isOrignal)
	{
		m_printConfigParam.isOrignal = configparm.isOrignal;
	}
	if(m_printConfigParam.exposureValue != configparm.exposureValue)
	{//�ع�ֵ
		m_printConfigParam.exposureValue = configparm.exposureValue;
	}
	if (m_printConfigParam.isAutoExposure!=configparm.isAutoExposure)
	{//�Զ��ع�
		m_printConfigParam.isAutoExposure=configparm.isAutoExposure;
		if (m_printConfigParam.isAutoExposure)
		{
			EloamDevice_SetCameraControl(m_hMainDev, 0x01, 0, TRUE);
		}
		else
		{
			EloamDevice_SetCameraControl(m_hMainDev, 0x01, m_printConfigParam.exposureValue, false);
		}
	}
	if (m_printConfigParam.printNum != configparm.printNum)
	{//��ӡ����
		m_printConfigParam.printNum = configparm.printNum;
	}
	if (m_printConfigParam.savePath!=configparm.savePath)
	{//����·��
		m_printConfigParam.savePath=configparm.savePath;
	}
	if (m_printConfigParam.calibrationCard!=configparm.calibrationCard)
	{//�궨���ߴ�
		m_printConfigParam.calibrationCard=configparm.calibrationCard;
	}
}
//��Ӧ�ָ�Ĭ�������ź�
void EloamPrint::onStandardizationRestore()
{
	m_ischangeDPI = 0;
	if(PrintSizeA3==m_printConfigParam.printSize)
	{
		m_fCurXDpi=m_A3DefDpi;
		m_fCurYDpi=m_A3DefDpi;
		m_A4Width  =  m_A3HA4WidthDef;
		m_A4Height =  m_A3HA4HeightDef;
		m_A3Width =  m_A3HA3WidthDef;
		m_A3Height = m_A3HA3HeightDef;

	}
	else if(PrintSizeA4==m_printConfigParam.printSize)
	{
		m_fCurXDpi=m_A4DefDpi;
		m_fCurYDpi=m_A4DefDpi;
		m_A4Width  =  m_A4HA4WidthDef;
		m_A4Height =  m_A4HA4HeightDef;
		m_A3Width  = m_A4HA3WidthDef;
		m_A3Height = m_A4HA3HeightDef;
	}
}
//��Ӧ�궨�ź�
void EloamPrint::onStandardizationCheckDPI( CalibrationCard caliCardSize,bool isDeskew)
{
	QString tip = translate.GetlanguageAbbr("STRING_PRINT_TIPS");//��ʾ
	QString tipPutA4Card = translate.GetlanguageAbbr("STRING_PRINT_PUTA4_CARD");//��A4ֽ
	QString tipPutA5Card = translate.GetlanguageAbbr("STRING_PRINT_PUTA5_CARD");//��A5ֽ
	QString tipOutRange = translate.GetlanguageAbbr("STRING_PRINT_CHECKDPI_OUTRANGE");//������Χ
	QString tipComplete = translate.GetlanguageAbbr("STRING_PRINT_CHECKDPI_COMPLETE");//���
	float caliCardWidth=A4Width;//Ĭ�ϱ궨������A4ֽ���
	float caliCardHeight=A4Height;//Ĭ�ϱ궨��߶���A4ֽ�߶�
	if (CalibrationCardA5==caliCardSize)
	{//����궨����A5�ߴ�
		caliCardWidth=A5Width;
		caliCardHeight=A5Height;
	}
	//��ʾ�ű궨��
	if (CalibrationCardA4 == caliCardSize)
	{
		QMessageBox::information(m_printConfigUi,tip,tipPutA4Card);	
	}
	else if (CalibrationCardA5 == caliCardSize)
	{
		QMessageBox::information(m_printConfigUi,tip,tipPutA5Card);	
	}
	HELOAMIMAGE hImg = EloamVideo_CreateImage(m_hMainVideo, 0, m_hMainView);

	int imgWidth=EloamImage_GetWidth(hImg);
	int imgHeight=EloamImage_GetHeight(hImg);
	if (imgWidth<imgHeight)
	{//��ȸ�С���������
		int temp=imgWidth;
		imgWidth=imgHeight;
		imgHeight=imgWidth;
	}
	if (isDeskew)
	{
		imgWidth+=18;
		imgHeight+=22;
	}
	m_fCurXDpi=imgWidth/caliCardWidth*Inch;
	m_fCurYDpi=imgHeight/caliCardHeight*Inch;
	if (CalibrationCardA4 == caliCardSize)
	{
		m_A4Width = imgWidth;
		m_A4Height = imgHeight;
		m_A3Width = m_A4Height*2;
		m_A3Height = m_A4Width;
	}
	else if (CalibrationCardA5==caliCardSize)
	{
		m_A4Width = imgHeight*2;
		m_A4Height = imgWidth;
		m_A3Width = m_A4Height*2;
		m_A3Height = m_A4Width;
	}
	//if(!(((m_CurXDpi<(m_A4DefDpi+10)) && (m_CurXDpi>(m_A4DefDpi-10)) && (m_CurYDpi<(m_A4DefDpi+10)) && (m_CurYDpi>(m_A4DefDpi-10)))||((m_CurXDpi<(m_A3DefDpi+10)) && (m_CurXDpi>(m_A3DefDpi-10)) && (m_CurYDpi<(m_A3DefDpi+10)) && ( m_CurYDpi>(m_A3DefDpi-10)))))
	//{
	//	onStandardizationRestore();//�궨ʧ�ܣ��궨�����ָ�Ĭ��ֵ
	//	QMessageBox::information(m_printConfigUi,tip,tipOutRange);
	//	return;
	//}
	m_ischangeDPI = 1;			//�궨�ɹ�
	EloamImage_Release(hImg);
	hImg = NULL;
	QMessageBox::information(m_printConfigUi,tip,tipComplete);
}

//�̳���д�麯��
void EloamPrint::closeEvent(QCloseEvent *event)
{

}

//�豸�ص�����
VOID ELOAMAPI EloamPrint::DevChangeCallback(LONG type, LONG idx, LONG dbt, LPVOID userData) 
{//type:1=��Ƶ�豸��2=��Ƶ�豸;idx=�豸����;dbt:1=�豸���2�豸��ʧ;userData=�û�����
	if(1 != type)
	{//������Ƶ����
		return;
	}		
	EloamPrint * pDlg = (EloamPrint *)userData;
	if(1 == dbt)
	{//�豸����
		HELOAMDEVICE hDev = EloamGlobal_CreateDevice(1,idx);	
		pDlg->m_vDevices.push_back(hDev);
		if(1 == EloamDevice_GetEloamType(hDev))
		{//���������Ƶ,����Ƶ
			pDlg->m_hMainDev=hDev;
			pDlg->OpenVideo();
		}
	}
	else if(2 == dbt)
	{//�豸��ʧ
		for (int i=0;i<pDlg->m_vDevices.size();i++)
		{
			int devIndex=EloamDevice_GetIndex(pDlg->m_vDevices[i]);
			if (-1==devIndex||devIndex==idx)
			{//�豸��ʧ
				HELOAMDEVICE hMainDev = EloamVideo_GetDevice(pDlg->m_hMainVideo);
				if (pDlg->m_vDevices[i]==hMainDev)
				{
					EloamView_SetText(pDlg->m_hMainView, L"û�������ź�...", 0);
					//�ͷ�����Ƶ
					EloamVideo_Release(pDlg->m_hMainVideo);
					pDlg->m_hMainVideo = NULL;
					//������Ƶ�豸����ΪNULL
					pDlg->m_hMainDev=NULL;
				}
				//�ͷŰε����豸
				EloamDevice_Release(pDlg->m_vDevices[i]);
				pDlg->m_vDevices.erase(pDlg->m_vDevices.begin() + i);
			}
		}
	}
}

VOID ELOAMAPI EloamPrint::AttachCallback(HELOAMVIDEO video, LONG videoId, HELOAMVIEW view, LONG viewId, LPVOID userData)
{//������Ƶ�򿪺�����
	EloamPrint *pDlg = (EloamPrint *)userData;
	if (1 == videoId && 1 == viewId)
	{	
		g_TheDilgAPP->InitDeskew();	
	}	
}

VOID ELOAMAPI EloamPrint::ArivlaCallback(HELOAMVIDEO video, LONG videoId, LPVOID userData)
{	
	if (1 == videoId)
	{//��Ƶ֡����	
		g_TheDilgAPP->InitExposure();	//��ʼ���ع�ֵ
		g_TheDilgAPP->InitDeskew();		//��ʼ���Զ�����
	}	
}

VOID ELOAMAPI EloamPrint::EloamTouchCallback(HELOAMVIDEO video, LPVOID userData)
{
	
}

VOID ELOAMAPI EloamPrint::ZoomChange(HELOAMVIEW view, LONG flag, LONG value, LPVOID userData)
{	
	//��ƵԤ�����ڴ�С�����ı�ʱ�ص�
	EloamPrint *pDlg = (EloamPrint *)userData;
	if(NULL != pDlg)
	{
		if(flag == 2)
		{
		}		
	}	
}


//void EloamPrint::OnCut()
//{
//	QString strLid;
//	QString strName;
//	if(false == m_bIsCut)
//	{
//		EloamView_SetState(m_PrintView, 2);
//		m_bIsCut= true;
//		//ui.pB_Cut->setText(QString::fromLocal8Bit("ȷ�ϲü�"));
//		strLid ="STRING_PRINT_SURECUT";
//		strName = getlanguageAbbr(strLid);
//		ui.label_cut->setText(strName);
//		ui.pB_Cut->setToolTip(strName);
//
//	}
//	else
//	{
//		strLid ="STRING_PRINT_CUT";
//		strName = getlanguageAbbr(strLid);
//		ui.label_cut->setText(strName);
//		ui.pB_Cut->setToolTip(strName);
//		HELOAMRECT m_hCutSelectRect=EloamView_GetSelectedRect(m_PrintView);
//		int Sy=EloamRect_GetTop(m_hCutSelectRect);
//		int Sx=EloamRect_GetLeft(m_hCutSelectRect);
//		int Sw = EloamRect_GetWidth(m_hCutSelectRect);
//		int Sh= EloamRect_GetHeight(m_hCutSelectRect);
//
//		int Vw = EloamVideo_GetWidth(m_hMainVideo);
//		int Vh = EloamVideo_GetHeight(m_hMainVideo);
//
//		if(NULL!=m_hCutSelectRect)
//		{
//			if(m_sShowResultImagePath != NULL)
//			{
//				//����ʾԤ��Ч����ͼƬ���м��ã������Ǳ��棬�ٽ��ж�ֵ������ʱ�Լ��ú��Ϊ����
//				if(NULL != m_hCutImage )
//				{
//					EloamImage_Release(m_hCutImage);
//					m_hCutImage =NULL;
//				}
//				m_hCutImage = EloamGlobal_CreateImageFromFile((BSTR)(LPCWSTR)m_sShowResultImagePath.data(),0);
//				if(NULL == m_hCutRect)
//				{
//
//					int imgW = EloamImage_GetWidth(m_hCutImage);
//					int imgH = EloamImage_GetHeight(m_hCutImage);
//					float rw =  (imgW*1.0)/Vw;
//					float rh =  (imgH*1.0)/Vh;
//					int x1 = Sx*rw;
//					int y1 = Sy*rh;
//					int w1 = Sw*rw;
//					int h1 = Sh*rh;
//					m_hCutRect=EloamGlobal_CreateRect(x1,y1,w1,h1);
//				}
//				else
//				{
//					int x1 = EloamRect_GetLeft(m_hCutRect);
//					int y1 = EloamRect_GetTop(m_hCutRect);
//					int w1 =  EloamRect_GetWidth(m_hCutRect);
//					int h1 =  EloamRect_GetHeight(m_hCutRect);
//					float rw =  (w1*1.0)/Vw;
//					float rh =  (h1*1.0)/Vh;
//					int x2 = x1 + Sx*rw;
//					int y2 = y1 + Sy*rh;
//					int w2 = Sw*rw;
//					int h2 = Sh*rh;
//
//					EloamRect_Release(m_hCutRect);
//					m_hCutRect =NULL;
//					m_hCutRect=EloamGlobal_CreateRect(x2,y2,w2,h2);
//				}
//				EloamImage_Crop(m_hCutImage,m_hCutRect);
//
//				//����ͼƬ�Ķ�ֵ��Ч������Ϊ��һ��ͼƬ
//				if(!PrintConfig_ui.cB_Original->isChecked())
//				{
//					EloamImage_AdaptiveThreshold(m_hCutImage,m_nThresholdValue);
//				}
//				QDateTime Datetime = QDateTime::currentDateTime();
//				QString Ctime = Datetime.toString("yyyyMMdd_hhmmss");
//				m_sCutImagePath = m_CurPrintImageSavePath + QString::fromLocal8Bit("/") + Ctime + m_sImageSaveType;
//				EloamView_SelectImage(m_PrintView,m_hCutImage);
//				A4CombineProcessB(m_hCutImage,m_sCutImagePath);
//				EloamThumbnail_Add(m_hThumbPrint,(BSTR)(LPCWSTR)m_sCutImagePath.data());
//
//			}
//
//		}
//
//		//�ͷ��������
//		EloamRect_Release(m_hCutSelectRect);
//		m_hCutSelectRect= NULL;
//		m_bIsCut= false;
//		EloamView_SetSelectedRect(m_PrintView, NULL);
//		EloamView_SetState(m_PrintView, 1);	
//		//ui.pB_Cut->setText(QString::fromLocal8Bit("�����ü�"));
//	}
//
//}