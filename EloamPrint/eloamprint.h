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
	//�豸����ص�����
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
	//���ʻ�����
	void SetLanguage();
	//���ð�ť��ʽ
	void SetButtonStyle();
	//�������ļ�
	void ReadConfig();
	//д�����ļ�
	void WriteConfig();
	//��ȡ����ͼ���ļ���׺
	QString GetImageSaveSuffix(int imgType);
	//��ȡ����ͼ���ļ���
	QString GetImageSaveFileName();
	//��ʼ���Զ�����
	void InitDeskew();
	//��ʼ���Զ��ع�
	void InitExposure();
	//���ɴ�ӡͼƬ
	HELOAMIMAGE GeneratePrintImage(HELOAMIMAGE hInputImg,PrintSize printSize);
	//����Ƶ
	void OpenVideo();
	//�ͷ��豸
	void OnCloseDevice();
	

	
	void ReversePrintImage(HELOAMIMAGE m_hPrintImage);
	//�ϳ�A4��С
	void A4CombineProcess(HELOAMIMAGE hImg,QString imgPath);
	//�ϳ�A4��С
	void A4CombineProcessB(HELOAMIMAGE hImg,QString imgPath);
	//���֤�����ϳ�
	void A4IDCardCombineProcess(HELOAMIMAGE hImg,QString imgPath);
	//�ϳ�A3��С
	void A3CombineProcess(HELOAMIMAGE hImg,QString imgPath);

public slots:
	void OnScanPrint2();
	//��Ӧ��ӡ�����ϵĹرհ�ť
	void OnStopPrint();

	void onNewPrintConfig(QVariant& newParm);
	void onStandardizationRestore();
	void onStandardizationCheckDPI(CalibrationCard size);
	void OnScanSizeA3();
	void OnScanSizeA4();

	//�����ô���
	void OnConfigDlg();
	//�����ü�
	void OnCut();
	//����
	void OnCapture();
	//��ӡԤ������ƵԤ���л�
	void OnSwitchView();
	//��ӡԤ��
	void OnPrintPic();
	//ͼƬѡȫ
	void OnCbSelectAll();
	//���ö�ֵ����ֵ
	void setThresholdValue(int Value);

protected:
	void closeEvent(QCloseEvent *event);

private:
	Ui::EloamPrint ui;
	Translate translate;						//���ʻ�������
	PrintConfig *m_printConfigUi;				//���ô���
	PrintConfigParam m_printConfigParam;		//���ò���
	int m_bLanguageIndex;						//��������
	int m_iImgSaveType;							//ͼƬ������������
	
	
	bool m_bIsVideoView;                        //��־��ӡ������ʾ���ǲ�����Ƶͼ��
	bool m_bIsCut;								//��־�Ƿ��ڲü�״

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
	
	
	
	int m_nThresholdValue;						//��ֵ����ֵ
	
	
	long m_iJPEGQulitVal;
	float m_fCurXDpi;
	float m_fCurYDpi;
	
	bool m_bIsShowA;


	QString m_CurBinPath;
	
	QString m_sShowResultImagePath;//����·��
	QString m_sCutImagePath;
	
	HWND m_hPrintView;
	HELOAMDEVICE m_hMainDev;					//��ͷ��ǰ�豸
	HELOAMVIDEO m_hMainVideo;					//����Ƶ
	std::vector<HELOAMDEVICE> m_vMainDevice;    //���豸�б�
	HELOAMVIEW m_PrintView;						//��ӡ���ڵĻ���  
	HELOAMIMAGE m_hPrintImage;					//��ӡ���ڵ�ͼƬ���
	HELOAMRECT m_hCutRect;						//�ü�����ľ��
	HELOAMIMAGE m_hCutImage;
	HELOAMIMAGE m_hShowResultImage;				//������ʾ��ֵ��Ч����ͼƬ����
	HELOAMIMAGE m_hShowResultImageA;
	HELOAMIMAGE m_hImageReverse;
	HELOAMIMAGE m_IDCardFirstImage;				//����֤�ϳɵĵ�һ��ͼƬ
	HELOAMIMAGE m_hIDCardCombine;				//����֤�ϳ�ͼƬ���
	HELOAMTHUMBNAIL m_hThumbPrint;              //��ӡ���ڵ�����ͼ���
};
#endif // ELOAMPRINT_H