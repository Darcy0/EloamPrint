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
	,m_hCutRect(NULL)
	,m_hCutImage(NULL)
	,m_IDCardFirstImage(NULL)
	,m_PrintView(NULL)
	,m_hPrintImage(NULL)
	,m_hPrintView(NULL)
	,m_hThumbPrint(NULL)
	,m_hIDCardCombine(NULL)				//����֤�ϳ�ͼƬ���
	,m_hImageReverse(NULL)
	,m_hShowResultImage(NULL)
	,m_hShowResultImageA(NULL)
	,m_nThresholdValue(40)
	,m_A3DefDpi(0)
	,m_A4DefDpi(0)
	,m_A4Width(0)
	,m_A4Height(0)
	,m_A3Width(0)
	,m_A3Height(0)
	,m_bIsShowA(false)
	,m_bIsVideoView(true)
	,m_ischangeDPI(0)					
{
	g_TheDilgAPP = this;
	ui.setupUi(this);
	SetButtonStyle();	//���ð�ť��ʽ
	SetLanguage();		//���ʻ���������
	m_CurBinPath = QCoreApplication::applicationDirPath();	
	ReadConfig();		//��ȡ���ò���
	
	m_printConfigUi=new PrintConfig(this);
	m_printConfigUi->SetConfigParam(m_printConfigParam);
	connect(m_printConfigUi,SIGNAL(newPrintConfigParm(QVariant&)),this,SLOT(onNewPrintConfig(QVariant&)));
	connect(m_printConfigUi,SIGNAL(StandardizationRestore()),this,SLOT(onStandardizationRestore()));
	connect(m_printConfigUi,SIGNAL(StandardizationCheckDPI(CalibrationCard)),this,SLOT(onStandardizationCheckDPI(CalibrationCard)));

	
	m_vMainDevice.clear();	
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
	m_hPrintView  = (HWND)ui.widget_VideoDlg->winId();
	m_PrintView = EloamGlobal_CreateView(m_hPrintView ,NULL,0);
	EloamGlobal_InitDevs(DevChangeCallback,this);
	if(m_vMainDevice.size()>0)
	{
		for(int i = 0;i< m_vMainDevice.size();i++)
		{	
			if(1 == EloamDevice_GetEloamType( m_vMainDevice[i]))
			{
				m_hMainDev = m_vMainDevice[i];
			}
		}		
	}
	else
	{
		m_hMainDev = NULL;
		EloamView_SetText(m_PrintView, L"δ��⵽�豸", RGB(255, 255, 255));

	}
	this->setMouseTracking ( true);

	ui.slider_Threshold->setValue(m_nThresholdValue);
	ui.spinBox_Threshold->setValue(m_nThresholdValue);
	ui.slider_Threshold->setVisible(false);
	ui.spinBox_Threshold->setVisible(false);
	ui.label_Threshold->setVisible(false);
	ui.pushButton_Cut->setVisible(false);
	ui.label_Cut->setText("");
	ui.label_kongbai->setVisible(false);

	connect(ui.pushButton_Config, SIGNAL(clicked()), this, SLOT(OnConfigDlg()));//����
	connect(ui.pushButton_Cut, SIGNAL(clicked()), this, SLOT(OnCut()));//�����ü�
	connect(ui.pushButton_Capture, SIGNAL(clicked()), this, SLOT(OnCapture()));//����
	connect(ui.pushButton_SwitchView, SIGNAL(clicked()), this, SLOT(OnSwitchView()));//��ӡԤ��
	connect(ui.pushButton_Print,SIGNAL(clicked()),this,SLOT(OnPrintPic()));//��ӡ
	connect(ui.checkBox_SelectAll,SIGNAL(clicked()),this,SLOT(OnCbSelectAll()));//ͼƬȫѡ
	connect(ui.slider_Threshold, SIGNAL(valueChanged(int)), ui.spinBox_Threshold, SLOT(setValue(int)));
	connect(ui.spinBox_Threshold, SIGNAL(valueChanged(int)),ui.slider_Threshold, SLOT(setValue(int)));
	connect(ui.slider_Threshold, SIGNAL(valueChanged(int)),this, SLOT(setThresholdValue(int)));
	connect(ui.spinBox_Threshold, SIGNAL(valueChanged(int)),this, SLOT(setThresholdValue(int)));
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
	if ( NULL != m_hPrintImage)//��ӡ���ڵ�ͼƬ���
	{
		EloamImage_Release(m_hPrintImage);
		m_hPrintImage = NULL;
	}
	if ( NULL != m_hShowResultImage)//������ʾ��ֵ��Ч����ͼƬ����
	{
		EloamImage_Release(m_hShowResultImage);
		m_hShowResultImage = NULL;
	}
	if ( NULL != m_hShowResultImageA)//������ʾ��ֵ��Ч����ͼƬ����a
	{
		EloamImage_Release(m_hShowResultImageA);
		m_hShowResultImageA = NULL;
	}

	if ( NULL != m_hImageReverse)//��ӡ���ڵ�ͼƬ���
	{
		EloamImage_Release(m_hImageReverse);
		m_hImageReverse = NULL;
	}

	if ( NULL != m_hIDCardCombine)//����֤�ϳ�ͼƬ���
	{
		EloamImage_Release(m_hIDCardCombine);
		m_hPrintImage = NULL;
	}
	if ( NULL != m_IDCardFirstImage)//����֤�ϳɵĵ�һ��ͼƬ
	{
		EloamImage_Release(m_IDCardFirstImage);
		m_IDCardFirstImage = NULL;
	}
	if ( NULL != m_hCutRect)//����֤�ϳɵĵ�һ��ͼƬ
	{
		EloamRect_Release(m_hCutRect);
		m_hCutRect = NULL;
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
	if(m_PrintView)
	{
		EloamView_Release(m_PrintView);
		m_PrintView = NULL;
	}
	int size = m_vMainDevice.size();
	for (int i = 0; i < size; i++)
	{
		HELOAMDEVICE hDev = m_vMainDevice[i];
		EloamDevice_Release(hDev);
	}
	m_vMainDevice.clear();
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
	QString configPath = m_CurBinPath + QString::fromLocal8Bit("/QDilgCameraConfig.ini");
	QFile file(configPath);
	if(file.exists())
	{
		QString strDefImageSavePath=m_CurBinPath + QString::fromLocal8Bit("/image");
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
	QString configPath = m_CurBinPath + QString::fromLocal8Bit("/QDilgCameraConfig.ini");
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
	for(int i = 0;i< m_vMainDevice.size();i++)
	{	
		HELOAMDEVICE hDev = m_vMainDevice[i];
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
				EloamView_SelectVideo(m_PrintView, m_hMainVideo, AttachCallback, this);  
				EloamView_SetText(m_PrintView, L"����Ƶ�У���ȴ�...", RGB(255, 255, 255));
				EloamView_SetCallback(m_PrintView, ZoomChange, this);					
				m_bIsLostDev =false;
				devFlag=100;
				break;
			}			
		}
	}	
}

void EloamPrint::OnCloseDevice()
{
	EloamView_SetText(m_PrintView, NULL, 0);
	if(m_hMainVideo)
	{
		EloamVideo_Release(m_hMainVideo);
		m_hMainVideo = NULL;
	}	
}


void EloamPrint::ReversePrintImage(HELOAMIMAGE m_hPrintImage)
{	
	if(m_hImageReverse != NULL)
	{
		EloamImage_Release(m_hImageReverse);
		m_hImageReverse = NULL;
	}
	if(m_printConfigParam.printSize==PrintSizeA3)
	{
		m_hImageReverse = EloamGlobal_CreateImage(m_A3Width, m_A3Height,3);	
	}
	else
	{
		m_hImageReverse = EloamGlobal_CreateImage(m_A4Width, m_A4Height,3);
	}
	int w1 = EloamImage_GetWidth(m_hPrintImage);
	int h1 = EloamImage_GetHeight(m_hPrintImage);
	bool bResize = EloamImage_Resize(m_hPrintImage,w1/1.038,h1/1.038,0);
	if(bResize)
	{
		w1 = EloamImage_GetWidth(m_hPrintImage);
		h1 = EloamImage_GetHeight(m_hPrintImage);
	}
	if (m_hImageReverse)
	{
		EloamImage_SetXDPI(m_hImageReverse,m_fCurXDpi);
		EloamImage_SetYDPI(m_hImageReverse,m_fCurYDpi);
		HELOAMRECT rectDest1 = EloamGlobal_CreateRect((m_A4Width-w1)/2+20, (m_A4Height-h1)/2+20, w1-40,h1-40);
		HELOAMRECT rectSrc1 = EloamGlobal_CreateRect(20, 20, w1-40, h1-40);
		EloamImage_Blend(m_hImageReverse, rectDest1, m_hPrintImage, rectSrc1, 0, 0);
		EloamRect_Release(rectDest1);
		EloamRect_Release(rectSrc1);
	}
}

void EloamPrint::OnStopPrint()
{
	QString m_printInfo = translate.GetlanguageAbbr("STRING_PRINT_TIPS");

	//m_bDeleteBk = false;
	EloamVideo_DisableDelBkColor(m_hMainVideo);
	//ui.m_DeleteNoise->setChecked(false);
	if(m_PrintView)
	{
		EloamView_Release(m_PrintView);
		m_PrintView=NULL;
	}
	if(m_hPrintImage)
	{
		EloamImage_Release(m_hPrintImage);
		m_hPrintImage=NULL;
	}
	m_hPrintView=NULL;
	m_printConfigParam.printNum=1;
	//m_PrintNum=1;
	
}


void EloamPrint::A3CombineProcess(HELOAMIMAGE hImg,QString imgPath)
{
	HELOAMRECT rectDest1;
	HELOAMRECT rectSrc1;
	HELOAMIMAGE hImageCombine;
	int w1=EloamImage_GetWidth(hImg);
	int h1=EloamImage_GetHeight(hImg);
	bool bResize = EloamImage_Resize(hImg,w1*1.038,h1*1.038,0);
	if(bResize)
	{
		w1 = EloamImage_GetWidth(hImg);
		h1 = EloamImage_GetHeight(hImg);
	}
	if(w1>=h1)
	{
		if(!m_printConfigParam.isOrignal)
		{
			hImageCombine=EloamGlobal_CreateImage(m_A3Width, m_A3Height,1);//�Ҷ�ͼ��
		}else 
		{	
			hImageCombine=EloamGlobal_CreateImage(m_A3Width, m_A3Height,3);//��ɫͼ��
		}
		if((w1>m_A3Width)||(h1>m_A3Height))
		{
			//���������ļ��ߴ��Ƿ����A3
			rectDest1 = EloamGlobal_CreateRect(0,0,m_A3Width,m_A3Height);
			rectSrc1 = EloamGlobal_CreateRect((w1-m_A3Width)/2,(h1-m_A3Height)/2, m_A3Width, m_A3Height);
		}
		else
		{
			rectDest1 = EloamGlobal_CreateRect((m_A3Width-w1)/2, (m_A3Height-h1)/2, w1,h1);
			rectSrc1 = EloamGlobal_CreateRect(0, 0, w1, h1);
		}
	}
	else
	{		
		if(!m_printConfigParam.isOrignal)
		{
			hImageCombine=EloamGlobal_CreateImage(m_A3Height,m_A3Width, 1);
		}else 
		{	
			hImageCombine=EloamGlobal_CreateImage(m_A3Height, m_A3Width,3);
		}
		if((w1>m_A3Height)||(h1>m_A3Width))
		{
			//���������ļ��ߴ��Ƿ����A3
			rectDest1 = EloamGlobal_CreateRect(0,0,m_A3Height,m_A3Width);
			rectSrc1 = EloamGlobal_CreateRect((w1-m_A3Height)/2,(h1-m_A3Width)/2,m_A3Height, m_A3Width);
		}
		else
		{
			rectDest1 = EloamGlobal_CreateRect((m_A3Height-w1)/2+20, (m_A3Width-h1)/2+20, w1-40,h1-40);
			rectSrc1 = EloamGlobal_CreateRect(20, 20, w1-40,h1-40);
		}
	}
	if (hImageCombine)
	{

		EloamImage_SetXDPI(hImageCombine,m_fCurXDpi);
		EloamImage_SetYDPI(hImageCombine,m_fCurYDpi);
		EloamImage_Blend(hImageCombine, rectDest1, hImg, rectSrc1, 0, 0);
		
		if (EloamImage_Save(hImageCombine, (BSTR)(LPCWSTR)imgPath.data(), m_iJPEGQulitVal))
		{
				EloamGlobal_PlayCaptureVoice();
				//EloamView_PlayCaptureEffect(m_hMainView);
				EloamThumbnail_Add(m_hThumbPrint,(BSTR)(LPCWSTR)imgPath.data());
				SendMsgToParent(imgPath);
		}
		EloamImage_Release( hImageCombine );
		hImageCombine = NULL;
	}
	EloamRect_Release(rectDest1);
	EloamRect_Release(rectSrc1);
}

void EloamPrint::A4CombineProcess(HELOAMIMAGE hImg,QString imgPath)
{
	HELOAMIMAGE hImageCombine;
	HELOAMRECT rectDest1;
	HELOAMRECT rectSrc1;
	int w1 = EloamImage_GetWidth(hImg);
	int h1 = EloamImage_GetHeight(hImg);
	bool bResize = EloamImage_Resize(hImg,w1*1.038,h1*1.038,0);
	if(bResize)
	{
		w1 = EloamImage_GetWidth(hImg);
		h1 = EloamImage_GetHeight(hImg);
	}
	if(w1>=h1)
	{
		if(!m_printConfigParam.isOrignal)
		{
			hImageCombine=EloamGlobal_CreateImage(m_A4Width, m_A4Height,1);
		}else 
		{	
			hImageCombine=EloamGlobal_CreateImage(m_A4Width, m_A4Height,3);
		}
		if((w1>m_A4Width)||(h1>m_A4Height))
		{
			//���������ļ��ߴ��Ƿ����A4
			rectDest1 = EloamGlobal_CreateRect(0,0,m_A4Width,m_A4Height);
			rectSrc1 = EloamGlobal_CreateRect((w1-m_A4Width)/2,(h1-m_A4Height)/2, m_A4Width, m_A4Height);
		}
		else
		{
			rectDest1 = EloamGlobal_CreateRect((m_A4Width-w1)/2+20, (m_A4Height-h1)/2+20, w1-40,h1-40);
			rectSrc1 = EloamGlobal_CreateRect(20,20, w1-40, h1-40);
		}
	}
	else
	{

		if(!m_printConfigParam.isOrignal)
		{
			hImageCombine=EloamGlobal_CreateImage(m_A4Height,m_A4Width, 1);
		}else 
		{	
			hImageCombine=EloamGlobal_CreateImage(m_A4Height, m_A4Width,3);
		}
		if((w1>m_A4Height)||(h1>m_A4Width))
		{
			//���������ļ��ߴ��Ƿ����A4
			rectDest1 = EloamGlobal_CreateRect(0,0,m_A4Height,m_A4Width);
			rectSrc1 = EloamGlobal_CreateRect((w1-m_A4Height)/2,(h1-m_A4Width)/2,m_A4Height, m_A4Width);
		}
		else
		{
			rectDest1 = EloamGlobal_CreateRect((m_A4Height-w1)/2, (m_A4Width-h1)/2, w1,h1);
			rectSrc1 = EloamGlobal_CreateRect(20, 20, w1-40,h1-40);
		}
	}

	if (hImageCombine)
	{
		EloamImage_SetXDPI(hImageCombine,m_fCurXDpi);
		EloamImage_SetYDPI(hImageCombine,m_fCurYDpi);
		EloamImage_Blend(hImageCombine, rectDest1, hImg, rectSrc1, 0, 0);
		if (EloamImage_Save(hImageCombine, (BSTR)(LPCWSTR)imgPath.data(), m_iJPEGQulitVal))
		{
			EloamGlobal_PlayCaptureVoice();
			EloamThumbnail_Add(m_hThumbPrint,(BSTR)(LPCWSTR)imgPath.data());
			SendMsgToParent(imgPath);
		}
		EloamImage_Release( hImageCombine );
		hImageCombine = NULL;
	}
	EloamRect_Release(rectDest1);
	EloamRect_Release(rectSrc1);

}



void EloamPrint::A4CombineProcessB(HELOAMIMAGE hImg,QString imgPath)
{
 	HELOAMIMAGE hImageCombine = EloamGlobal_CreateImage(m_A4Width, m_A4Height,1);
	HELOAMRECT rectDest1;
	HELOAMRECT rectSrc1;
	int w1 = EloamImage_GetWidth(hImg);
	int h1 = EloamImage_GetHeight(hImg); 
	bool bResize = EloamImage_Resize(hImg,w1*1.038,h1*1.038,0);
	if(bResize)
	{
		w1 = EloamImage_GetWidth(hImg);
		h1 = EloamImage_GetHeight(hImg);
	}
	if(w1>=h1)
	{
		if(!m_printConfigParam.isOrignal)
		{
			hImageCombine=EloamGlobal_CreateImage(m_A4Width, m_A4Height,1);
		}else 
		{	
			hImageCombine=EloamGlobal_CreateImage(m_A4Width, m_A4Height,3);
		}
		if((w1>m_A4Width)||(h1>m_A4Height))
		{
			//���������ļ��ߴ��Ƿ����A4
			rectDest1 = EloamGlobal_CreateRect(0,0,m_A4Width,m_A4Height);
			rectSrc1 = EloamGlobal_CreateRect((w1-m_A4Width)/2,(h1-m_A4Height)/2, m_A4Width, m_A4Height);
		}
		else
		{
			rectDest1 = EloamGlobal_CreateRect((m_A4Width-w1)/2, (m_A4Height-h1)/2, w1,h1);
			rectSrc1 = EloamGlobal_CreateRect(0, 0, w1, h1);
		}
	}
	else
	{
		
		if(!m_printConfigParam.isOrignal)
		{
			hImageCombine=EloamGlobal_CreateImage(m_A4Height,m_A4Width, 1);
		}else 
		{	
			hImageCombine=EloamGlobal_CreateImage(m_A4Height, m_A4Width,3);
		}
		if((w1>m_A4Height)||(h1>m_A4Width))
		{
			//���������ļ��ߴ��Ƿ����A4
			rectDest1 = EloamGlobal_CreateRect(0,0,m_A4Height,m_A4Width);
			rectSrc1 = EloamGlobal_CreateRect((w1-m_A4Height)/2,(h1-m_A4Width)/2,m_A4Height, m_A4Width);
		}
		else
		{
			rectDest1 = EloamGlobal_CreateRect((m_A4Height-w1)/2, (m_A4Width-h1)/2, w1,h1);
			rectSrc1 = EloamGlobal_CreateRect(0,0, w1,h1);
		}
	}
	if (hImageCombine)
	{
		
		EloamImage_SetXDPI(hImageCombine,m_fCurXDpi);
		EloamImage_SetYDPI(hImageCombine,m_fCurYDpi);
		EloamImage_Blend(hImageCombine, rectDest1, hImg, rectSrc1, 0, 0);
		if(EloamImage_Save(hImageCombine, (BSTR)(LPCWSTR)imgPath.data(), 0))
		{
			//EloamThumbnail_Add(m_hThumbPrint,(BSTR)(LPCWSTR)imgPath.data());
			SendMsgToParent(imgPath);
		}
		EloamImage_Release( hImageCombine );
		hImageCombine = NULL;
	}
	EloamRect_Release(rectDest1);
	EloamRect_Release(rectSrc1);
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
		rectSrcWidth=printWidth;
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
	//if (EloamImage_Save(hImageCombine, (BSTR)(LPCWSTR)imgPath.data(), m_iJPEGQulitVal))
	//	{
	//		EloamGlobal_PlayCaptureVoice();
	//		//EloamView_PlayCaptureEffect(m_hMainView);
	//		EloamThumbnail_Add(m_hThumbPrint,(BSTR)(LPCWSTR)imgPath.data());
	//		SendMsgToParent(imgPath);
	//	}
	//	EloamImage_Release( hImageCombine );
	//	hImageCombine = NULL;
	//}
}






void EloamPrint::A4IDCardCombineProcess(HELOAMIMAGE hImg,QString imgPath)
{	
	QString m_printInfo = translate.GetlanguageAbbr("STRING_PRINT_TIPS");
	QString m_printMessge = translate.GetlanguageAbbr("STRING_PRINT_IDCARDCOMBINE_START");
	QString m_printMessge2 =translate.GetlanguageAbbr("STRING_PRINT_IDCARDCOMBINE_END");
	QString m_printMessge3 =translate.GetlanguageAbbr("STRING_PRINT_IDCARDCOMBINE_OVERRANGEA5");
	QString m_printMessge4 =translate.GetlanguageAbbr("STRING_PRINT_IDCARDCOMBINE_OVERRANGEA4");
	EloamView_SetMessage(m_PrintView,NULL);
	int w1=EloamImage_GetWidth(hImg);
	int h1=EloamImage_GetHeight(hImg);
	int width,height;
	HELOAMRECT rectDest1;
	HELOAMRECT rectSrc1;
	bool bResize = EloamImage_Resize(hImg,w1*1.038,h1*1.038,0);
	if(bResize)
	{
		w1 = EloamImage_GetWidth(hImg);
		h1 = EloamImage_GetHeight(hImg);
		if(PrintSizeA4==m_printConfigParam.printSize)
		{
			width = m_A4Height;
			height = m_A4Width;
			if(((width*1.038)<w1)||((height/2*1.038)<h1))
			{
				QMessageBox::information(NULL,m_printInfo,m_printMessge3,NULL);
				return;
			}
		}
		else 
		{
			width = m_A3Height;
			height = m_A3Width;
			if(((width*1.038)<=w1)||((height/2*1.038)<=h1))
			{
				QMessageBox::information(NULL,m_printInfo,m_printMessge4,NULL);
				return;
			}

		}
		
	}
	if(m_hIDCardCombine)
	{
		EloamImage_SetXDPI(m_hIDCardCombine,m_fCurYDpi);
		EloamImage_SetYDPI(m_hIDCardCombine,m_fCurXDpi);
		//�ڶ������֤��Ƭ������
		if(w1>width||h1>(height/2))
		{
			rectDest1 = EloamGlobal_CreateRect(0,height/2, width,height/2);
			rectSrc1 = EloamGlobal_CreateRect((w1-width)/2, (h1-height/2)/2, width, height/2);
		}else
		{
			rectDest1 = EloamGlobal_CreateRect((width-w1)/2,(height/2+20), w1,h1);
			rectSrc1 = EloamGlobal_CreateRect(0, 0, w1, h1);
		}
		EloamImage_Blend(m_hIDCardCombine, rectDest1, hImg, rectSrc1, 0, 0);
		EloamRect_Release(rectDest1);
		EloamRect_Release(rectSrc1);
		if(!m_printConfigParam.isOrignal)
		{
			EloamImage_AdaptiveThreshold(m_hIDCardCombine,m_nThresholdValue);
		}
		if (EloamImage_Save(m_hIDCardCombine, (BSTR)(LPCWSTR)imgPath.data(), m_iJPEGQulitVal))
		{
			EloamThumbnail_Add(m_hThumbPrint,(BSTR)(LPCWSTR)imgPath.data());
			SendMsgToParent(imgPath);
		}
		EloamImage_Release( m_hIDCardCombine );
		m_hIDCardCombine = NULL;
		QMessageBox::information(NULL,m_printInfo,m_printMessge2,NULL);
	}
	else
	{
		if(!m_printConfigParam.isOrignal)
		{
			m_hIDCardCombine = EloamGlobal_CreateImage(width, height,1);
		}else 
		{	
			m_hIDCardCombine = EloamGlobal_CreateImage(width, height,3);
		}
		if(m_hIDCardCombine)
		{
			EloamImage_SetXDPI(m_hIDCardCombine,m_fCurYDpi);
			EloamImage_SetYDPI(m_hIDCardCombine,m_fCurXDpi);
		}
		//��һ�����֤��Ƭ������
		QMessageBox::information(NULL,m_printInfo,m_printMessge,NULL);

		if(w1>width||h1>(height/2))
		{
			rectDest1 = EloamGlobal_CreateRect(0,0, width,height/2);
			rectSrc1 = EloamGlobal_CreateRect((w1-width)/2, (h1-height/2)/2, width, height/2);
		}else
		{
			rectDest1 = EloamGlobal_CreateRect((width-w1)/2, (height/2-h1-20), w1,h1);
			rectSrc1 = EloamGlobal_CreateRect(0, 0, w1, h1);
		}

		EloamImage_Blend(m_hIDCardCombine, rectDest1, hImg, rectSrc1, 0, 0);
		EloamRect_Release(rectDest1);
		EloamRect_Release(rectSrc1);
		m_printMessge = translate.GetlanguageAbbr("STRING_PRINT_OTHERHAND");
		EloamView_SetMessage(m_PrintView,(BSTR)(LPCWSTR)m_printMessge.data());
	}	
}

void EloamPrint::OnScanPrint2()
{
	if(NULL != m_hPrintImage)
	{
		EloamImage_Release(m_hPrintImage);
		m_hPrintImage=NULL;
	}	
	m_hPrintImage = EloamVideo_CreateImage(m_hMainVideo, 0, m_PrintView);
	if(!m_hPrintImage)
	{
		return;
	}
	if(!m_printConfigParam.isOrignal)
	{
		EloamImage_AdaptiveThreshold(m_hPrintImage,m_nThresholdValue);
	}
	if(m_hPrintImage)                //ϵͳʱ��
	{
		QDateTime Datetime = QDateTime::currentDateTime();
		QString Ctime = Datetime.toString("yyyyMMdd_hhmmss");
		QString FileSaveName;
		FileSaveName =m_printConfigParam.savePath + QString::fromLocal8Bit("/") + Ctime + GetImageSaveSuffix(m_iImgSaveType);
		A4IDCardCombineProcess(m_hPrintImage,FileSaveName);
	}
}



void EloamPrint::OnScanSizeA3()
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

void EloamPrint::OnScanSizeA4()
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
	if(!m_bIsCut)
	{
		m_bIsCut= true;
		EloamView_SetState(m_PrintView, 2);//���ô���״̬Ϊ�Զ���
		ui.label_Cut->setText(translate.GetlanguageAbbr("STRING_PRINT_SURECUT"));
		ui.pushButton_Cut->setToolTip(translate.GetlanguageAbbr("STRING_PRINT_SURECUT"));
	}
	else
	{
		ui.label_Cut->setText(translate.GetlanguageAbbr("STRING_PRINT_CUT"));
		ui.pushButton_Cut->setToolTip(translate.GetlanguageAbbr("STRING_PRINT_CUT"));
		
		HELOAMRECT m_hCutSelectRect=EloamView_GetSelectedRect(m_PrintView);
		if (NULL==m_hCutSelectRect)
		{
			m_bIsCut= false;
			EloamView_SetSelectedRect(m_PrintView, NULL);
			EloamView_SetState(m_PrintView, 1);	
			return;
		}
		
		
		int Sx=EloamRect_GetLeft(m_hCutSelectRect);
		int Sy=EloamRect_GetTop(m_hCutSelectRect);
		int Sw = EloamRect_GetWidth(m_hCutSelectRect);
		int Sh= EloamRect_GetHeight(m_hCutSelectRect);

		int Vw = EloamVideo_GetWidth(m_hMainVideo);
		int Vh = EloamVideo_GetHeight(m_hMainVideo);
		if(m_sShowResultImagePath != NULL)
		{
			//����ʾԤ��Ч����ͼƬ���м��ã������Ǳ��棬�ٽ��ж�ֵ������ʱ�Լ��ú��Ϊ����
			if(NULL != m_hCutImage )
			{
				EloamImage_Release(m_hCutImage);
				m_hCutImage =NULL;
			}
			m_hCutImage = EloamGlobal_CreateImageFromFile((BSTR)(LPCWSTR)m_sShowResultImagePath.data(),0);
			if(NULL == m_hCutRect)
			{

				int imgW = EloamImage_GetWidth(m_hCutImage);
				int imgH = EloamImage_GetHeight(m_hCutImage);
				float rw =  (imgW*1.0)/Vw;
				float rh =  (imgH*1.0)/Vh;
				int x1 = Sx*rw;
				int y1 = Sy*rh;
				int w1 = Sw*rw;
				int h1 = Sh*rh;
				m_hCutRect=EloamGlobal_CreateRect(x1,y1,w1,h1);
			}
			else
			{
				int x1 = EloamRect_GetLeft(m_hCutRect);
				int y1 = EloamRect_GetTop(m_hCutRect);
				int w1 =  EloamRect_GetWidth(m_hCutRect);
				int h1 =  EloamRect_GetHeight(m_hCutRect);
				float rw =  (w1*1.0)/Vw;
				float rh =  (h1*1.0)/Vh;
				int x2 = x1 + Sx*rw;
				int y2 = y1 + Sy*rh;
				int w2 = Sw*rw;
				int h2 = Sh*rh;

				EloamRect_Release(m_hCutRect);
				m_hCutRect =NULL;
				m_hCutRect=EloamGlobal_CreateRect(x2,y2,w2,h2);
			}
			EloamImage_Crop(m_hCutImage,m_hCutRect);

			//����ͼƬ�Ķ�ֵ��Ч������Ϊ��һ��ͼƬ
			if(!m_printConfigParam.isOrignal)
			{
				EloamImage_AdaptiveThreshold(m_hCutImage,m_nThresholdValue);
			}
			QDateTime Datetime = QDateTime::currentDateTime();
			QString Ctime = Datetime.toString("yyyyMMdd_hhmmss");
			m_sCutImagePath = m_printConfigParam.savePath + QString::fromLocal8Bit("/") + Ctime + GetImageSaveSuffix(m_iImgSaveType);
			EloamView_SelectImage(m_PrintView,m_hCutImage);
			A4CombineProcessB(m_hCutImage,m_sCutImagePath);
			EloamThumbnail_Add(m_hThumbPrint,(BSTR)(LPCWSTR)m_sCutImagePath.data());
		}
		//�ͷ��������
		EloamRect_Release(m_hCutSelectRect);
		m_hCutSelectRect= NULL;
		m_bIsCut= false;
		EloamView_SetSelectedRect(m_PrintView, NULL);
		EloamView_SetState(m_PrintView, 1);	
	}
}
//����
void EloamPrint::OnCapture()
{
	QString saveFileName=GetImageSaveFileName();
	
	if(m_printConfigParam.isIdCard)
	{
		OnScanPrint2();
	}
	else
	{
		if(NULL != m_hPrintImage)
		{
			EloamImage_Release(m_hPrintImage);
			m_hPrintImage=NULL;
		}

		m_hPrintImage = EloamVideo_CreateImage(m_hMainVideo, 0, m_PrintView);
		if(NULL == m_hPrintImage)
		{
			return;
		}
		else            //ϵͳʱ��
		{
			QDateTime Datetime = QDateTime::currentDateTime();
			QString Ctime = Datetime.toString("yyyyMMdd_hhmmss");
			QString FileSaveName;
			FileSaveName =m_printConfigParam.savePath + QString::fromLocal8Bit("/") + Ctime + GetImageSaveSuffix(m_iImgSaveType);

			if(!m_printConfigParam.isOrignal)
			{
				EloamImage_AdaptiveThreshold(m_hPrintImage,m_nThresholdValue);
			}

			if(PrintSizeA3==m_printConfigParam.printSize)
			{
				A3CombineProcess(m_hPrintImage,FileSaveName);
			}
			else
			{
				A4CombineProcess(m_hPrintImage,FileSaveName);
			}
		}
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

		EloamView_SelectVideo(m_PrintView,m_hMainVideo,NULL,this);
		m_bIsCut = false;
		ui.pushButton_Cut->setToolTip(translate.GetlanguageAbbr("STRING_PRINT_CUT"));
	
		if(NULL != m_hCutRect)
		{
			EloamRect_Release(m_hCutRect);
			m_hCutRect = NULL;
		}
		if(NULL != m_hCutImage)
		{
			EloamImage_Release(m_hCutImage);
			m_hCutImage = NULL;
		}
		if(m_hShowResultImage != NULL)
		{
			EloamImage_Release(m_hShowResultImage);
			m_hShowResultImage = NULL; 
		}
		if(m_hShowResultImageA != NULL)
		{
			EloamImage_Release(m_hShowResultImageA);
			m_hShowResultImageA = NULL; 
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
		
		
		if(NULL!=m_hShowResultImage)
		{
			EloamImage_Release(m_hShowResultImage);
			m_hShowResultImage = NULL; 
		}
		if(NULL!=m_hShowResultImageA)
		{
			EloamImage_Release(m_hShowResultImageA);
			m_hShowResultImageA = NULL; 
		}

		m_sShowResultImagePath = GetImageSaveFileName();
		m_hShowResultImage = EloamVideo_CreateImage(m_hMainVideo,0,m_PrintView);
		EloamImage_Save(m_hShowResultImage,(BSTR)(LPCWSTR)m_sShowResultImagePath.data(),0);
		if(!m_printConfigParam.isOrignal)
		{
			EloamImage_AdaptiveThreshold(m_hShowResultImage,m_nThresholdValue); 
			QString m_sImagePath = GetImageSaveFileName("1");	
			A4CombineProcess(m_hShowResultImage,m_sImagePath);
		}
		else
		{	
			QString m_sImagePath = GetImageSaveFileName();
			A4CombineProcess(m_hShowResultImage,m_sImagePath);
		}
		EloamView_SelectImage(m_PrintView,m_hShowResultImage);	
	}
}
//��ӡ
void EloamPrint::OnPrintPic()										//��ӡ��ǰͼƬ
{
	QString strLid;
	QString m_printInfo = translate.GetlanguageAbbr("STRING_PRINT_TIPS");
	QString m_printMessge =  translate.GetlanguageAbbr("STRING_PRINT_CHECK_SELECTED");
	HELOAMIMAGELIST imgList = EloamGlobal_CreateImageList();	           
	if(false == m_bIsVideoView)//�����ǰΪ��ӡԤ��״̬��ֱ�Ӵ�ӡ��ǰԤ����ͼƬ
	{
		if(NULL != m_hCutRect)
		{
			m_hPrintImage = EloamGlobal_CreateImageFromFile((BSTR)(LPCWSTR)m_sCutImagePath.data(),0);
		}
		else
		{
			m_hPrintImage = EloamGlobal_CreateImageFromFile((BSTR)(LPCWSTR)m_sShowResultImagePath.data(),0);
			if(!m_printConfigParam.isOrignal)
			{
				EloamImage_AdaptiveThreshold(m_hPrintImage,m_nThresholdValue);
			}
		}
		ReversePrintImage(m_hPrintImage);
		EloamImage_AdaptivePrintByDPI(m_hImageReverse,NULL);	
		EloamImage_Release(m_hImageReverse);
		m_hImageReverse = NULL;
		EloamImage_Release(m_hPrintImage);
		m_hPrintImage = NULL;
	}
	else
	{
		if (imgList) 
		{
			int count = EloamThumbnail_GetCount(m_hThumbPrint);
			for (int i = 0; i < count; i++)
			{
				bool b = EloamThumbnail_GetCheck(m_hThumbPrint,i);
				if(b)
				{
					BSTR imagePath = EloamThumbnail_GetFileName(m_hThumbPrint,i);
					HELOAMIMAGE tempImage = EloamGlobal_CreateImageFromFile(imagePath, 0);
					EloamImageList_Add(imgList, tempImage);
					EloamImage_Release(tempImage);
				}
			}

			int listCount =EloamImageList_GetCount(imgList);
			if(0 < listCount)
			{
				for(int i=0;i<listCount;i++)
				{

					//��ԭ�����Ŵ��ͼƬ
					if(NULL != m_hPrintImage)
					{
						EloamImage_Release(m_hPrintImage);
						m_hPrintImage = NULL;
					}
					m_hPrintImage = EloamImageList_GetImage(imgList,i);
					ReversePrintImage(m_hPrintImage);

					if(m_printConfigParam.printNum != 0)
					{
						for(int i = 0 ; i < m_printConfigParam.printNum ; i++)
						{
							if(NULL != m_hImageReverse )
							{
								EloamImage_AdaptivePrintByDPI(m_hImageReverse,NULL);	
							}
						}
					}
					EloamImage_Release(m_hImageReverse);
					m_hImageReverse = NULL;
				}
			}
			else
			{
				QMessageBox::information(NULL,m_printInfo,m_printMessge,NULL);	
			}
			EloamImageList_Release(imgList);
			imgList = NULL;
		}
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
void EloamPrint::setThresholdValue(int Value)
{
	int thresholdValue = ui.slider_Threshold->value();
	if (m_sShowResultImagePath.isEmpty())
	{
		return;
	}
	if(m_bIsShowA)
	{
		if(m_hShowResultImage != NULL)
		{
			EloamImage_Release(m_hShowResultImage);
			m_hShowResultImage = NULL;
		}
		m_hShowResultImage = EloamGlobal_CreateImageFromFile((BSTR)(LPCWSTR)m_sShowResultImagePath.data(),0);
		EloamImage_AdaptiveThreshold(m_hShowResultImage,m_nThresholdValue);
		if(NULL != m_hCutRect)
		{
			EloamImage_Crop(m_hShowResultImage,m_hCutRect);
			A4CombineProcessB(m_hShowResultImage,m_sCutImagePath);
		}

		EloamView_SelectImage(m_PrintView,m_hShowResultImage);
		m_bIsShowA = FALSE;
	}
	else
	{
		if(m_hShowResultImageA != NULL)
		{
			EloamImage_Release(m_hShowResultImageA);
			m_hShowResultImageA = NULL;
		}
		m_hShowResultImageA = EloamGlobal_CreateImageFromFile((BSTR)(LPCWSTR)m_sShowResultImagePath.data(),0);
		EloamImage_AdaptiveThreshold(m_hShowResultImageA,m_nThresholdValue);
		if(NULL != m_hCutRect)
		{
			EloamImage_Crop(m_hShowResultImageA,m_hCutRect);
			A4CombineProcessB(m_hShowResultImageA,m_sCutImagePath);
		}
		EloamView_SelectImage(m_PrintView,m_hShowResultImageA);
		m_bIsShowA = TRUE;
	}
}







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
			OnScanSizeA4();
		}
		else if (PrintSizeA3==m_printConfigParam.printSize)
		{
			OnScanSizeA3();
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
	else
	{
		m_fCurXDpi=m_A4DefDpi;
		m_fCurYDpi=m_A4DefDpi;

		m_A4Width  =  m_A4HA4WidthDef;
		m_A4Height =  m_A4HA4HeightDef;

		m_A3Width  = m_A4HA3WidthDef;
		m_A3Height = m_A4HA3HeightDef;
	}
}

void EloamPrint::onStandardizationCheckDPI( CalibrationCard size )
{
	QString strLid;
	QString strName;
	QString m_printInfo = translate.GetlanguageAbbr("STRING_PRINT_TIPS");
	QString m_printMessge = translate.GetlanguageAbbr("STRING_PRINT_PUTA4_CARD");
	QString m_printMessge2 = translate.GetlanguageAbbr("STRING_PRINT_PUTA5_CARD");
	QString m_printMessge3 = translate.GetlanguageAbbr("STRING_PRINT_CHECKDPI_OUTRANGE");
	QString m_printMessge4 = translate.GetlanguageAbbr("STRING_PRINT_CHECKDPI_COMPLETE");
	int m_temp=0;
	if(CalibrationCardA4 == m_printConfigParam.calibrationCard)
	{//A4
		QMessageBox::information(NULL,m_printInfo,m_printMessge,NULL);	
	
		HELOAMIMAGE hImage = EloamVideo_CreateImage(m_hMainVideo, 0, m_PrintView);
		m_A4Width = EloamImage_GetWidth(hImage);
		m_A4Height = EloamImage_GetHeight(hImage);
		if( m_A4Width > m_A4Height)
		{//���
			if(m_printConfigParam.isDeskew)
			{
				m_A4Width = m_A4Width +20;
				m_A4Height = m_A4Height +23;
			}
			m_A3Width = m_A4Height*2;
			m_A3Height = m_A4Width;
			m_fCurXDpi = (m_A4Width * 254.0 ) / 2970.0;
			m_fCurYDpi = (m_A4Height * 254.0) / 2100.0;
		}
		else
		{//����	
			if(!m_printConfigParam.isDeskew)
			{
				m_temp = m_A4Width;
				m_A4Width = m_A4Height +18;
				m_A4Height = m_temp + 22 ;
			}
			else
			{	
				m_temp = m_A4Width; 
				m_A4Width = m_A4Height ;
				m_A4Height = m_temp  ;
			}
			m_fCurXDpi=(m_A4Width * 25.4 )/2100.0;
			m_fCurYDpi=(m_A4Height * 25.4 )/2970.0;
		}
	
		EloamImage_Release(hImage);
		hImage = NULL;
	}
	else
	{//A5
		QMessageBox::information(NULL,m_printInfo,m_printMessge2,NULL);	
		int m_A5Width,m_A5Height;
		HELOAMIMAGE hImage = EloamVideo_CreateImage(m_hMainVideo, 0, m_PrintView);
		m_A5Width = EloamImage_GetWidth(hImage);
		m_A5Height = EloamImage_GetHeight(hImage);
	
		if( m_A5Width > m_A5Height)
		{
			if(m_printConfigParam.isDeskew)
			{
				m_A5Width = m_A5Width +18;
				m_A5Height = m_A5Height +22;
			}
			m_A4Width = m_A5Height*2;
			m_A4Height = m_A5Width;
			m_A3Width = m_A4Height*2;
			m_A3Height = m_A4Width;
			m_fCurXDpi = (m_A5Width * 254.0 ) / 2100.0;
			m_fCurYDpi = (m_A5Height * 254.0) / 1485.0;
		}
		else
		{
			if(!m_printConfigParam.isDeskew)
			{
				m_temp = m_A5Width;
				m_A5Width = m_A5Height +18;
				m_A5Height = m_temp +22;
			}
			else
			{
				m_temp = m_A5Width;
				m_A5Width = m_A5Height;
				m_A5Height = m_temp;
			}
			m_A4Width = m_A5Height*2;
			m_A4Height = m_A5Width;
			m_A3Width = m_A4Height*2;
			m_A3Height = m_A4Width;
			m_fCurXDpi=(m_A4Width * 25.4 )/1485.0;
			m_fCurYDpi=(m_A4Height * 25.4 )/2100.0;
		}
		EloamImage_Release(hImage);
		hImage = NULL;
	}
	//if(!( ((m_CurXDpi<(m_A4DefDpi+10)) && (m_CurXDpi>(m_A4DefDpi-10)) && (m_CurYDpi<(m_A4DefDpi+10)) && (m_CurYDpi>(m_A4DefDpi-10)))||((m_CurXDpi<(m_A3DefDpi+10)) && (m_CurXDpi>(m_A3DefDpi-10)) && (m_CurYDpi<(m_A3DefDpi+10)) && ( m_CurYDpi>(m_A3DefDpi-10)) )) )
	//{
	//	OnRestore();
	//	QMessageBox::information(0,m_printInfo,m_printMessge3,0);
	//	return;
	//}
	//else
	//{
	QMessageBox::information(0,m_printInfo,m_printMessge4,0);
	//}
	char format = 'g';
	m_ischangeDPI = 1;
}

//�̳���д�麯��
void EloamPrint::closeEvent(QCloseEvent *event)
{

}

//�豸�ص�����
VOID ELOAMAPI EloamPrint::DevChangeCallback(LONG type, LONG idx, LONG dbt, LPVOID userData) 
{
	if(1 != type)
	{//������Ƶ����
		return;
	}		
	EloamPrint * pDlg = (EloamPrint *)userData;
	if(1 == dbt)
	{//�豸����
		HELOAMDEVICE hDev = EloamGlobal_CreateDevice(1,idx);	

		if(1 == EloamDevice_GetEloamType(hDev))
		{
			WCHAR * wName = EloamDevice_GetFriendlyName(hDev);
			char buf[MAX_PATH] = {0}; 
			int len = WideCharToMultiByte(CP_ACP,0, wName ,wcslen(wName),NULL,0,NULL,NULL);      
			WideCharToMultiByte(CP_ACP,0,wName,wcslen(wName), buf,len,NULL,NULL);  	

			pDlg->m_vMainDevice.push_back(hDev);
			WCHAR * wNameg =  EloamGlobal_GetDisplayName(1,idx);
			char bufname[MAX_PATH] = {0};
			int leng = WideCharToMultiByte(CP_ACP,0,wNameg,wcslen(wNameg),NULL,0,NULL,NULL);
			WideCharToMultiByte(CP_ACP,0,wNameg,wcslen(wNameg),bufname,leng,NULL,NULL);			
			if(QString::fromLocal8Bit(bufname).mid(33,4) == "1020"||QString::fromLocal8Bit(bufname).mid(33,4) == "10a3")//ͨ��PID������Ĭ�ϵ�DPI //1:1��ӡ����ר��
			{
				pDlg->m_A4DefDpi=302;		
				pDlg->m_A3DefDpi=214;
				pDlg->m_A4HA4WidthDef=3536;
				pDlg->m_A4HA4HeightDef=2500;
				pDlg->m_A4HA3WidthDef=5000;
				pDlg->m_A4HA3HeightDef=3536;
				pDlg->m_A3HA4WidthDef=2500;
				pDlg->m_A3HA4HeightDef=1768;
				pDlg->m_A3HA3WidthDef=3536;
				pDlg->m_A3HA3HeightDef=2500;

			}
			if(QString::fromLocal8Bit(bufname).mid(33,4) == "1502"||QString::fromLocal8Bit(bufname).mid(33,4) == "1500")//ͨ��PID������Ĭ�ϵ�DPI //1:1��ӡ����ר��
			{
				pDlg->m_A4DefDpi=356;
				pDlg->m_A3DefDpi=252;
				pDlg->m_A4HA4WidthDef=4176;
				pDlg->m_A4HA4HeightDef=2954;
				pDlg->m_A4HA3WidthDef=5908;
				pDlg->m_A4HA3HeightDef=4176;
				pDlg->m_A3HA4WidthDef=2954;
				pDlg->m_A3HA4HeightDef=2088; 
				pDlg->m_A3HA3WidthDef=4176;
				pDlg->m_A3HA3HeightDef=2954;
			}
			EloamGlobal_DestroyString(wName);
			pDlg->devFlag=idx;
			pDlg->OpenVideo();

		}

	}
	else if(2 == dbt)
	{//�豸��ʧ		
		if(pDlg->m_vMainDevice.size() <=0)
			return;
		pDlg->OnCloseDevice();
		pDlg->m_vMainDevice.clear();
		pDlg->m_bIsLostDev =true;
		return;
		HELOAMDEVICE hDev = pDlg->m_vMainDevice[idx];
		HELOAMDEVICE hDevTemp = EloamVideo_GetDevice(pDlg->m_hMainVideo);
		if (hDev == hDevTemp)
		{
			pDlg->OnCloseDevice();
			int size = pDlg->m_vMainDevice.size();
			for (int i = 0; i < size; ++i)
			{
				HELOAMDEVICE hDev = pDlg->m_vMainDevice[i];
				EloamDevice_Release(hDev);
			}
			pDlg->m_vMainDevice.clear();
			pDlg->m_bIsLostDev =true;

		}		
		EloamDevice_Release(hDevTemp);
		EloamDevice_Release(hDev);
	}
}

VOID ELOAMAPI EloamPrint::AttachCallback(HELOAMVIDEO video, LONG videoId, HELOAMVIEW view, LONG viewId, LPVOID userData)
{
	EloamPrint *pDlg = (EloamPrint *)userData;
	//return;
	//������Ƶ�򿪺������	
	//QDilgCamera *pDlg = (QDilgCamera *)userData; 
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
	//g_TheDilgAPP->OnScan();
}

VOID ELOAMAPI EloamPrint::ZoomChange(HELOAMVIEW view, LONG flag, LONG value, LPVOID userData)
{	
	//��ƵԤ�����ڴ�С�����ı�ʱ�ص�
	EloamPrint *pDlg = (EloamPrint *)userData;
	if(NULL != pDlg)
	{
		if(flag == 2)
		{
			//pDlg->ui.Slide_Zoom->setValue(value);
		}		
	}	
}