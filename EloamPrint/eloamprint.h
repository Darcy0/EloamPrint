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
	QString GetImageSaveFileName(QString flag="");
	//��ʼ���Զ�����
	void InitDeskew();
	//��ʼ���Զ��ع�
	void InitExposure();
	//���ɴ�ӡͼƬ
	HELOAMIMAGE GeneratePrintImage(HELOAMIMAGE hInputImg,PrintSize printSize);
	//ͼƬ�ϳ� hImg1=�ϳɵĵ�һ��ͼƬ;hImg2=�ϳɵĵڶ���ͼƬ;space=�ϳ�ͼƬʱ�ļ��;isVertical=�Ƿ�ֱ����
	HELOAMIMAGE ImageCombine(HELOAMIMAGE hImg1,HELOAMIMAGE hImg2,int space=0,bool isVertical=true);
	//����Ƶ
	void OpenVideo();
	//Ϊ�˽�����������ɱ����ͼƬ��windows�Դ��Ĵ�ӡʵ��Ч����ƫС��������һ��1.038�ľ���ֵ��
	//�ú���Ϊ�������ó����Դ���ӡʱ������ֵ��Ӱ�졣
	HELOAMIMAGE ReversePrintImage(HELOAMIMAGE printImage);
public slots:
	//��Ӧ��ӡ�����ϵĹرհ�ť
	void OnStopPrint();	
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
	void OnThresholdValueChanged(int Value);
	//��Ӧ���ò����ı�
	void onNewPrintConfig(QVariant& newParm);
	//��Ӧ�ָ�Ĭ�������ź�
	void onStandardizationRestore();
	//��Ӧ�궨�ź�
	void onStandardizationCheckDPI(CalibrationCard size,bool isDeskew);
protected:
	//���ڹر��¼�
	void closeEvent(QCloseEvent *event);

private:
	Ui::EloamPrint ui;
	Translate translate;						//���ʻ�������
	PrintConfig *m_printConfigUi;				//���ô���
	long m_iJPEGQulitVal;						//ͼ������
	PrintConfigParam m_printConfigParam;		//���ò���
	int m_bLanguageIndex;						//��������
	int m_iImgSaveType;							//ͼƬ������������	
	bool m_bIsVideoView;                        //��־��ӡ������ʾ���ǲ�����Ƶͼ��
	bool m_bIsCut;								//��־�Ƿ��ڲü�״
	int m_nThresholdValue;						//��ֵ����ֵ

	               	
	
	
	int m_ischangeDPI;							//�Ƿ�궨��
	
	float m_fCurXDpi;							//Ĭ��X����Dpi
	float m_fCurYDpi;							//Ĭ��Y����Dpi
	int m_A4Width;                              //A4ֽ�ŵ����ؿ�
	int m_A4Height;								//A4ֽ�ŵ����ظ�
	int m_A3Width;                              //A3ֽ�ŵ����ؿ�
	int m_A3Height;								//A3ֽ�ŵ����ظ�

	int m_A4DefDpi;								//Ĭ�ϵ�A4DPI��������
	int m_A3DefDpi;								//Ĭ�ϵ�A3DPI��������
	int m_A4HA4WidthDef;						//Ĭ��A4�߶ȣ�A4ֽ���
	int m_A4HA4HeightDef;						//Ĭ��A4�߶ȣ�A4ֽ�߶�
	int m_A4HA3WidthDef;						//Ĭ��A4�߶ȣ�A3ֽ���
	int m_A4HA3HeightDef;						//Ĭ��A4�߶ȣ�A3ֽ�߶�
	int m_A3HA4WidthDef;						//Ĭ��A3�߶ȣ�A4ֽ���
	int m_A3HA4HeightDef;						//Ĭ��A3�߶ȣ�A4ֽ�߶�
	int m_A3HA3WidthDef;						//Ĭ��A3�߶ȣ�A3ֽ���
	int m_A3HA3HeightDef;						//Ĭ��A3�߶ȣ�A3ֽ�߶�
	
		
	QString m_sShowResultImagePath;//����·��
	QString m_sCutImagePath;
	
	HELOAMDEVICE m_hMainDev;					//��ͷ��ǰ�豸
	std::vector<HELOAMDEVICE> m_vDevices;		//���豸�б�
	HELOAMVIDEO m_hMainVideo;					//����Ƶ
	HELOAMVIEW m_hMainView;						//����Ƶ���ڵĻ���  
	HELOAMTHUMBNAIL m_hThumbPrint;              //��ӡ���ڵ�����ͼ���	
	
	HELOAMIMAGE m_hPrintImage;					//��ӡ���ڵ�ͼƬ���
	HELOAMRECT m_hCutRect;						//�ü�����ľ��
	HELOAMIMAGE m_hCutImage;
	HELOAMIMAGE m_hShowResultImage;				//������ʾ��ֵ��Ч����ͼƬ����
};
#endif // ELOAMPRINT_H