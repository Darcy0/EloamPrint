/*
 * Copyright (C) 2014 eloam technology
 * Auther: luoliangyi
 * CreateTime:2014.8.5
 * Email:luoliangyi@eloam.com.cn
 */


#ifndef __EDCOM_H__
#define __EDCOM_H__

#ifdef __cplusplus
#define EL_DEFAULT(val) = val
#else
#define EL_DEFAULT(val)
#endif

#define EDC_FLAG_CHECKBOX	0x01

#define EDC_MENU_NULL	0x01
#define EDC_MENU_BIND	0x02
#define EDC_MENU_FOLDER	0x03
#define EDC_MENU_FILE	0X04

#define EDC_ITEM_REFRESH		0x00000001	
#define EDC_ITEM_OPENFOLDERPOS	0x00000002
#define EDC_ITEM_DELETE			0x00000004
#define EDC_ITEM_PROPERTY		0x00000008
#define EDC_ITEM_EDIT			0x00000010
#define EDC_ITEM_BIND			0x00000020
#define EDC_ITEM_UNBIND			0x00000040
#define EDC_ITEM_OPENFILEPOS	0x00000080
#define EDC_ITEM_BLEND			0x00000100
#define EDC_ITEM_CREATEPDF		0x00000200
#define EDC_ITEM_OCR			0x00000400
#define EDC_ITEM_PRINT			0x00000800
#define EDC_ITEM_ADD			0x00001000
#define EDC_ITEM_SAVEAS			0x00002000

typedef void *HThumbnail;

//��Ӧ˫����callbackԭ�ͣ�strΪ·���ַ�����i==1ʱ��־�ļ���i==2ʱ��־�ļ��У�paramΪ��ǰ����ָ��
enum PORD{DIRECTORYPATH,FILEPATH};
typedef int (*DbFun)(const char *str,  PORD pord,  void* param);
//��Ӧ������callback������ͬ��
typedef int (*LclkFun)(const char *str, PORD pord, void* param);
//��Ӧˢ�µ�callback������ͬ��
typedef int (*UpdateFun)(void* param);
//��Ӧ�༭��callback������ͬ��
typedef int (*EditFun)(const char *str, void* param);
//OCR�ص�����
typedef int(*OcrFun)(const char* str, void* param);

//newһ��thumbnail��
HThumbnail WINAPI edcCreateThumbnail();

//delete��thumbnail��
void WINAPI edcReleaseThumbnail(HThumbnail  h);

//��ʼ����
int WINAPI edcInitLib(HThumbnail  h,  HWND hWnd, int flag);

//���ͼƬ��szFilePthͼƬ�ļ�·��
int WINAPI edcAddImages(HThumbnail h,  LPCSTR szFilePath);

//ָ��λ�ò��룬FilePathͼƬ·����Indexָ��λ��
int WINAPI edcInsert(HThumbnail h, LPCSTR FilePath, int Index);

//ָ��λ��ɾ����Index	ָ��λ�ã�bDelFile�Ƿ�ɾ��ԭ�ļ���bDelFileΪtrueʱɾ��
int WINAPI edcRemove(HThumbnail h, int Index, BOOL bDelFile EL_DEFAULT(FALSE));

//ָ��λ���滻��FilePathͼƬ�ļ�·����Indexָ��λ��
int WINAPI edcReplace(HThumbnail h, LPCSTR FilePath, int Index);

//Ĭ�����򣬰��ļ����ַ������ֵ��е�˳�����У�rhs, lhsΪ������ʼλ��
int WINAPI edcSortItem(HThumbnail h, int rhs EL_DEFAULT(0), int lhs EL_DEFAULT(0), BOOL bDel EL_DEFAULT(FALSE));

//��������ͼ��С
int WINAPI edcResizeImage(HThumbnail h, int width, int hight);

//������Ӧ˫��ͼƬ��callback��fΪ�ص�������paramΪ�ص���������
int WINAPI edcSetDbClick(HThumbnail h, DbFun f,  void* param);

//������Ӧcallback
int WINAPI edcSetLclk(HThumbnail h, LclkFun f,  void* param);

//ˢ����Ӧcallback
int WINAPI edcSetUpdate(HThumbnail h, UpdateFun f,  void* param);

//�༭��Ӧcallback
int WINAPI edcSetEdit(HThumbnail h, EditFun f,  void* param);

//Ocr��Ӧcallback
int WINAPI edcSetOcr(HThumbnail h, OcrFun f, void* param);

//�����Ƿ���ʾͼƬ��Ϣ�ַ�����bΪtrueʱ��ʾ
int WINAPI edcSetShowInfo(HThumbnail h, BOOL b);

//�õ���ʾ������ͼ�ϵ�ͼƬ����
int WINAPI edcGetTotalCount(HThumbnail h);

//�������õ��ļ�·��,�˴����볤��ΪMAX_PATH��char�ڴ棬ʹ����Ϸ���ֵ����edcDeleteTmpstr����
char* WINAPI edcGetFilePathByIdx(HThumbnail h, int idx);

//����edcGetFilePathByIdx�������ڴ�
void WINAPI edcDeleteTmpstr(char* str);

//ˢ������
int WINAPI edcArrange(HThumbnail h);

//�ı䴰�ڴ�С
int	WINAPI edcReSizes(HThumbnail h);

//ˢ����ʾ
int WINAPI edcUpdates(HThumbnail h);

//ֹͣ����ͼƬ�̣߳������ͼƬ����
int WINAPI edcStopAddimage(HThumbnail h);

//�����߳�
int WINAPI edcRestartThread(HThumbnail h);

//�õ�ѡ��������
int WINAPI edcGetSelectIndex(HThumbnail h);

//ɾ������,bDelΪtrueʱɾ��ԭ�ļ�������ɾ��
int WINAPI edcRemoveAll(HThumbnail h, BOOL bDel);

//��������pdfģʽ��bmodeΪTRUEʱ��ÿ��ͼƬ����һ��pdf�ļ���������ͼƬ����һ��pdf�ļ�
int WINAPI edcSetPdfMode(HThumbnail h, BOOL bmode);

//���úϳ�ͼƬ��ʽ��bmodeΪTRUEʱ��Ϊ���ºϳɣ�����Ϊ���Һϳ�
int WINAPI edcSetComposeMode(HThumbnail h, BOOL bmode);

//�ж���һ���Ƿ�ѡ��
int WINAPI edcIsSelected(HThumbnail h, int index);

//�õ�ѡ�����б�
char* WINAPI edcGetSelectedItem(HThumbnail h);

// �������ԡ���Ϊϵͳ���ԣ���Ϊ�������ģ���Ϊ�������ģ���ΪӢ��
int WINAPI edcSetLanguage(HThumbnail h, int langId);

// ���ò˵���
int WINAPI edcSetMenuItem(HThumbnail h, int menuId, int flag);

// �Ƿ�ѡ��
int WINAPI edcGetCheck(HThumbnail h, int index);

// ����ѡ��
int WINAPI edcSetCheck(HThumbnail h, int index, BOOL bCheck);

#endif