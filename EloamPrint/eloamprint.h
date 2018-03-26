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
	//�豸����ص�����
	static VOID ELOAMAPI AttachCallback(HELOAMVIDEO video, LONG videoId, HELOAMVIEW view, LONG viewId, LPVOID userData);
	static VOID ELOAMAPI ArivlaCallback(HELOAMVIDEO video, LONG videoId, LPVOID userData);
	static VOID ELOAMAPI EloamTouchCallback(HELOAMVIDEO video, LPVOID userData);
	static VOID ELOAMAPI ZoomChange(HELOAMVIEW view, LONG flag, LONG value, LPVOID userData);
	static VOID ELOAMAPI DevChangeCallback(LONG type, LONG idx, LONG dbt, LPVOID userData) ;
	//�������ļ�
	void ReadConfig();
	//д�����ļ�
	void WriteConfig();
	void InitDeskew();
	void InitPrintConfigDlg();
	void InitFiletype();
	void SetButtonStyle();
public:
	Ui::PrintConfigDlg PrintConfig_ui; //��ӡ�����ô���  CG 2017/7/24
	QWidget *m_PrintConfigDlg;
	int m_bLanguageIndex;
	int m_iFileSaveType;
	int devFlag;
	bool m_bIsLostDev;                         //��־�豸�ε���
	int m_A4DefDpi;								//Ĭ�ϵ�A4DPI��������
	int m_A3DefDpi;								//Ĭ�ϵ�A3DPI��������
	int m_ischangeDPI;							//�Ƿ�궨��
	int m_A4HA4WidthDef;
	int m_A4HA4HeightDef;
	int m_A4HA3WidthDef;
	int m_A4HA3HeightDef;
	int m_A3HA4WidthDef;
	int m_A3HA4HeightDef;
	int m_A3HA3WidthDef;
	int m_A3HA3HeightDef;
	int m_A4Width;                              //A4ֽ�ŵ����ؿ�
	int m_A4Height;								//A4ֽ�ŵ����ظ�
	int m_A3Width;                              //A3ֽ�ŵ����ؿ�
	int m_A3Height;								//A3ֽ�ŵ����ظ�
	int m_nCheckSize;						   //��⿨ֽ�ı�׼  0  A5   1  A4
	int m_nThresholdValue;						//��ֵ����ֵ
	int m_nExposureValue;						//�ع�ֵ
	int m_nExposureValueTem;					//�ع�ֵ����
	int m_nIsDeskew;							//0��ʾ����ƫ    1��ʾ��ƫ
	int m_nPrintSize;							//0��ʾA4        1��ʾA3
	int m_nIsCombine;							//0��ʾ��������  1��ʾ���źϳ�
	int m_nIsOrignal;							//0��ʾ��ֵ��ͼ  1��ʾԭͼ
	int m_PrintNum;								//��ӡͼƬ�ķ��� 2017/5/2
	int m_nIsDeskewTem;							//0��ʾ����ƫ    1��ʾ��ƫ
	int m_nPrintSizeTem;						//0��ʾA4        1��ʾA3
	int m_nIsCombineTem;						//0��ʾ��������  1��ʾ���źϳ�
	int m_nIsOrignalTem;
	int m_PrintNumTem;
	long m_iJPEGQulitVal;
	float m_CurXDpi;
	float m_CurYDpi;
	int m_nIsAutoExposure;
	int m_nIsAutoExposureTem;					//�Ƿ��Զ��ع�ı���
	bool m_bIsShowA;
	bool m_bIsView;                             //��־��ӡ������ʾ���ǲ�����Ƶͼ��
	bool m_bIsCut;								//��־�Ƿ��ڲü�״
	bool m_bIsCombineIDCard;					//�Ƿ�������֤�����ϳ�
	QString m_CurBinPath;
	QString m_CurPrintImageSavePath;
	QString m_sShowResultImagePath;
	QString m_sCutImagePath;
	QString m_sImageSaveType;
	HWND m_hPrintView;
	HELOAMDEVICE m_hMainDev;					//��ͷ��ǰ�豸
	HELOAMVIDEO m_hMainVideo;					//����Ƶ
	std::vector<HELOAMDEVICE> m_vMainDevice;    //���豸�б�
	HELOAMVIEW m_PrintView;						//��ӡ���ڵĻ���  
	HELOAMIMAGE m_hPrintImage;					//��ӡ���ڵ�ͼƬ���
	HELOAMRECT m_hCutRect;						//�ü�����ľ��
	HELOAMIMAGE m_hCutImage;
	HELOAMIMAGE m_hShowResultImage;            //������ʾ��ֵ��Ч����ͼƬ����
	HELOAMIMAGE m_hShowResultImageA;
	HELOAMIMAGE m_hImageReverse;
	HELOAMIMAGE m_IDCardFirstImage;				//����֤�ϳɵĵ�һ��ͼƬ
	HELOAMIMAGE m_hIDCardCombine;					//����֤�ϳ�ͼƬ���
	HELOAMTHUMBNAIL m_hThumbPrint;              //��ӡ���ڵ�����ͼ���
public:
	void OpenVideo();
	void OnCloseDevice();
		//��������
	void InitExposure();
	void SetLanguage();
	void GetSelectState();
	void SendMsgToParent(QString filePath);
	void ReversePrintImage(HELOAMIMAGE m_hPrintImage);
	//�ϳ�A4��С
	void A4CombineProcess(HELOAMIMAGE hImg,QString imgPath);
		//�ϳ�A4��С
	void A4CombineProcessB(HELOAMIMAGE hImg,QString imgPath);
	//���֤�����ϳ�
	void A4IDCardCombineProcess(HELOAMIMAGE hImg,QString imgPath);
	//�ϳ�A3��С
	void A3CombineProcess(HELOAMIMAGE hImg,QString imgPath);

	/*ͨ������һ��ID������ȡÿһ��Item����д�����û����д��ֱ�ӷ���ȫ��*/
	QString getlanguageAbbr(QString StrID);

	/*ͨ������һ��ID������ȡÿһ��Item����д*/
	QString getlanguageFullName(QString StrID);
	
public slots:
		//��Ӧ��ӡ���ڰ��հ�ť
	void OnScanPrint();
	void OnScanPrint2();
	void OnScanPrintA3();
	//��Ӧ��ӡ����У��DPI��ť
	void OnCheckDPI();
	//��Ӧ��ӡ�����ϵĴ�ӡ��ť
	void OnPrintPic();
	//��Ӧ��ӡ�����ϵ�΢����
	void OnSetPrintNum(int);
	//��Ӧ��ӡ�����ϵĹرհ�ť
	void OnStopPrint();
	//A3����
	void OnCbA3();
		//A3����
	void OnCbA4();
	void OnCbAutoExposure();
	//���ü�⿨��׼ΪA4�ĳߴ�
	void OnRbA4();
	//���ü�⿨��׼ΪA5�ĳߴ�
	void OnRbA5();
	//�����Զ���ƫ
	void OnCbDeskew();
	//�����ֶ���ѡ	
	void OnCbHandCrop(); 
	//�л�����
	void OnSwitchView();
	void OnSelectSavePath();
	//��Ӧȫѡ
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
