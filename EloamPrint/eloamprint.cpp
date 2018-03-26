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
#include "eloamprint.h"
#include "language\language.h"
#include  <qfileinfo.h>
#include <qmessagebox.h>
#include  <qdir.h>
#include <qdatetime.h>
HWND g_hParent = NULL;
EloamPrint * g_TheDilgAPP = NULL;
#define FILE_EXISTS(FILE_NAME) ((0xFFFFFFFF==GetFileAttributes(FILE_NAME))?FALSE:TRUE)

/*通过传入一个区分各种国家的语言，传入一个ＩＤ号来判断返回此ＩＤ国家的语言*/

bool InitLanguages(QString CountyID)
{
	 char buf[MAX_PATH] = {0};
		GetModuleFileNameA(NULL, buf, MAX_PATH);
		int len = strlen(buf);
		for(int i = len-1; i > 3; i--)
		{
			if(buf[i] != '\\')
			{
				buf[i] = '\0';
			}
			else
			{
				break;
			}
		}

	strcat(buf, "lang.xml");   //获取当前程序exe运行目录下的xml语言包文件。
	readXml(buf);                                                 //读取xml的语言
	char *Cid;
	QByteArray ba = CountyID.toLatin1();
	Cid= ba.data();
	int flag = getCountryLanguage(Cid);                           //根据Cid来选择需要加载哪一个国家的语言
	if(flag == 1)
	return true;
	else
		return false;
}

int  GetFindFile(QString path, QString sPreName,QString CType)
{

    QDir dir(path);
    if (!dir.exists())
	return false;
	int Max = -1;
	int curnum = 0;
	QFileInfoList file_list = dir.entryInfoList(QDir::Files);
	for(int i = 0 ;i<= file_list.size()-1;i++)
	{
		 QFileInfo fileInfo = file_list.at(i);
		 QString path = fileInfo.fileName();
		 QString curPre = path.left(sPreName.length() );
		 QString cNum ;
		 if(curPre == sPreName && path.contains(CType))
		 {
			 cNum = path.mid(sPreName.length()+1,path.length() - sPreName.length()-5);
			 curnum = cNum.toLong();
			 if(Max < curnum )
			 {
				Max = curnum;
			 }
		 }
	}
	return ++Max;
}
//设备回调函数
VOID ELOAMAPI EloamPrint::DevChangeCallback(LONG type, LONG idx, LONG dbt, LPVOID userData) 
{
	if(1 != type)
		return;
	EloamPrint * pDlg = (EloamPrint *)userData;
	if(1 == dbt)
	{
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
		if(QString::fromLocal8Bit(bufname).mid(33,4) == "1020"||QString::fromLocal8Bit(bufname).mid(33,4) == "10a3")//通过PID来设置默认的DPI //1:1打印机型专用
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
		if(QString::fromLocal8Bit(bufname).mid(33,4) == "1502"||QString::fromLocal8Bit(bufname).mid(33,4) == "1500")//通过PID来设置默认的DPI //1:1打印机型专用
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
	{
		//设备丢失
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
	//设置视频打开后的属性	
	//QDilgCamera *pDlg = (QDilgCamera *)userData; 
	if (1 == videoId && 1 == viewId)
	{	
		g_TheDilgAPP->InitDeskew();	
	}	
		
}
VOID ELOAMAPI EloamPrint::ArivlaCallback(HELOAMVIDEO video, LONG videoId, LPVOID userData)
{	
	if (1 == videoId)
	{	
		g_TheDilgAPP->InitExposure();
		g_TheDilgAPP->InitDeskew();
	}	
	
	
}
VOID ELOAMAPI EloamPrint::EloamTouchCallback(HELOAMVIDEO video, LPVOID userData)
{
	//g_TheDilgAPP->OnScan();
}
VOID ELOAMAPI EloamPrint::ZoomChange(HELOAMVIEW view, LONG flag, LONG value, LPVOID userData)
{	
	//视频预览窗口大小发生改变时回调
	EloamPrint *pDlg = (EloamPrint *)userData;
	if(NULL != pDlg)
	{
		if(flag == 2)
		{
			//pDlg->ui.Slide_Zoom->setValue(value);
		}		
	}	
}

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
	,m_hIDCardCombine(NULL)				//二代证合成图片句柄
	,m_hImageReverse(NULL)
	,m_hShowResultImage(NULL)
	,m_hShowResultImageA(NULL)
	,m_nThresholdValue(40)
	,m_nExposureValue(5)
	,m_A3DefDpi(0)
	,m_A4DefDpi(0)
	,m_A4Width(0)
	,m_A4Height(0)
	,m_A3Width(0)
	,m_A3Height(0)
	,m_bIsShowA(false)
	,m_bIsView(true)
	,m_PrintNum(1)
	,m_nIsDeskew(1)							//0表示不纠偏    1表示纠偏
	,m_nPrintSize(0)						//0表示A4        1表示A3
	,m_ischangeDPI(0)					
	,m_nIsCombine(0)						//0表示单张拍照  1表示两张合成
	,m_nIsOrignal(0)
	,m_nIsAutoExposure(1)
{
	m_CurBinPath = QCoreApplication::applicationDirPath();
	m_CurPrintImageSavePath = m_CurBinPath;
	g_TheDilgAPP = this;
	ui.setupUi(this);
	ReadConfig();

	InitFiletype();
	m_vMainDevice.clear();	
	m_hThumbPrint = EloamGlobal_CreateThumbnail((HWND)ui.listWidget_forPrintThumb->winId(),0, 1);
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
	
	//创建视频窗口
	m_hPrintView  = (HWND)ui.W_PrintVideoDlg->winId();
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
		EloamView_SetText(m_PrintView, L"未检测到设备", RGB(255, 255, 255));

	}
	this->setMouseTracking ( true);
	connect(ui.pB_Print,SIGNAL(clicked()),this,SLOT(OnPrintPic()));
	connect(ui.pB_Memu, SIGNAL(clicked()), this, SLOT(OnConfigDlg()));
	connect(ui.pB_ScanPrint, SIGNAL(clicked()), this, SLOT(OnScanPrint()));
	connect(ui.pB_switchView, SIGNAL(clicked()), this, SLOT(OnSwitchView()));
	connect(ui.pB_Cut, SIGNAL(clicked()), this, SLOT(OnCut()));
	connect(ui.m_ThresholdSlider, SIGNAL(valueChanged(int)), ui.spinBox_Threshold, SLOT(setValue(int)));
	connect(ui.spinBox_Threshold, SIGNAL(valueChanged(int)),ui.m_ThresholdSlider, SLOT(setValue(int)));
	connect(ui.m_ThresholdSlider, SIGNAL(valueChanged(int)),this, SLOT(setThresholdValue(int)));
	connect(ui.spinBox_Threshold, SIGNAL(valueChanged(int)),this, SLOT(setThresholdValue(int)));
	connect(ui.cB_SelectAll,SIGNAL(clicked()),this,SLOT(OnCbSelectAll()));
	ui.m_ThresholdSlider->setValue(m_nThresholdValue);
	ui.spinBox_Threshold->setValue(m_nThresholdValue);
	ui.m_ThresholdSlider->setVisible(false);
	ui.spinBox_Threshold->setVisible(false);
	ui.label_3->setVisible(false);
	ui.pB_Cut->setVisible(false);
	ui.label_cut->setText("");
	ui.label_kongbai->setVisible(false);

	if(!m_ischangeDPI)
	{
		m_CurXDpi = m_A4DefDpi;
		m_CurYDpi = m_A4DefDpi;
		m_A4Width  = m_A4HA4WidthDef;
		m_A4Height = m_A4HA4HeightDef;
		m_A3Width  = m_A4HA3WidthDef;
		m_A3Height = m_A4HA3HeightDef;
	}
	InitPrintConfigDlg();
	SetButtonStyle();
	SetLanguage();
}

EloamPrint::~EloamPrint()
{
	if ( NULL != m_hPrintImage)//打印窗口的图片句柄
	{
		EloamImage_Release(m_hPrintImage);
		m_hPrintImage = NULL;
	}
	if ( NULL != m_hShowResultImage)//用于显示二值化效果的图片对象
	{
		EloamImage_Release(m_hShowResultImage);
		m_hShowResultImage = NULL;
	}
	if ( NULL != m_hShowResultImageA)//用于显示二值化效果的图片对象a
	{
		EloamImage_Release(m_hShowResultImageA);
		m_hShowResultImageA = NULL;
	}

	if ( NULL != m_hImageReverse)//打印窗口的图片句柄
	{
		EloamImage_Release(m_hImageReverse);
		m_hImageReverse = NULL;
	}

	if ( NULL != m_hIDCardCombine)//二代证合成图片句柄
	{
		EloamImage_Release(m_hIDCardCombine);
		m_hPrintImage = NULL;
	}
	if ( NULL != m_IDCardFirstImage)//二代证合成的第一张图片
	{
		EloamImage_Release(m_IDCardFirstImage);
		m_IDCardFirstImage = NULL;
	}
	if ( NULL != m_hCutRect)//二代证合成的第一张图片
	{
		EloamRect_Release(m_hCutRect);
		m_hCutRect = NULL;
	}
	if ( NULL != m_hThumbPrint)//二代证合成的第一张图片
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
	delete m_PrintConfigDlg;
	WriteConfig();
}
void EloamPrint::InitPrintConfigDlg()
{
	m_PrintConfigDlg = new QWidget();
	PrintConfig_ui.setupUi(m_PrintConfigDlg);
	m_PrintConfigDlg->setWindowFlags(Qt::Window);
	m_PrintConfigDlg->hide();
	//connect(PrintConfig_ui.rB_Deskew,SIGNAL(clicked()),this,SLOT(OnCbDeskew()));
	connect(PrintConfig_ui.rB_A3,SIGNAL(clicked()),this,SLOT(OnCbA3()));
	connect(PrintConfig_ui.rB_A4_2,SIGNAL(clicked()),this,SLOT(OnCbA4()));
	connect(PrintConfig_ui.cB_AutoExposure,SIGNAL(clicked()),this,SLOT(OnCbAutoExposure()));
	connect(PrintConfig_ui.rB_A4,SIGNAL(clicked()),this,SLOT(OnRbA4()));
	connect(PrintConfig_ui.rB_A5,SIGNAL(clicked()),this,SLOT(OnRbA5()));
	connect(PrintConfig_ui.pB_Restore,SIGNAL(clicked()),this,SLOT(OnRestore()));
	connect(PrintConfig_ui.pB_CheckDPI, SIGNAL(clicked()), this, SLOT(OnCheckDPI()));
	connect(PrintConfig_ui.spB_PrintNum, SIGNAL(valueChanged(int)), this, SLOT(OnSetPrintNum(int)));
	connect(PrintConfig_ui.pB_Sure, SIGNAL(clicked()), this, SLOT(OnSure()));
	connect(PrintConfig_ui.pB_Cancel, SIGNAL(clicked()), this, SLOT(OnCancel()));
	connect(PrintConfig_ui.pB_SelectSavePath, SIGNAL(clicked()), this, SLOT(OnSelectSavePath()));
	connect(PrintConfig_ui.pB_Help, SIGNAL(clicked()), this, SLOT(OnHelp()));
	connect(PrintConfig_ui.Slider_Exposure, SIGNAL(valueChanged(int)), PrintConfig_ui.spinBox_exposure, SLOT(setValue(int)));
	connect(PrintConfig_ui.spinBox_exposure, SIGNAL(valueChanged(int)),PrintConfig_ui.Slider_Exposure, SLOT(setValue(int)));
	connect(PrintConfig_ui.Slider_Exposure, SIGNAL(valueChanged(int)),this, SLOT(setExposureValue(int)));
	connect(PrintConfig_ui.spinBox_exposure, SIGNAL(valueChanged(int)),this, SLOT(setExposureValue(int)));
	
	PrintConfig_ui.cB_AutoExposure->setChecked(m_nIsAutoExposure);//m_nIsAutoExposure   0表示手动曝光  1表示自动曝光
	PrintConfig_ui.rB_Deskew->setChecked(m_nIsDeskew);		//m_nIsDeskew  0表示不纠偏   1表示纠偏
	PrintConfig_ui.cB_IDCard->setChecked(m_nIsCombine);							
	PrintConfig_ui.cB_Original->setChecked(m_nIsOrignal);  // m_nIsOrignal 0表示单张拍照  1表示两张合成					
	if(0 == m_nPrintSize)									//m_nPrintSize0表示A4        1表示A3
	{
		PrintConfig_ui.rB_A4_2 ->setChecked(true);
		PrintConfig_ui.rB_A3 ->setChecked(false);
	}
	else
	{
		PrintConfig_ui.rB_A4_2 ->setChecked(false);
		PrintConfig_ui.rB_A3 ->setChecked(true);
	}
	PrintConfig_ui.spB_PrintNum->setValue(m_PrintNum);
	PrintConfig_ui.lineEdit_SavePath->setText(m_CurPrintImageSavePath);
	m_PrintNumTem = m_PrintNum;
	m_nIsDeskewTem = m_nIsDeskew;			//0表示不纠偏    1表示纠偏
	m_nPrintSizeTem = m_nPrintSize;			//0表示A4        1表示A3
	m_nIsCombineTem = m_nIsCombine;		//0表示单张拍照  1表示两张合成
	m_nIsOrignalTem = m_nIsOrignal;
	m_nIsAutoExposureTem = m_nIsAutoExposure;
	m_nExposureValueTem = m_nExposureValue;
	//PrintConfig_ui.Slider_Exposure->setValue(m_nIsAutoExposureTem);
	//PrintConfig_ui.spinBox_exposure->setValue(m_nIsAutoExposureTem);
}
void EloamPrint::closeEvent(QCloseEvent *event)
{
  
}


void EloamPrint::OnConfigDlg()
{
	m_PrintConfigDlg->show();
	QPoint pt(0,0);
	m_PrintConfigDlg->move(pt);
	
}

	
//检测DPI
void EloamPrint::OnCheckDPI()
{ 
	QString strLid;
	QString strName;
	strLid ="STRING_PRINT_TIPS";
	QString m_printInfo = getlanguageAbbr(strLid);
	strLid ="STRING_PRINT_PUTA4_CARD";
	QString m_printMessge =  getlanguageAbbr(strLid);
	strLid ="STRING_PRINT_PUTA5_CARD";
	QString m_printMessge2 = getlanguageAbbr(strLid);
	strLid ="STRING_PRINT_CHECKDPI_OUTRANGE";
	QString m_printMessge3 = getlanguageAbbr(strLid);
	strLid ="STRING_PRINT_CHECKDPI_COMPLETE";
	QString m_printMessge4 = getlanguageAbbr(strLid);
	int m_temp=0;
	if(1 == m_nCheckSize)
	{
		QMessageBox::information(NULL,m_printInfo,m_printMessge,NULL);	

		HELOAMIMAGE hImage = EloamVideo_CreateImage(m_hMainVideo, 0, m_PrintView);
		m_A4Width = EloamImage_GetWidth(hImage);
		m_A4Height = EloamImage_GetHeight(hImage);
		if( m_A4Width > m_A4Height)
		{
			if(PrintConfig_ui.rB_Deskew->isChecked())
			{
				m_A4Width = m_A4Width +20;
				m_A4Height = m_A4Height +23;
			}
			m_A3Width = m_A4Height*2;
			m_A3Height = m_A4Width;
			m_CurXDpi = (m_A4Width * 254.0 ) / 2970.0;
			m_CurYDpi = (m_A4Height * 254.0) / 2100.0;
		}
		else
		{	if(!PrintConfig_ui.rB_Deskew->isChecked())
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
			m_CurXDpi=(m_A4Width * 25.4 )/2100.0;
			m_CurYDpi=(m_A4Height * 25.4 )/2970.0;
		}

		EloamImage_Release(hImage);
		hImage = NULL;
	}
	else
	{
		QMessageBox::information(NULL,m_printInfo,m_printMessge2,NULL);	
		int m_A5Width,m_A5Height;
		HELOAMIMAGE hImage = EloamVideo_CreateImage(m_hMainVideo, 0, m_PrintView);
		m_A5Width = EloamImage_GetWidth(hImage);
		m_A5Height = EloamImage_GetHeight(hImage);

		if( m_A5Width > m_A5Height)
		{
			if(PrintConfig_ui.rB_Deskew->isChecked())
			{
				m_A5Width = m_A5Width +18;
				m_A5Height = m_A5Height +22;
			}
			m_A4Width = m_A5Height*2;
			m_A4Height = m_A5Width;
			m_A3Width = m_A4Height*2;
			m_A3Height = m_A4Width;
			m_CurXDpi = (m_A5Width * 254.0 ) / 2100.0;
			m_CurYDpi = (m_A5Height * 254.0) / 1485.0;
		}
		else
		{
			if(!PrintConfig_ui.rB_Deskew->isChecked())
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
			m_CurXDpi=(m_A4Width * 25.4 )/1485.0;
			m_CurYDpi=(m_A4Height * 25.4 )/2100.0;
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
void EloamPrint::OnRestore()
{
	m_ischangeDPI = 0;
	
	if(PrintConfig_ui.rB_A3->isChecked())
	{
		m_CurXDpi=m_A3DefDpi;
		m_CurYDpi=m_A3DefDpi;

		m_A4Width  =  m_A3HA4WidthDef;
		m_A4Height =  m_A3HA4HeightDef;

		m_A3Width =  m_A3HA3WidthDef;
		m_A3Height = m_A3HA3HeightDef;

	}
	else
	{
		m_CurXDpi=m_A4DefDpi;
		m_CurYDpi=m_A4DefDpi;

		m_A4Width  =  m_A4HA4WidthDef;
		m_A4Height =  m_A4HA4HeightDef;

		m_A3Width  = m_A4HA3WidthDef;
		m_A3Height = m_A4HA3HeightDef;
	}
}

void EloamPrint::GetSelectState()
{

	m_nIsDeskewTem = PrintConfig_ui.rB_Deskew->isChecked();
	
	if(PrintConfig_ui.rB_A3->isChecked())
	{
		m_nPrintSizeTem = 1;
	}
	if(PrintConfig_ui.rB_A4_2->isChecked())
	{
		m_nPrintSizeTem = 0;
	}
	
	m_nIsCombineTem = PrintConfig_ui.cB_IDCard->isChecked();
	
	m_nIsOrignalTem = PrintConfig_ui.cB_Original->isChecked();
	
	m_PrintNumTem  = PrintConfig_ui.spB_PrintNum->value();
	
}

void EloamPrint::OnSure()
{
	GetSelectState();
	if(m_nIsDeskew != m_nIsDeskewTem)
	{
		m_nIsDeskew=m_nIsDeskewTem;
		PrintConfig_ui.rB_Deskew->setChecked(m_nIsDeskew);
		OnCbDeskew();
	}
	if(m_nPrintSize != m_nPrintSizeTem)
	{
		m_nPrintSize=m_nPrintSizeTem;
		if(m_nPrintSize==0)
		{
			OnScanSizeA4();
		}else
		{
			OnScanSizeA3();
		}

	}
	if(m_nIsCombine != m_nIsCombineTem)
	{
		m_nIsCombine = m_nIsCombineTem;
		PrintConfig_ui.cB_IDCard->setChecked(m_nIsCombine);
	}
	if(m_nIsOrignal != m_nIsOrignalTem)
	{
		m_nIsOrignal = m_nIsOrignalTem;
		PrintConfig_ui.cB_Original->setChecked(m_nIsOrignal);
	}

	if(m_PrintNum != m_PrintNumTem )
	{
		m_PrintNum = m_PrintNumTem;
	}

	if(m_nIsAutoExposure != m_nIsAutoExposureTem)
	{
		m_nIsAutoExposure = m_nIsAutoExposureTem;
		if(m_nIsAutoExposure)
		{
			EloamDevice_SetCameraControl(m_hMainDev, 0x1, 0, TRUE);
		}
		else
		{
			if(m_nExposureValue != m_nExposureValueTem )
			{
				m_nExposureValue = m_nExposureValueTem ;
			}
			EloamDevice_SetCameraControl(m_hMainDev, 1, m_nExposureValue, false);
		}
	}
	
	m_PrintConfigDlg->hide();
}
void  EloamPrint::OnCancel()
{
	GetSelectState();
	if(m_nIsDeskew != m_nIsDeskewTem)
	{
		PrintConfig_ui.rB_Deskew->setChecked(m_nIsDeskew);
	}
	if(m_nPrintSize != m_nPrintSizeTem)
	{
		if(m_nPrintSize==0)
		{
			PrintConfig_ui.rB_A4_2->setChecked(true);
			PrintConfig_ui.rB_A3->setChecked(false);
		}
		else
		{
			PrintConfig_ui.rB_A4_2->setChecked(false);
			PrintConfig_ui.rB_A3->setChecked(true);
		}
	}
	if(m_nIsCombine != m_nIsCombineTem)
	{
		PrintConfig_ui.cB_IDCard->setChecked(m_nIsCombine);
	}
	if(m_nIsOrignal != m_nIsOrignalTem)
	{
		PrintConfig_ui.cB_Original->setChecked(m_nIsOrignal);
	}
	if(m_PrintNum != m_PrintNumTem )
	{
		PrintConfig_ui.spB_PrintNum->setValue(m_PrintNum);
	}
	if(m_nIsAutoExposure != m_nIsAutoExposureTem)
	{
		PrintConfig_ui.cB_AutoExposure->setChecked(m_nIsAutoExposure);
		if(m_nIsAutoExposure)
		{
			EloamDevice_SetCameraControl(m_hMainDev, 0x1, 0, TRUE);
		}
		else
		{
			if(m_nExposureValue != m_nExposureValueTem )
			{
				PrintConfig_ui.Slider_Exposure->setValue(m_nExposureValue);
			}	
		}
	}


	m_PrintConfigDlg->hide();
}

void EloamPrint::ReversePrintImage(HELOAMIMAGE m_hPrintImage)
{	
	if(m_hImageReverse != NULL)
	{
		EloamImage_Release(m_hImageReverse);
		m_hImageReverse = NULL;
	}
	if(PrintConfig_ui.rB_A3->isChecked())
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
		EloamImage_SetXDPI(m_hImageReverse,m_CurXDpi);
		EloamImage_SetYDPI(m_hImageReverse,m_CurYDpi);
		HELOAMRECT rectDest1 = EloamGlobal_CreateRect((m_A4Width-w1)/2+20, (m_A4Height-h1)/2+20, w1-40,h1-40);
		HELOAMRECT rectSrc1 = EloamGlobal_CreateRect(20, 20, w1-40, h1-40);
		EloamImage_Blend(m_hImageReverse, rectDest1, m_hPrintImage, rectSrc1, 0, 0);
		EloamRect_Release(rectDest1);
		EloamRect_Release(rectSrc1);
	}
}

void EloamPrint::OnPrintPic()										//打印当前图片
{
	QString strLid;
	strLid ="STRING_PRINT_TIPS";
	QString m_printInfo = getlanguageAbbr(strLid);
	strLid ="STRING_PRINT_CHECK_SELECTED";
	QString m_printMessge =  getlanguageAbbr(strLid);
	HELOAMIMAGELIST imgList = EloamGlobal_CreateImageList();	           
	if(false == m_bIsView)//如果当前为打印预览状态，直接打印当前预览的图片
	{
		if(NULL != m_hCutRect)
		{
			m_hPrintImage = EloamGlobal_CreateImageFromFile((BSTR)(LPCWSTR)m_sCutImagePath.data(),0);
		}
		else
		{
			m_hPrintImage = EloamGlobal_CreateImageFromFile((BSTR)(LPCWSTR)m_sShowResultImagePath.data(),0);
			if(!PrintConfig_ui.cB_Original->isChecked())
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
					 
					//还原经过放大的图片
					if(NULL != m_hPrintImage)
					{
						EloamImage_Release(m_hPrintImage);
						m_hPrintImage = NULL;
					}
					m_hPrintImage = EloamImageList_GetImage(imgList,i);
					ReversePrintImage(m_hPrintImage);
					
					if(m_PrintNum != 0)
					{
						for(int i = 0 ; i < m_PrintNum ; i++)
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

void EloamPrint::OnSetPrintNum(int num)									//设置打印的份数
{ 
	bool ok;
	m_PrintNumTem=num;
}

void EloamPrint::OnStopPrint()
{
	QString strLid;
	strLid ="STRING_PRINT_TIPS";
	QString m_printInfo = getlanguageAbbr(strLid);

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
	m_PrintNum=1;
	
}
void EloamPrint::OnCbSelectAll()
{
	bool setCheck = ui.cB_SelectAll->isChecked();
	int count = EloamThumbnail_GetCount(m_hThumbPrint);
	for (int i = 0; i < count; i++)
	{
		EloamThumbnail_SetCheck(m_hThumbPrint,i,setCheck);
						
	}
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
		if(!PrintConfig_ui.cB_Original->isChecked())
		{
			hImageCombine=EloamGlobal_CreateImage(m_A3Width, m_A3Height,1);
		}else 
		{	
			hImageCombine=EloamGlobal_CreateImage(m_A3Width, m_A3Height,3);
		}
		if((w1>m_A3Width)||(h1>m_A3Height))
		{
			//请检查拍摄文件尺寸是否大于A3
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
		
		if(!PrintConfig_ui.cB_Original->isChecked())
		{
			hImageCombine=EloamGlobal_CreateImage(m_A3Height,m_A3Width, 1);
		}else 
		{	
			hImageCombine=EloamGlobal_CreateImage(m_A3Height, m_A3Width,3);
		}
		if((w1>m_A3Height)||(h1>m_A3Width))
		{
			//请检查拍摄文件尺寸是否大于A3
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

		EloamImage_SetXDPI(hImageCombine,m_CurXDpi);
		EloamImage_SetYDPI(hImageCombine,m_CurYDpi);
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
		if(!PrintConfig_ui.cB_Original->isChecked())
		{
			hImageCombine=EloamGlobal_CreateImage(m_A4Width, m_A4Height,1);
		}else 
		{	
			hImageCombine=EloamGlobal_CreateImage(m_A4Width, m_A4Height,3);
		}
		if((w1>m_A4Width)||(h1>m_A4Height))
		{
			//请检查拍摄文件尺寸是否大于A4
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
		
		if(!PrintConfig_ui.cB_Original->isChecked())
		{
			hImageCombine=EloamGlobal_CreateImage(m_A4Height,m_A4Width, 1);
		}else 
		{	
			hImageCombine=EloamGlobal_CreateImage(m_A4Height, m_A4Width,3);
		}
		if((w1>m_A4Height)||(h1>m_A4Width))
		{
			//请检查拍摄文件尺寸是否大于A4
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
		
		EloamImage_SetXDPI(hImageCombine,m_CurXDpi);
		EloamImage_SetYDPI(hImageCombine,m_CurYDpi);
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
		if(!PrintConfig_ui.cB_Original->isChecked())
		{
			hImageCombine=EloamGlobal_CreateImage(m_A4Width, m_A4Height,1);
		}else 
		{	
			hImageCombine=EloamGlobal_CreateImage(m_A4Width, m_A4Height,3);
		}
		if((w1>m_A4Width)||(h1>m_A4Height))
		{
			//请检查拍摄文件尺寸是否大于A4
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
		
		if(!PrintConfig_ui.cB_Original->isChecked())
		{
			hImageCombine=EloamGlobal_CreateImage(m_A4Height,m_A4Width, 1);
		}else 
		{	
			hImageCombine=EloamGlobal_CreateImage(m_A4Height, m_A4Width,3);
		}
		if((w1>m_A4Height)||(h1>m_A4Width))
		{
			//请检查拍摄文件尺寸是否大于A4
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
		EloamImage_SetXDPI(hImageCombine,m_CurXDpi);
		EloamImage_SetYDPI(hImageCombine,m_CurYDpi);
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

void EloamPrint::A4IDCardCombineProcess(HELOAMIMAGE hImg,QString imgPath)
{	
	QString strLid;	
	strLid ="STRING_PRINT_TIPS";
	QString m_printInfo = getlanguageAbbr(strLid);
	strLid ="STRING_PRINT_IDCARDCOMBINE_START";
	QString m_printMessge = getlanguageAbbr(strLid);
	strLid ="STRING_PRINT_IDCARDCOMBINE_END";
	QString m_printMessge2 =getlanguageAbbr(strLid);
	strLid ="STRING_PRINT_IDCARDCOMBINE_OVERRANGEA5";
	QString m_printMessge3 =getlanguageAbbr(strLid);
	strLid ="STRING_PRINT_IDCARDCOMBINE_OVERRANGEA4";
	QString m_printMessge4 =getlanguageAbbr(strLid);
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
		if(PrintConfig_ui.rB_A4_2->isChecked())
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
		EloamImage_SetXDPI(m_hIDCardCombine,m_CurYDpi);
		EloamImage_SetYDPI(m_hIDCardCombine,m_CurXDpi);
		//第二张身份证照片的区域
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
		if(!PrintConfig_ui.cB_Original->isChecked())
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
		if(!PrintConfig_ui.cB_Original->isChecked())
		{
			m_hIDCardCombine = EloamGlobal_CreateImage(width, height,1);
		}else 
		{	
			m_hIDCardCombine = EloamGlobal_CreateImage(width, height,3);
		}
		if(m_hIDCardCombine)
		{
			EloamImage_SetXDPI(m_hIDCardCombine,m_CurYDpi);
			EloamImage_SetYDPI(m_hIDCardCombine,m_CurXDpi);
		}
		//第一张身份证照片的区域
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
		strLid ="STRING_PRINT_OTHERHAND";
		m_printMessge = getlanguageAbbr(strLid);
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
	if(!PrintConfig_ui.cB_Original->isChecked())
	{
		EloamImage_AdaptiveThreshold(m_hPrintImage,m_nThresholdValue);
	}
	if(!m_hPrintImage)return;
	if(m_hPrintImage)                //系统时间
	{
		QDateTime Datetime = QDateTime::currentDateTime();
			QString Ctime = Datetime.toString("yyyyMMdd_hhmmss");
			QString FileSaveName;
			FileSaveName =m_CurPrintImageSavePath + QString::fromLocal8Bit("/") + Ctime + m_sImageSaveType;
			A4IDCardCombineProcess(m_hPrintImage,FileSaveName);
	}
}


void EloamPrint::OnScanPrintA3()
{
	if(NULL != m_hPrintImage)
	{
		EloamImage_Release(m_hPrintImage);
		m_hPrintImage = NULL;
	}
	
	m_hPrintImage = EloamVideo_CreateImage(m_hMainVideo, 0, m_PrintView);
	
	if(!m_hPrintImage)return;
	if(m_hPrintImage)                //系统时间
	{
		QDateTime Datetime = QDateTime::currentDateTime();
			QString Ctime = Datetime.toString("yyyyMMdd_hhmmss");
			QString FileSaveName;
			FileSaveName =m_CurPrintImageSavePath + QString::fromLocal8Bit("/") + Ctime + m_sImageSaveType;
			EloamImage_AdaptiveThreshold(m_hPrintImage,m_nThresholdValue);
			A3CombineProcess(m_hPrintImage,FileSaveName);
	}
	
}

void EloamPrint::OnScanPrint()
{
	if(PrintConfig_ui.cB_IDCard->isChecked())
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
	
	if(NULL == m_hPrintImage)return;
	else            //系统时间
	{
			QDateTime Datetime = QDateTime::currentDateTime();
			QString Ctime = Datetime.toString("yyyyMMdd_hhmmss");
			QString FileSaveName;
			FileSaveName =m_CurPrintImageSavePath + QString::fromLocal8Bit("/") + Ctime + m_sImageSaveType;
			
			if(!PrintConfig_ui.cB_Original->isChecked())
			EloamImage_AdaptiveThreshold(m_hPrintImage,m_nThresholdValue);
			
			if( PrintConfig_ui.rB_A3->isChecked())
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
void EloamPrint::OnCut()
{
	QString strLid;
	QString strName;
	if(false == m_bIsCut)
	{
		EloamView_SetState(m_PrintView, 2);
		m_bIsCut= true;
		//ui.pB_Cut->setText(QString::fromLocal8Bit("确认裁剪"));
		strLid ="STRING_PRINT_SURECUT";
		strName = getlanguageAbbr(strLid);
		ui.label_cut->setText(strName);
		ui.pB_Cut->setToolTip(strName);
		
	}
	else
	{
		strLid ="STRING_PRINT_CUT";
		strName = getlanguageAbbr(strLid);
		ui.label_cut->setText(strName);
		ui.pB_Cut->setToolTip(strName);
		HELOAMRECT m_hCutSelectRect=EloamView_GetSelectedRect(m_PrintView);
		int Sy=EloamRect_GetTop(m_hCutSelectRect);
		int Sx=EloamRect_GetLeft(m_hCutSelectRect);
		int Sw = EloamRect_GetWidth(m_hCutSelectRect);
		int Sh= EloamRect_GetHeight(m_hCutSelectRect);
		
		int Vw = EloamVideo_GetWidth(m_hMainVideo);
		int Vh = EloamVideo_GetHeight(m_hMainVideo);
		
		if(NULL!=m_hCutSelectRect)
		{
			if(m_sShowResultImagePath != NULL)
			{
				//将显示预览效果的图片进行剪裁，并覆盖保存，再进行二值化调整时以剪裁后的为基础
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
				
				//将改图片的二值化效果保存为另一张图片
				if(!PrintConfig_ui.cB_Original->isChecked())
				{
					EloamImage_AdaptiveThreshold(m_hCutImage,m_nThresholdValue);
				}
				QDateTime Datetime = QDateTime::currentDateTime();
				QString Ctime = Datetime.toString("yyyyMMdd_hhmmss");
				m_sCutImagePath = m_CurPrintImageSavePath + QString::fromLocal8Bit("/") + Ctime + m_sImageSaveType;
				EloamView_SelectImage(m_PrintView,m_hCutImage);
				A4CombineProcessB(m_hCutImage,m_sCutImagePath);
				EloamThumbnail_Add(m_hThumbPrint,(BSTR)(LPCWSTR)m_sCutImagePath.data());

			}
		 
		}
		
		//释放区域对象
		EloamRect_Release(m_hCutSelectRect);
		m_hCutSelectRect= NULL;
		m_bIsCut= false;
		EloamView_SetSelectedRect(m_PrintView, NULL);
		EloamView_SetState(m_PrintView, 1);	
		//ui.pB_Cut->setText(QString::fromLocal8Bit("启动裁剪"));
	}

}
void EloamPrint::OnSwitchView()            //切回视频画面
{
	QString strLid;	
	strLid ="STRING_PRINT_TIPS";
	QString m_printInfo = getlanguageAbbr(strLid);
	strLid ="STRING_PRINT_SCAN_FIRST";
	QString m_printMessge =  getlanguageAbbr(strLid);
	QString strName;
	if(false == m_bIsView)
	{
		
		EloamView_SelectVideo(m_PrintView,m_hMainVideo,NULL,this);
		m_bIsView = true;
		m_bIsCut = false;
		strLid ="STRING_PRINT_CUT";
		strName = getlanguageAbbr(strLid);
		ui.label_cut->setText(strName);
		ui.pB_Cut->setToolTip(strName);
		//ui.pB_switchView->setText(QString::fromLocal8Bit("打印预览"));
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
		strLid ="STRING_DAYING_YULAN";
		strName = getlanguageAbbr(strLid);
		ui.label_PreView->setText(strName);
		ui.pB_switchView->setToolTip(strName);
		ui.label_3->setVisible(false);
		ui.m_ThresholdSlider->setVisible(false);
		ui.spinBox_Threshold->setVisible(false);
		ui.pB_Cut->setVisible(false);
		ui.label_cut->setText("");
		ui.pB_ScanPrint->setEnabled(true);
		ui.pB_ScanPrint->setVisible(true);
		strLid ="STRING_PRINT_SCAN";
		strName = getlanguageAbbr(strLid);
		ui.label_Scan->setText(strName);
		PrintConfig_ui.pB_CheckDPI->setEnabled(true);
	}
	else
	{
			
			m_bIsView = false;
			strLid ="STRING_PRINT_VIDEOPREVIEW";
			strName = getlanguageAbbr(strLid);
			ui.label_PreView->setText(strName);
			ui.pB_switchView->setToolTip(strName);	
			ui.pB_ScanPrint->setEnabled(false);
			strLid ="STRING_PRINT_CUT";
			strName = getlanguageAbbr(strLid);
			ui.label_cut->setText(strName);
			ui.pB_Cut->setVisible(true);
			//ui.label_cut->setVisible(true);
			ui.label_3->setVisible(true);
			ui.m_ThresholdSlider->setVisible(true);
			ui.spinBox_Threshold->setVisible(true);
			ui.pB_ScanPrint->setVisible(false);
			ui.label_Scan->setText("");
			PrintConfig_ui.pB_CheckDPI->setEnabled(false);
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
			QDateTime Datetime = QDateTime::currentDateTime();
			QString Ctime = Datetime.toString("yyyyMMdd_hhmmss");
			m_sShowResultImagePath = m_CurPrintImageSavePath + QString::fromLocal8Bit("/") + Ctime + m_sImageSaveType;
			m_hShowResultImage = EloamVideo_CreateImage(m_hMainVideo,0,m_PrintView);
			EloamImage_Save(m_hShowResultImage,(BSTR)(LPCWSTR)m_sShowResultImagePath.data(),0);
			if(!PrintConfig_ui.cB_Original->isChecked())
			{
				EloamImage_AdaptiveThreshold(m_hShowResultImage,m_nThresholdValue); 
				Datetime = QDateTime::currentDateTime();
				Ctime = Datetime.toString("yyyyMMdd_hhmmss");
				QString m_sImagePath = m_CurPrintImageSavePath + QString::fromLocal8Bit("/") + Ctime + QString::fromLocal8Bit("1")+ m_sImageSaveType;
				A4CombineProcess(m_hShowResultImage,m_sImagePath);
			}
			else
			{	
				Ctime = Datetime.toString("yyyyMMdd_hhmmss");
				QString m_sImagePath = m_CurPrintImageSavePath + QString::fromLocal8Bit("/") + Ctime + m_sImageSaveType;
				A4CombineProcess(m_hShowResultImage,m_sImagePath);
			}
			EloamView_SelectImage(m_PrintView,m_hShowResultImage);	
	}


}

void EloamPrint::setThresholdValue(int Value)
{
		int val = ui.m_ThresholdSlider->value();
		m_nThresholdValue = val;
		
		if(m_sShowResultImagePath != NULL)
		{
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

}
void EloamPrint::setExposureValue(int value)
{
	int val = PrintConfig_ui.Slider_Exposure->value();
	m_nExposureValueTem = val;
	bool b=EloamDevice_SetCameraControl(m_hMainDev, 1, m_nExposureValueTem, false);
	
}
void EloamPrint::OnScanSizeA3()
{
	PrintConfig_ui.rB_A4_2->setChecked(false);
	if(m_ischangeDPI == 0)
	{
		m_CurXDpi = m_A3DefDpi;
		m_CurYDpi = m_A3DefDpi;
		m_A4Width = m_A3HA4WidthDef;
		m_A4Height= m_A3HA4HeightDef;
		m_A3Width = m_A3HA3WidthDef;
		m_A3Height = m_A3HA3HeightDef;
	}
}

void EloamPrint::OnScanSizeA4()
{
	PrintConfig_ui.rB_A3->setChecked(false);
	if(m_ischangeDPI == 0)
	{
		m_CurXDpi = m_A4DefDpi;
		m_CurYDpi = m_A4DefDpi;
		m_A4Width = m_A4HA4WidthDef;
		m_A4Height =  m_A4HA4HeightDef;
		m_A3Width = m_A4HA3WidthDef;
		m_A3Height = m_A4HA3HeightDef;
	}
}
//A3幅面
void EloamPrint::OnCbA3()
{
	m_nPrintSizeTem = 0;
	PrintConfig_ui.rB_A3->setChecked(true);
	PrintConfig_ui.rB_A4_2->setChecked(false);
}

void EloamPrint::OnCbA4()
{
	m_nPrintSizeTem = 0;
	PrintConfig_ui.rB_A4_2->setChecked(true);
	PrintConfig_ui.rB_A3->setChecked(false);
}
//自动曝光
void EloamPrint::OnCbAutoExposure()
{

	if(PrintConfig_ui.cB_AutoExposure->isChecked())
	{	
		PrintConfig_ui.Slider_Exposure->setEnabled(FALSE);
		PrintConfig_ui.spinBox_exposure->setEnabled(FALSE);
		m_nIsAutoExposureTem = 1;
		EloamDevice_SetCameraControl(m_hMainDev, 0x1, 0, TRUE);
	}
	else
	{
		PrintConfig_ui.Slider_Exposure->setEnabled(TRUE);
		PrintConfig_ui.spinBox_exposure->setEnabled(TRUE);
		m_nIsAutoExposureTem = -2;
		EloamDevice_SetCameraControl(m_hMainDev, 1, m_nExposureValue, false);
	}

}
	//设置检测卡标准为A4的尺寸
void EloamPrint::OnRbA4()
{
	 m_nCheckSize = 1;
}
	//设置检测卡标准为A5的尺寸
void EloamPrint::OnRbA5()
{
	m_nCheckSize = 0;
}

	//设置自动纠偏
void EloamPrint::OnCbDeskew()
{
	if( PrintConfig_ui.rB_Deskew->isChecked())
	{
		EloamVideo_DisableDeskew(m_hMainVideo);
		EloamVideo_EnableDeskew(m_hMainVideo,true);
	}
	else
	{	
		
		EloamVideo_DisableDeskew(m_hMainVideo);
		
	}
}
	//设置手动框选
void EloamPrint::OnCbHandCrop()
{

}
//打开帮助文档
void EloamPrint::OnHelp()
{
	QString path = m_CurBinPath + QString::fromLocal8Bit("/EloamPrint.chm");
	ShellExecute(NULL, L"open",(BSTR)(LPCWSTR)path.data(), NULL, NULL, SW_NORMAL);
}
//选择保存路径
void EloamPrint::OnSelectSavePath()
{
	QString file_full, file_name,file_path,file_suffix;
	QFileInfo fileinfo;

	file_full = QFileDialog::getExistingDirectory(this,QString::fromLocal8Bit("保存路径"),m_CurPrintImageSavePath);
	if(!file_full.isEmpty())
	{
		m_CurPrintImageSavePath = file_full;
		PrintConfig_ui.lineEdit_SavePath->setText(m_CurPrintImageSavePath);
	}
}

void EloamPrint::SetLanguage()
{
	//QMessageBox::information( 0,QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("11111") ,0);	
	if(!InitLanguages(QString::number(m_bLanguageIndex)))//(g_CountrId))
	{
		return;
	 }
	QString strLid;
	QString strName;
	//打印窗口
	QString sfont = QString::fromLocal8Bit("<font color='blue' size='4' weight ='500'>");
	QString sfontl = QString::fromLocal8Bit("</font>");

	strLid ="STRING_DAYING";
	strName = getlanguageAbbr(strLid);	
	setWindowTitle(strName);
	//QMessageBox::information( 0,QString::fromLocal8Bit("提示"), strName ,0);
	strLid ="STRING_PRINT_CONFIG";
	strName = getlanguageAbbr(strLid);
	ui.label_Config->setText(strName);
	
	strLid ="STRING_PRINT_ADJUSTMENT";
	strName = getlanguageAbbr(strLid);
	ui.label_3->setText(strName);
	
	strLid ="STRING_PRINT_CUT";
	strName = getlanguageAbbr(strLid);
	ui.label_cut->setText("");

	strLid ="STRING_DAYING_YULAN";
	strName = getlanguageAbbr(strLid);
	ui.label_PreView->setText(strName);

	strLid ="STRING_PRINT_SCAN";
	strName = getlanguageAbbr(strLid);
	ui.label_Scan->setText(strName);
	
	strLid ="STRING_DAYING";
	strName = getlanguageAbbr(strLid);
	ui.label_Print->setText(strName);
	
	strLid ="STRING_PRINT_SELECTALL";
	strName = getlanguageAbbr(strLid);
	ui.cB_SelectAll->setText(strName);
	
	//打印设置窗口
	strLid ="STRING_PRINT_CONFIG";
	strName = getlanguageAbbr(strLid);
	m_PrintConfigDlg->setWindowTitle(strName);

	strLid ="STRING_PRINT_DESKEW";
	strName = getlanguageAbbr(strLid);
	PrintConfig_ui.label_Deskew->setText(strName);

	strLid ="STRING_PRINT_AUTOCROP";
	strName = getlanguageAbbr(strLid);
	PrintConfig_ui.rB_Deskew->setText(strName);
	
	strLid ="STRING_PRINT_SIZE";
	strName = getlanguageAbbr(strLid);
	PrintConfig_ui.label_Size->setText(strName);
	
	strLid ="STRING_PRINT_IDCARDCOMBINE";
	strName = getlanguageAbbr(strLid);
	PrintConfig_ui.cB_IDCard->setText(strName);

	strLid ="STRING_PRINT_ORIGINAL";
	strName = getlanguageAbbr(strLid);
	PrintConfig_ui.cB_Original->setText(strName);


	strLid ="STRING_PRINT_NUM";
	strName = getlanguageAbbr(strLid);
	PrintConfig_ui.label_PrintNum->setText(strName);

	strLid ="STRING_PRINT_STANDARDIZATION";
	strName = getlanguageAbbr(strLid);
	PrintConfig_ui.groupBox_Standardization->setTitle(strName);

	strLid ="STRING_PRINT_STANDARDSIZE";
	strName = getlanguageAbbr(strLid);
	PrintConfig_ui.label_StandardSize->setText(strName);

	strLid ="STRING_PRINT_CHECKDPI";
	strName = getlanguageAbbr(strLid);
	PrintConfig_ui.pB_CheckDPI->setText(strName);

	strLid ="STRING_PRINT_RESTORE";
	strName = getlanguageAbbr(strLid);
	PrintConfig_ui.pB_Restore->setText(strName);
	
	strLid ="STRING_BAOCUN_LUJING";
	strName = getlanguageAbbr(strLid);
	PrintConfig_ui.label_SavePath->setText(strName);

	strLid ="STRING_PRINT_SELECTPATH";
	strName = getlanguageAbbr(strLid);
	PrintConfig_ui.pB_SelectSavePath->setText(strName);

	strLid ="STRING_PRINT_HELP";
	strName = getlanguageAbbr(strLid);
	PrintConfig_ui.pB_Help->setText(strName);

	QString fontpre = QString::fromLocal8Bit("<font color='blue' size='5' weight ='200'>");
	QString fontback = QString::fromLocal8Bit("</font>");

	//按钮提示
	strLid ="STRING_PRINT_CONFIG";
	strName = fontpre + getlanguageAbbr(strLid) + fontback;
	ui.pB_Memu->setToolTip(strName); 

	strLid ="STRING_PRINT_CUT";
	strName = fontpre + getlanguageAbbr(strLid) + fontback;
	ui.pB_Cut->setToolTip(strName); 

	strLid ="STRING_DAYING_YULAN";
	strName = fontpre + getlanguageAbbr(strLid) + fontback;
	ui.pB_switchView->setToolTip(strName); 

	strLid ="STRING_PRINT_SCAN";
	strName = fontpre + getlanguageAbbr(strLid) + fontback;
	ui.pB_ScanPrint->setToolTip(strName); 

	strLid ="STRING_DAYING";
	strName = fontpre + getlanguageAbbr(strLid) + fontback;
	ui.pB_Print->setToolTip(strName); 
	
	


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
QString EloamPrint::getlanguageAbbr(QString StrID)
{


	QByteArray ba = StrID.toLatin1();
	char *cstr = ba.data();
	char test[200];
	memset(test,0,200);
	QString str;
	//str =QString::fromLocal8Bit(getLanguageItem(cstr));
	//QMessageBox::warning(0,0,str);
	//return str;
	if(getLanguageItem(cstr))
	{
		strcpy(test, getLanguageItem(cstr));                             //根据具体的按钮id获取此Id的缩写名称，如果没有缩写则获取全称。
	}
	else
	{
		str = str.fromLocal8Bit("没有此ID的Item!");
		return str;
	}
	if(strlen(test)>0)
	{
		           
		/*转化编码  (LPWSTR)pUnicode*/
		 int  unicodeLen = ::MultiByteToWideChar( CP_UTF8,0, test,-1,  NULL, 0 );  
		  wchar_t *  pUnicode;  
		  pUnicode = new  wchar_t[unicodeLen+1];  
		  memset(pUnicode,0,(unicodeLen+1)*sizeof(wchar_t));
		 ::MultiByteToWideChar( CP_UTF8,0,test, -1,(LPWSTR)pUnicode ,unicodeLen );  
		 str =QString((QChar*)pUnicode);	
		 delete pUnicode;
		return str;
		
	}
	else
		return NULL;
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

QString getlanguageFullName(QString StrID)                      //获取全称
{
	QByteArray ba = StrID.toLatin1();
	char *cstrId =ba.data();
	QString Dstr;
	char getname[200];
	memset(getname,0,200);
	if(getLanguageFname(cstrId))	
	strcpy(getname,getLanguageFname(cstrId));
	else
	{
		Dstr =Dstr.fromLocal8Bit("没有此ID的Item！");
	}
	if(strlen(getname)>0)
	{
		           
		/*转化编码  (LPWSTR)pUnicode*/
		 int  unicodeLen = ::MultiByteToWideChar( CP_UTF8,0, getname,-1,  NULL, 0 );  
		  wchar_t *  pUnicode;  
		  pUnicode = new  wchar_t[unicodeLen+1];  
		  memset(pUnicode,0,(unicodeLen+1)*sizeof(wchar_t));
		 ::MultiByteToWideChar( CP_UTF8,0,getname, -1,(LPWSTR)pUnicode ,unicodeLen );  
		 Dstr =QString((QChar*)pUnicode);
		 delete pUnicode;
		return Dstr;
	}
	else
		return NULL;
}

void EloamPrint::OpenVideo()
{
			
		for(int i = 0;i< m_vMainDevice.size();i++)
		{	
			HELOAMDEVICE hDev = m_vMainDevice[i];
			if(1 == EloamDevice_GetEloamType(hDev))
			{	
						int subType = EloamDevice_GetSubtype(hDev);
						int type;
						//若为辅摄像头优先采用mjpg模式打开
						if( 0 != (subType & 2) )
						{
							type = 2;
						}
						else
						{
							type = 1;
						}
				m_hMainVideo = EloamDevice_CreateVideo(hDev, 0, type, ArivlaCallback, NULL, EloamTouchCallback, this);
				if (m_hMainVideo) 
				{
					//MessageBox(0,_T("cc"),0,0);
					//InitRatate();
					EloamView_SelectVideo(m_PrintView, m_hMainVideo, AttachCallback, this);  
					//EloamView_SetText(m_PrintView, L"打开视频中，请等待...", RGB(255, 255, 255));
					EloamView_SetCallback(m_PrintView, ZoomChange, this);					
					m_bIsLostDev =false;
					devFlag=100;
					break;
				}
			
			}
	     }
	
}

void  EloamPrint::ReadConfig()
{
	QString configPath = m_CurBinPath + QString::fromLocal8Bit("/QDilgCameraConfig.ini");
	QFile file(configPath);
	if(file.exists())
	{
		QString strDefImageSavePath=m_CurBinPath + QString::fromLocal8Bit("/image");
		std::string strDef = strDefImageSavePath.toStdString(); //将QString转换为string
		std::wstring wstrDef(strDef.length(), L' ');//初始化宽字节wstr
		std::copy(strDef.begin(), strDef.end(), wstrDef.begin());//将str复制到wstr
		LPCWSTR DefPath = wstrDef.c_str();//将wstr转换为C字符串的指针,然后赋值给path


		//图片扫描路径
		TCHAR buf[MAX_PATH] = {0};
 		GetPrivateProfileString(_T("Print param"),_T("m_CurPrintImageSavePath"),DefPath,buf,MAX_PATH,configPath.toStdWString().c_str());
		m_CurPrintImageSavePath =QString::fromWCharArray(buf);
		//保存命名方式
		m_iFileSaveType = GetPrivateProfileInt(_T("DigCamera driver param"), _T("m_iFileSaveType"), 0, configPath.toStdWString().c_str());
		//图片质量
		m_iJPEGQulitVal = GetPrivateProfileInt(_T("DigCamera driver param"), _T("m_iJPEGQulitVal"), 0, configPath.toStdWString().c_str());
		//当前言语索引
		m_bLanguageIndex = GetPrivateProfileInt(_T("DigCamera driver param"), _T("m_bLanguageIndex"), 0, configPath.toStdWString().c_str());
		//二值化阈值
		m_nThresholdValue = GetPrivateProfileInt(_T("Print param"), _T("m_nThresholdValue"), 0,configPath.toStdWString().c_str());
		//曝光值
		m_nExposureValue = GetPrivateProfileInt(_T("Print param"), _T("m_nExposureValue"), 0,configPath.toStdWString().c_str());
		m_nIsAutoExposure = GetPrivateProfileInt(_T("Print param"), _T("m_nIsAutoExposure"), 0,configPath.toStdWString().c_str());
		//X、YDPI
		m_CurXDpi = GetPrivateProfileInt(_T("Print param"), _T("CurXDpi"), 0,configPath.toStdWString().c_str());
		m_CurYDpi = GetPrivateProfileInt(_T("Print param"), _T("CurYDpi"), 0,configPath.toStdWString().c_str());
		//A4纸像素宽、高
		m_A4Width=GetPrivateProfileInt(_T("Print param"), _T("A4Width"), 0,configPath.toStdWString().c_str());
		m_A4Height=GetPrivateProfileInt(_T("Print param"), _T("A4Height"), 0,configPath.toStdWString().c_str());
		//A3纸像素宽、高
		m_A3Width=GetPrivateProfileInt(_T("Print param"), _T("A3Width"), 0,configPath.toStdWString().c_str());
		m_A3Height=GetPrivateProfileInt(_T("Print param"), _T("A3Height"), 0,configPath.toStdWString().c_str());
		//默认DPI
		m_A4DefDpi= GetPrivateProfileInt(_T("Print param"), _T("DefaultA4DPI"), 0, configPath.toStdWString().c_str());
		m_A3DefDpi= GetPrivateProfileInt(_T("Print param"), _T("DefaultA3DPI"), 0, configPath.toStdWString().c_str());
		//默认A4高度下  A3尺寸像素长宽 A4尺寸像素长宽
		m_A4HA4WidthDef= GetPrivateProfileInt(_T("Print param"), _T("m_A4HA4WidthDef"), 0, configPath.toStdWString().c_str());;
		m_A4HA4HeightDef= GetPrivateProfileInt(_T("Print param"), _T("m_A4HA4HeightDef"), 0, configPath.toStdWString().c_str());;
		m_A4HA3WidthDef= GetPrivateProfileInt(_T("Print param"), _T("m_A4HA3WidthDef"), 0, configPath.toStdWString().c_str());;
		m_A4HA3HeightDef= GetPrivateProfileInt(_T("Print param"), _T("m_A4HA3HeightDef"), 0, configPath.toStdWString().c_str());;
		//默认A3高度下  A3尺寸像素长宽 A4尺寸像素长宽
		m_A3HA4WidthDef= GetPrivateProfileInt(_T("Print param"), _T("m_A3HA4WidthDef"), 0, configPath.toStdWString().c_str());;
		m_A3HA4HeightDef= GetPrivateProfileInt(_T("Print param"), _T("m_A3HA4HeightDef"), 0, configPath.toStdWString().c_str());;
		m_A3HA3WidthDef= GetPrivateProfileInt(_T("Print param"), _T("m_A3HA3WidthDef"), 0, configPath.toStdWString().c_str());;
		m_A3HA3HeightDef= GetPrivateProfileInt(_T("Print param"), _T("m_A3HA3HeightDef"), 0, configPath.toStdWString().c_str());;
		//DPI是否重新标定过
		m_ischangeDPI = GetPrivateProfileInt(_T("Print param"), _T("m_ischangeDPI"), 0, configPath.toStdWString().c_str());
		//是否启用纠偏
		m_nIsDeskew	= GetPrivateProfileInt(_T("Print param"), _T("m_nIsDeskew"), 0, configPath.toStdWString().c_str());					//0表示不纠偏    1表示纠偏
		//打印尺寸
		m_nPrintSize = GetPrivateProfileInt(_T("Print param"), _T("m_nPrintSize"), 0, configPath.toStdWString().c_str());							//0表示A4        1表示A3
		//是否开启而正反合成
		m_nIsCombine = GetPrivateProfileInt(_T("Print param"), _T("m_nIsCombine"), 0, configPath.toStdWString().c_str());						//0表示单张拍照  1表示两张合成
		//是否打印原图
		m_nIsOrignal = GetPrivateProfileInt(_T("Print param"), _T("m_nIsOrignal"), 0, configPath.toStdWString().c_str());							//0表示二值化图  1表示原图
		//打印份数
		m_PrintNum = GetPrivateProfileInt(_T("Print param"), _T("m_PrintNum"), 0, configPath.toStdWString().c_str());
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
		//图片扫描路径 m_CurPrintImageSavePath
	para = m_CurPrintImageSavePath;	
	WritePrivateProfileString(_T("Print param"), _T("m_CurPrintImageSavePath"), para.toStdWString().c_str(), configPath.toStdWString().c_str());
	
	//设置机型默认dpi 1:1 打印使用
	para = QString("%10").arg(m_nThresholdValue);
	WritePrivateProfileString(_T("Print param"), _T("m_nThresholdValue"), para.toStdWString().c_str(), configPath.toStdWString().c_str());
	//曝光值
	para = QString("%10").arg(m_nExposureValue);
	WritePrivateProfileString(_T("Print param"), _T("m_nExposureValue"), para.toStdWString().c_str(), configPath.toStdWString().c_str());
	//是否自动曝光
	para = QString("%10").arg(m_nIsAutoExposure);
	WritePrivateProfileString(_T("Print param"), _T("m_nIsAutoExposure"), para.toStdWString().c_str(), configPath.toStdWString().c_str());
	//m_CurXDpi
	para = QString("%10").arg(m_CurXDpi);
	WritePrivateProfileString(_T("Print param"), _T("CurXDpi"), para.toStdWString().c_str(), configPath.toStdWString().c_str());
	//m_CurYDpi
	para = QString("%10").arg(m_CurYDpi);
	WritePrivateProfileString(_T("Print param"), _T("CurYDpi"), para.toStdWString().c_str(), configPath.toStdWString().c_str());
	//A4纸像素宽
	para = QString("%10").arg(m_A4Width);
	WritePrivateProfileString(_T("Print param"), _T("A4Width"), para.toStdWString().c_str(), configPath.toStdWString().c_str());
	
	//A4纸像素高
	para = QString("%10").arg(m_A4Height);
	WritePrivateProfileString(_T("Print param"), _T("A4Height"), para.toStdWString().c_str(), configPath.toStdWString().c_str());
	
	//A3纸像素宽
	para = QString("%10").arg(m_A3Width);
	WritePrivateProfileString(_T("Print param"), _T("A3Width"), para.toStdWString().c_str(), configPath.toStdWString().c_str());

	//A3纸像素高
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
	
	para = QString("%1").arg(m_nIsDeskew);
	WritePrivateProfileString(_T("Print param"), _T("m_nIsDeskew"), para.toStdWString().c_str(), configPath.toStdWString().c_str());
	para = QString("%1").arg(m_nPrintSize);
	WritePrivateProfileString(_T("Print param"), _T("m_nPrintSize"), para.toStdWString().c_str(), configPath.toStdWString().c_str());
	para = QString("%1").arg(m_nIsCombine);
	WritePrivateProfileString(_T("Print param"), _T("m_nIsCombine"), para.toStdWString().c_str(), configPath.toStdWString().c_str());
	para = QString("%1").arg(m_nIsOrignal);
	WritePrivateProfileString(_T("Print param"), _T("m_nIsOrignal"), para.toStdWString().c_str(), configPath.toStdWString().c_str());
	para = QString("%10").arg(m_PrintNum);
	WritePrivateProfileString(_T("Print param"), _T("m_PrintNum"), para.toStdWString().c_str(), configPath.toStdWString().c_str());


}

void EloamPrint::InitFiletype()
{
	switch(m_iFileSaveType)
	{
	case 0:
		m_sImageSaveType = QString::fromLocal8Bit(".bmp");
		break;
	case 1:
		m_sImageSaveType = QString::fromLocal8Bit(".jpg");
		break;
	case 2:
		m_sImageSaveType = QString::fromLocal8Bit(".png");
		break;
	case 3:
		m_sImageSaveType = QString::fromLocal8Bit(".tif");
		break;
	case 4:
		m_sImageSaveType = QString::fromLocal8Bit(".tga");
		break;
	case 5:
		m_sImageSaveType = QString::fromLocal8Bit(".pcx");
		break;
	case 6:
		m_sImageSaveType = QString::fromLocal8Bit(".ras");
		break;
	case 7:
		m_sImageSaveType = QString::fromLocal8Bit(".pdf");
		break;
	default:
		m_sImageSaveType = QString::fromLocal8Bit(".bmp");
	}
}
void EloamPrint::SetButtonStyle()
{
	ui.pB_Cut->setStyleSheet("QPushButton{border-image: url(:/EloamPrint/Resources/pic/pB_Cut.png);}"
		"QPushButton:hover{border-image: url(:/EloamPrint/Resources/pic/pB_Cut.png); background-color: rgb(255, 85, 127);}"
		"QPushButton:pressed{border-image: url(:/EloamPrint/Resources/pic/pB_Cut.png);  background-color: rgb(0, 170, 255);}");

	ui.pB_Memu->setStyleSheet("QPushButton{border-image: url(:/EloamPrint/Resources/pic/paramset.png);}"
		"QPushButton:hover{border-image: url(:/EloamPrint/Resources/pic/paramset.png); background-color: rgb(255, 85, 127);}"
		"QPushButton:pressed{border-image: url(:/EloamPrint/Resources/pic/paramset.png);  background-color: rgb(0, 170, 255);}");

	ui.pB_Print->setStyleSheet("QPushButton{border-image: url(:/EloamPrint/Resources/pic/print.png);}"
		"QPushButton:hover{border-image: url(:/EloamPrint/Resources/pic/print.png); background-color: rgb(255, 85, 127);}"
		"QPushButton:pressed{border-image: url(:/EloamPrint/Resources/pic/print.png);  background-color: rgb(0, 170, 255);}");

	ui.pB_ScanPrint->setStyleSheet("QPushButton{border-image: url(:/EloamPrint/Resources/pic/DnBar_scan.png);}"
		"QPushButton:hover{border-image: url(:/EloamPrint/Resources/pic/DnBar_scan.png); background-color: rgb(255, 85, 127);}"
		"QPushButton:pressed{border-image: url(:/EloamPrint/Resources/pic/DnBar_scan.png);  background-color: rgb(0, 170, 255);}");

	ui.pB_switchView->setStyleSheet("QPushButton{border-image: url(:/EloamPrint/Resources/pic/pB_Scrn.png);}"
		"QPushButton:hover{border-image: url(:/EloamPrint/Resources/pic/pB_Scrn.png); background-color: rgb(255, 85, 127);}"
		"QPushButton:pressed{border-image: url(:/EloamPrint/Resources/pic/pB_Scrn.png);  background-color: rgb(0, 170, 255);}");

}
void EloamPrint::InitDeskew()
{
	if(NULL != m_hMainVideo)
	{	if(m_nIsDeskew)
		{
			EloamVideo_EnableDeskew(m_hMainVideo,0);
		}
	}
}
void EloamPrint::InitExposure()
{
	if(NULL != m_hMainDev)
	{
	int max = EloamDevice_GetCameraControl(m_hMainDev,1,2);
	int min = EloamDevice_GetCameraControl(m_hMainDev,1,1);	
	int val = 0;
	if (max > 500||max < -500||(max == min))
	{
		//ui->
		PrintConfig_ui.label_ExposureValue->hide();
		PrintConfig_ui.Slider_Exposure->hide();
		PrintConfig_ui.spinBox_exposure->hide();
		return;
	}
	else
	{
		PrintConfig_ui.label_ExposureValue->show();
		PrintConfig_ui.Slider_Exposure->show();
		PrintConfig_ui.spinBox_exposure->show();
	}
	val = EloamDevice_GetCameraControl(m_hMainDev,1,5);
	if(m_nExposureValue > max ||m_nExposureValue < min)
	{
		m_nExposureValue = val;
	}
	PrintConfig_ui.Slider_Exposure->setRange(min,max);
	PrintConfig_ui.Slider_Exposure->setValue(val);
	PrintConfig_ui.spinBox_exposure->setRange(min,max);
	PrintConfig_ui.spinBox_exposure->setValue(val);
	if (m_nIsAutoExposure)
	{
		EloamDevice_SetCameraControl(m_hMainDev, 0x1, 0, TRUE);
		PrintConfig_ui.Slider_Exposure->setEnabled(FALSE);
		PrintConfig_ui.spinBox_exposure->setEnabled(FALSE);
		PrintConfig_ui.cB_AutoExposure->setChecked(TRUE);
	}
	else
	{		
		EloamDevice_SetCameraControl(m_hMainDev, 0x1, m_nExposureValue, false);
		EloamDevice_SetCameraControl(m_hMainDev,4,0,false);
		PrintConfig_ui.Slider_Exposure->setEnabled(TRUE);
		PrintConfig_ui.spinBox_exposure->setEnabled(TRUE);
		PrintConfig_ui.cB_AutoExposure->setChecked(FALSE);
		
	}
	}	
}