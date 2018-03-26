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

//响应双击的callback原型，str为路径字符串，i==1时标志文件，i==2时标志文件夹，param为当前窗口指针
enum PORD{DIRECTORYPATH,FILEPATH};
typedef int (*DbFun)(const char *str,  PORD pord,  void* param);
//响应单击的callback，参数同上
typedef int (*LclkFun)(const char *str, PORD pord, void* param);
//响应刷新的callback，参数同上
typedef int (*UpdateFun)(void* param);
//响应编辑的callback，参数同上
typedef int (*EditFun)(const char *str, void* param);
//OCR回调函数
typedef int(*OcrFun)(const char* str, void* param);

//new一个thumbnail类
HThumbnail WINAPI edcCreateThumbnail();

//delete掉thumbnail类
void WINAPI edcReleaseThumbnail(HThumbnail  h);

//初始化库
int WINAPI edcInitLib(HThumbnail  h,  HWND hWnd, int flag);

//添加图片，szFilePth图片文件路径
int WINAPI edcAddImages(HThumbnail h,  LPCSTR szFilePath);

//指定位置插入，FilePath图片路径，Index指定位置
int WINAPI edcInsert(HThumbnail h, LPCSTR FilePath, int Index);

//指定位置删除，Index	指定位置，bDelFile是否删除原文件，bDelFile为true时删除
int WINAPI edcRemove(HThumbnail h, int Index, BOOL bDelFile EL_DEFAULT(FALSE));

//指定位置替换，FilePath图片文件路径，Index指定位置
int WINAPI edcReplace(HThumbnail h, LPCSTR FilePath, int Index);

//默认排序，按文件名字符串在字典中的顺序排列，rhs, lhs为排序起始位置
int WINAPI edcSortItem(HThumbnail h, int rhs EL_DEFAULT(0), int lhs EL_DEFAULT(0), BOOL bDel EL_DEFAULT(FALSE));

//重置缩略图大小
int WINAPI edcResizeImage(HThumbnail h, int width, int hight);

//设置响应双击图片的callback，f为回调函数，param为回调函数参数
int WINAPI edcSetDbClick(HThumbnail h, DbFun f,  void* param);

//单击响应callback
int WINAPI edcSetLclk(HThumbnail h, LclkFun f,  void* param);

//刷新响应callback
int WINAPI edcSetUpdate(HThumbnail h, UpdateFun f,  void* param);

//编辑响应callback
int WINAPI edcSetEdit(HThumbnail h, EditFun f,  void* param);

//Ocr响应callback
int WINAPI edcSetOcr(HThumbnail h, OcrFun f, void* param);

//设置是否显示图片信息字符串，b为true时显示
int WINAPI edcSetShowInfo(HThumbnail h, BOOL b);

//得到显示在缩略图上的图片总数
int WINAPI edcGetTotalCount(HThumbnail h);

//由索引得到文件路径,此处申请长度为MAX_PATH的char内存，使用完毕返回值传给edcDeleteTmpstr销毁
char* WINAPI edcGetFilePathByIdx(HThumbnail h, int idx);

//销毁edcGetFilePathByIdx中申请内存
void WINAPI edcDeleteTmpstr(char* str);

//刷新排列
int WINAPI edcArrange(HThumbnail h);

//改变窗口大小
int	WINAPI edcReSizes(HThumbnail h);

//刷新显示
int WINAPI edcUpdates(HThumbnail h);

//停止加载图片线程，并清空图片队列
int WINAPI edcStopAddimage(HThumbnail h);

//重启线程
int WINAPI edcRestartThread(HThumbnail h);

//得到选择项索引
int WINAPI edcGetSelectIndex(HThumbnail h);

//删除所有,bDel为true时删除原文件，否则不删除
int WINAPI edcRemoveAll(HThumbnail h, BOOL bDel);

//设置生成pdf模式，bmode为TRUE时，每张图片生成一个pdf文件，否则多个图片生成一个pdf文件
int WINAPI edcSetPdfMode(HThumbnail h, BOOL bmode);

//设置合成图片方式，bmode为TRUE时，为上下合成，否则为左右合成
int WINAPI edcSetComposeMode(HThumbnail h, BOOL bmode);

//判断这一项是否被选中
int WINAPI edcIsSelected(HThumbnail h, int index);

//得到选中项列表
char* WINAPI edcGetSelectedItem(HThumbnail h);

// 设置语言　０为系统语言，１为简体中文，２为繁体中文，３为英文
int WINAPI edcSetLanguage(HThumbnail h, int langId);

// 设置菜单项
int WINAPI edcSetMenuItem(HThumbnail h, int menuId, int flag);

// 是否选择
int WINAPI edcGetCheck(HThumbnail h, int index);

// 设置选中
int WINAPI edcSetCheck(HThumbnail h, int index, BOOL bCheck);

#endif