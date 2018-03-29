#ifndef ELOAMPRINT_H
#define ELOAMPRINT_H
#include <QtWidgets/QWidget>
#include <tchar.h>
#include "ui_EloamPrint.h"
#include "PrintConfig.h"
#include "include\\eloamDll.h"
#include "translate.h"

#pragma comment(lib,"lib/eloamDll.lib")

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
	QString GetImageSaveFileName();
	//初始化自动纠裁
	void InitDeskew();
	//初始化自动曝光
	void InitExposure();
	//生成打印图片
	HELOAMIMAGE GeneratePrintImage(HELOAMIMAGE hInputImg,PrintSize printSize);
	//打开视频
	void OpenVideo();
	//释放设备
	void OnCloseDevice();
	

	
	void ReversePrintImage(HELOAMIMAGE m_hPrintImage);
	//合成A4大小
	void A4CombineProcess(HELOAMIMAGE hImg,QString imgPath);
	//合成A4大小
	void A4CombineProcessB(HELOAMIMAGE hImg,QString imgPath);
	//身份证正反合成
	void A4IDCardCombineProcess(HELOAMIMAGE hImg,QString imgPath);
	//合成A3大小
	void A3CombineProcess(HELOAMIMAGE hImg,QString imgPath);

public slots:
	void OnScanPrint2();
	//响应打印窗口上的关闭按钮
	void OnStopPrint();

	void onNewPrintConfig(QVariant& newParm);
	void onStandardizationRestore();
	void onStandardizationCheckDPI(CalibrationCard size);
	void OnScanSizeA3();
	void OnScanSizeA4();

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
	void setThresholdValue(int Value);

protected:
	void closeEvent(QCloseEvent *event);

private:
	Ui::EloamPrint ui;
	Translate translate;						//国际化翻译类
	PrintConfig *m_printConfigUi;				//设置窗口
	PrintConfigParam m_printConfigParam;		//设置参数
	int m_bLanguageIndex;						//语言索引
	int m_iImgSaveType;							//图片保存类型索引
	
	
	bool m_bIsVideoView;                        //标志打印界面显示的是不是视频图像
	bool m_bIsCut;								//标志是否处于裁剪状

	int devFlag;
	bool m_bIsLostDev;                         //标志设备拔掉了
	
	
	int m_A4DefDpi;								//默认的A4DPI出厂设置
	int m_A3DefDpi;								//默认的A3DPI出厂设置
	
	int m_ischangeDPI;							//是否标定过
	
	int m_A4HA4WidthDef;
	int m_A4HA4HeightDef;
	int m_A4HA3WidthDef;
	int m_A4HA3HeightDef;
	int m_A3HA4WidthDef;
	int m_A3HA4HeightDef;
	int m_A3HA3WidthDef;
	int m_A3HA3HeightDef;
	
	int m_A4Width;                              //A4纸张的像素宽
	int m_A4Height;								//A4纸张的像素高
	int m_A3Width;                              //A3纸张的像素宽
	int m_A3Height;								//A3纸张的像素高
	
	
	
	int m_nThresholdValue;						//二值化阈值
	
	
	long m_iJPEGQulitVal;
	float m_fCurXDpi;
	float m_fCurYDpi;
	
	bool m_bIsShowA;


	QString m_CurBinPath;
	
	QString m_sShowResultImagePath;//保存路径
	QString m_sCutImagePath;
	
	HWND m_hPrintView;
	HELOAMDEVICE m_hMainDev;					//主头当前设备
	HELOAMVIDEO m_hMainVideo;					//主视频
	std::vector<HELOAMDEVICE> m_vMainDevice;    //主设备列表
	HELOAMVIEW m_PrintView;						//打印窗口的画面  
	HELOAMIMAGE m_hPrintImage;					//打印窗口的图片句柄
	HELOAMRECT m_hCutRect;						//裁剪区域的句柄
	HELOAMIMAGE m_hCutImage;
	HELOAMIMAGE m_hShowResultImage;				//用于显示二值化效果的图片对象
	HELOAMIMAGE m_hShowResultImageA;
	HELOAMIMAGE m_hImageReverse;
	HELOAMIMAGE m_IDCardFirstImage;				//二代证合成的第一张图片
	HELOAMIMAGE m_hIDCardCombine;				//二代证合成图片句柄
	HELOAMTHUMBNAIL m_hThumbPrint;              //打印窗口的缩略图句柄
};
#endif // ELOAMPRINT_H