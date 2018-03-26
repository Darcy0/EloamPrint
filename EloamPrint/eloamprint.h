#ifndef ELOAMPRINT_H
#define ELOAMPRINT_H
#include <QtWidgets/QWidget>
#include "ui_eloamprint.h"
#include <tchar.h>
#include "ui_PrintConfigDlg.h"
#include "include\\eloamDll.h"
#pragma comment(lib,"lib/eloamDll.lib")
class EloamPrint : public QWidget
{
	Q_OBJECT

public:
	EloamPrint(QWidget *parent = 0);
	~EloamPrint();
public:
	//设备到达回调函数
	static VOID ELOAMAPI AttachCallback(HELOAMVIDEO video, LONG videoId, HELOAMVIEW view, LONG viewId, LPVOID userData);
	static VOID ELOAMAPI ArivlaCallback(HELOAMVIDEO video, LONG videoId, LPVOID userData);
	static VOID ELOAMAPI EloamTouchCallback(HELOAMVIDEO video, LPVOID userData);
	static VOID ELOAMAPI ZoomChange(HELOAMVIEW view, LONG flag, LONG value, LPVOID userData);
	static VOID ELOAMAPI DevChangeCallback(LONG type, LONG idx, LONG dbt, LPVOID userData) ;
	//读配置文件
	void ReadConfig();
	//写配置文件
	void WriteConfig();
	void InitDeskew();
	void InitPrintConfigDlg();
	void InitFiletype();
	void SetButtonStyle();
public:
	Ui::PrintConfigDlg PrintConfig_ui; //打印的设置窗口  CG 2017/7/24
	QWidget *m_PrintConfigDlg;
	int m_bLanguageIndex;
	int m_iFileSaveType;
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
	int m_nCheckSize;						   //检测卡纸的标准  0  A5   1  A4
	int m_nThresholdValue;						//二值化阈值
	int m_nExposureValue;						//曝光值
	int m_nExposureValueTem;					//曝光值变量
	int m_nIsDeskew;							//0表示不纠偏    1表示纠偏
	int m_nPrintSize;							//0表示A4        1表示A3
	int m_nIsCombine;							//0表示单张拍照  1表示两张合成
	int m_nIsOrignal;							//0表示二值化图  1表示原图
	int m_PrintNum;								//打印图片的份数 2017/5/2
	int m_nIsDeskewTem;							//0表示不纠偏    1表示纠偏
	int m_nPrintSizeTem;						//0表示A4        1表示A3
	int m_nIsCombineTem;						//0表示单张拍照  1表示两张合成
	int m_nIsOrignalTem;
	int m_PrintNumTem;
	long m_iJPEGQulitVal;
	float m_CurXDpi;
	float m_CurYDpi;
	int m_nIsAutoExposure;
	int m_nIsAutoExposureTem;					//是否自动曝光的变量
	bool m_bIsShowA;
	bool m_bIsView;                             //标志打印界面显示的是不是视频图像
	bool m_bIsCut;								//标志是否处于裁剪状
	bool m_bIsCombineIDCard;					//是否开启二代证正反合成
	QString m_CurBinPath;
	QString m_CurPrintImageSavePath;
	QString m_sShowResultImagePath;
	QString m_sCutImagePath;
	QString m_sImageSaveType;
	HWND m_hPrintView;
	HELOAMDEVICE m_hMainDev;					//主头当前设备
	HELOAMVIDEO m_hMainVideo;					//主视频
	std::vector<HELOAMDEVICE> m_vMainDevice;    //主设备列表
	HELOAMVIEW m_PrintView;						//打印窗口的画面  
	HELOAMIMAGE m_hPrintImage;					//打印窗口的图片句柄
	HELOAMRECT m_hCutRect;						//裁剪区域的句柄
	HELOAMIMAGE m_hCutImage;
	HELOAMIMAGE m_hShowResultImage;            //用于显示二值化效果的图片对象
	HELOAMIMAGE m_hShowResultImageA;
	HELOAMIMAGE m_hImageReverse;
	HELOAMIMAGE m_IDCardFirstImage;				//二代证合成的第一张图片
	HELOAMIMAGE m_hIDCardCombine;					//二代证合成图片句柄
	HELOAMTHUMBNAIL m_hThumbPrint;              //打印窗口的缩略图句柄
public:
	void OpenVideo();
	void OnCloseDevice();
		//设置语言
	void InitExposure();
	void SetLanguage();
	void GetSelectState();
	void SendMsgToParent(QString filePath);
	void ReversePrintImage(HELOAMIMAGE m_hPrintImage);
	//合成A4大小
	void A4CombineProcess(HELOAMIMAGE hImg,QString imgPath);
		//合成A4大小
	void A4CombineProcessB(HELOAMIMAGE hImg,QString imgPath);
	//身份证正反合成
	void A4IDCardCombineProcess(HELOAMIMAGE hImg,QString imgPath);
	//合成A3大小
	void A3CombineProcess(HELOAMIMAGE hImg,QString imgPath);

	/*通过传入一个ID号来获取每一个Item的缩写，如果没有缩写就直接返回全称*/
	QString getlanguageAbbr(QString StrID);

	/*通过传入一个ID号来获取每一个Item的缩写*/
	QString getlanguageFullName(QString StrID);
	
public slots:
		//响应打印窗口按照按钮
	void OnScanPrint();
	void OnScanPrint2();
	void OnScanPrintA3();
	//响应打印窗口校正DPI按钮
	void OnCheckDPI();
	//响应打印窗口上的打印按钮
	void OnPrintPic();
	//响应打印窗口上的微调框
	void OnSetPrintNum(int);
	//响应打印窗口上的关闭按钮
	void OnStopPrint();
	//A3幅面
	void OnCbA3();
		//A3幅面
	void OnCbA4();
	void OnCbAutoExposure();
	//设置检测卡标准为A4的尺寸
	void OnRbA4();
	//设置检测卡标准为A5的尺寸
	void OnRbA5();
	//设置自动纠偏
	void OnCbDeskew();
	//设置手动框选	
	void OnCbHandCrop(); 
	//切换画面
	void OnSwitchView();
	void OnSelectSavePath();
	//响应全选
	void OnCbSelectAll();
	void OnConfigDlg();
	void OnScanSizeA3();
	void OnScanSizeA4();
	void OnRestore();
	void OnCut();
	void OnSure();
	void OnCancel();
	void OnHelp();
	void setThresholdValue(int Value);
	void setExposureValue(int Value);
private:
	Ui::EloamPrint ui;
	void closeEvent(QCloseEvent *event);
};

#endif // ELOAMPRINT_H
