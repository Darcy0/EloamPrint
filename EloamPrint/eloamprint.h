#ifndef ELOAMPRINT_H
#define ELOAMPRINT_H
#include <QtWidgets/QWidget>
#include <tchar.h>
#include "ui_EloamPrint.h"
#include "PrintConfig.h"
#include "include\\eloamDll.h"
#include "translate.h"

#pragma comment(lib,"lib/eloamDll.lib")

const float A4Width=297.0;//mm
const float A4Height=210.0;//mm
const float A5Width=210.0;//mm
const float A5Height=148.5;//mm
const float Inch=25.4;//mm

class EloamPrint : public QWidget
{
	Q_OBJECT
public:
	//设备到达回调函数
	static VOID ELOAMAPI AttachCallback(HELOAMVIDEO video, LONG videoId, HELOAMVIEW view, LONG viewId, LPVOID userData);
	static VOID ELOAMAPI ArivlaCallback(HELOAMVIDEO video, LONG videoId, LPVOID userData);
	static VOID ELOAMAPI EloamTouchCallback(HELOAMVIDEO video, LPVOID userData);
	static VOID ELOAMAPI ZoomChange(HELOAMVIEW view, LONG flag, LONG value, LPVOID userData);
	static VOID ELOAMAPI DevChangeCallback(LONG type, LONG idx, LONG dbt, LPVOID userData) ;

public:
	EloamPrint(QWidget *parent = 0);
	~EloamPrint();

private:
	void SendMsgToParent(QString filePath);
	//国际化语言
	void SetLanguage();
	//设置按钮样式
	void SetButtonStyle();
	//读配置文件
	void ReadConfig();
	//写配置文件
	void WriteConfig();
	//获取保存图像文件后缀
	QString GetImageSaveSuffix(int imgType);
	//获取保存图像文件名
	QString GetImageSaveFileName(QString flag="");
	//初始化自动纠裁
	void InitDeskew();
	//初始化自动曝光
	void InitExposure();
	//生成打印图片
	HELOAMIMAGE GeneratePrintImage(HELOAMIMAGE hInputImg,PrintSize printSize);
	//图片合成 hImg1=合成的第一张图片;hImg2=合成的第二张图片;space=合成图片时的间距;isVertical=是否垂直排列
	HELOAMIMAGE ImageCombine(HELOAMIMAGE hImg1,HELOAMIMAGE hImg2,int space=0,bool isVertical=true);
	//打开视频
	void OpenVideo();
	//为了解决本程序生成保存的图片用windows自带的打印实际效果会偏小，所以有一个1.038的经验值。
	//该函数为了消除用程序自带打印时，经验值的影响。
	HELOAMIMAGE ReversePrintImage(HELOAMIMAGE printImage);
public slots:
	//响应打印窗口上的关闭按钮
	void OnStopPrint();	
	//打开设置窗口
	void OnConfigDlg();
	//启动裁剪
	void OnCut();
	//拍照
	void OnCapture();
	//打印预览、视频预览切换
	void OnSwitchView();
	//打印预览
	void OnPrintPic();
	//图片选全
	void OnCbSelectAll();
	//设置二值化阈值
	void OnThresholdValueChanged(int Value);
	//响应配置参数改变
	void onNewPrintConfig(QVariant& newParm);
	//响应恢复默认配置信号
	void onStandardizationRestore();
	//响应标定信号
	void onStandardizationCheckDPI(CalibrationCard size,bool isDeskew);
protected:
	//窗口关闭事件
	void closeEvent(QCloseEvent *event);

private:
	Ui::EloamPrint ui;
	Translate translate;						//国际化翻译类
	PrintConfig *m_printConfigUi;				//设置窗口
	long m_iJPEGQulitVal;						//图像质量
	PrintConfigParam m_printConfigParam;		//设置参数
	int m_bLanguageIndex;						//语言索引
	int m_iImgSaveType;							//图片保存类型索引	
	bool m_bIsVideoView;                        //标志打印界面显示的是不是视频图像
	bool m_bIsCut;								//标志是否处于裁剪状
	int m_nThresholdValue;						//二值化阈值

	               	
	
	
	int m_ischangeDPI;							//是否标定过
	
	float m_fCurXDpi;							//默认X方向Dpi
	float m_fCurYDpi;							//默认Y方向Dpi
	int m_A4Width;                              //A4纸张的像素宽
	int m_A4Height;								//A4纸张的像素高
	int m_A3Width;                              //A3纸张的像素宽
	int m_A3Height;								//A3纸张的像素高

	int m_A4DefDpi;								//默认的A4DPI出厂设置
	int m_A3DefDpi;								//默认的A3DPI出厂设置
	int m_A4HA4WidthDef;						//默认A4高度，A4纸宽度
	int m_A4HA4HeightDef;						//默认A4高度，A4纸高度
	int m_A4HA3WidthDef;						//默认A4高度，A3纸宽度
	int m_A4HA3HeightDef;						//默认A4高度，A3纸高度
	int m_A3HA4WidthDef;						//默认A3高度，A4纸宽度
	int m_A3HA4HeightDef;						//默认A3高度，A4纸高度
	int m_A3HA3WidthDef;						//默认A3高度，A3纸宽度
	int m_A3HA3HeightDef;						//默认A3高度，A3纸高度
	
		
	QString m_sShowResultImagePath;//保存路径
	QString m_sCutImagePath;
	
	HELOAMDEVICE m_hMainDev;					//主头当前设备
	std::vector<HELOAMDEVICE> m_vDevices;		//主设备列表
	HELOAMVIDEO m_hMainVideo;					//主视频
	HELOAMVIEW m_hMainView;						//主视频窗口的画面  
	HELOAMTHUMBNAIL m_hThumbPrint;              //打印窗口的缩略图句柄	
	
	HELOAMIMAGE m_hPrintImage;					//打印窗口的图片句柄
	HELOAMRECT m_hCutRect;						//裁剪区域的句柄
	HELOAMIMAGE m_hCutImage;
	HELOAMIMAGE m_hShowResultImage;				//用于显示二值化效果的图片对象
};
#endif // ELOAMPRINT_H