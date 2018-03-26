#ifndef __ELOAMDLL_H__
#define __ELOAMDLL_H__

#include <Windows.h>
#include <WTypes.h>

typedef void *HELOAMIMAGEDATA;
typedef void *HELOAMIMAGE;
typedef void *HELOAMIMAGELIST;
typedef void *HELOAMFTP;
typedef void *HELOAMHTTP;
typedef void *HELOAMDEVICE;
typedef void *HELOAMVIDEO;
typedef void *HELOAMVIEW;
typedef void *HELOAMMEMORY;
typedef void *HELOAMTHUMBNAIL;
typedef void *HELOAMRECT;
typedef void *HELOAMREGION;
typedef void *HELOAMREGIONLIST;
typedef void *HELOAMFONT;
typedef void *HELOAMTEMPL;
typedef void *HELOAMVIDEOCAP;

#define THUMBNAIL_MENU_NULL				0x01
#define THUMBNAIL_MENU_BIND				0x02
#define THUMBNAIL_MENU_FOLDER			0x03
#define THUMBNAIL_MENU_FILE				0X04

#define THUMBNAIL_ITEM_REFRESH			0x00000001	
#define THUMBNAIL_ITEM_OPENFOLDERPOS	0x00000002
#define THUMBNAIL_ITEM_DELETE			0x00000004
#define THUMBNAIL_ITEM_PROPERTY			0x00000008
#define THUMBNAIL_ITEM_EDIT				0x00000010
#define THUMBNAIL_ITEM_BIND				0x00000020
#define THUMBNAIL_ITEM_UNBIND			0x00000040
#define THUMBNAIL_ITEM_OPENFILEPOS		0x00000080
#define THUMBNAIL_ITEM_BLEND			0x00000100
#define THUMBNAIL_ITEM_CREATEPDF		0x00000200
#define THUMBNAIL_ITEM_OCR				0x00000400
#define THUMBNAIL_ITEM_PRINT			0x00000800

#define EXTERN_C extern "C"
#define ELOAMAPI __stdcall

// callback
typedef VOID (ELOAMAPI *ELOAM_DEVCHANGECALLBACK)(LONG type, LONG idx, LONG dbt, LPVOID userData);
typedef VOID (ELOAMAPI *ELOAM_IDCARDCHANGECALLBACK)(LONG dbt, LPVOID userData);
typedef VOID (ELOAMAPI *ELOAM_IDCARDCALLBACK)(LONG ret, LPVOID userData);
typedef VOID (ELOAMAPI *ELOAM_OCRCALLBACK)(LONG flag, LONG value, LPVOID userData);
typedef VOID (ELOAMAPI *ELOAM_TEMPLOCRCALLBACK)(LONG ret, LPVOID userData); 
typedef VOID (ELOAMAPI *ELOAM_ARRIVALCALLBACK)(HELOAMVIDEO video, LONG id, LPVOID userData);
typedef VOID (ELOAMAPI *ELOAM_TOUCHCALLBACK)(HELOAMVIDEO video, LPVOID userData);
typedef VOID (ELOAMAPI *ELOAM_MOVEDETECCALLBACK)(HELOAMVIDEO video, LONG id, LPVOID userData);
typedef VOID (ELOAMAPI *ELOAM_VIEWCALLBACK)(HELOAMVIEW view, LONG flag, LONG value, LPVOID userData);
typedef VOID (ELOAMAPI *ELOAM_ATTACHCALLBAK)(HELOAMVIDEO video, LONG videoId, HELOAMVIEW view, LONG viewId, LPVOID userData);
typedef VOID (ELOAMAPI *ELOAM_DESKEWCALLBACK)(HELOAMVIDEO video, HELOAMVIEW view, HELOAMREGIONLIST list, LPVOID userData);
typedef VOID (ELOAMAPI *ELOAM_HTTPCALLBACK)(HELOAMHTTP http, LONG type, LONG flag, LONG value, LPVOID userData);
typedef VOID (ELOAMAPI *ELOAM_FTPCALLBACK)(HELOAMFTP ftp, LONG type, LONG flag, LONG value, LPVOID userData);
typedef VOID (ELOAMAPI *ELOAM_BIOKEYCHANGECALLBACK)(LONG dbt, LPVOID userData);
typedef VOID (ELOAMAPI *ELOAM_BIOKEYCALLBACK)(LONG type, LPVOID userData);
typedef VOID (ELOAMAPI *ELOAM_READERCHANGECALLBACK)(LONG dbt, LPVOID userData);
typedef VOID (ELOAMAPI *ELOAM_READERCALLBACK)(LONG type, LONG subtype, LPVOID userData);
typedef VOID (ELOAMAPI *ELOAM_MAGNETIICCARDALLBACK)(LONG RET, LPVOID userData);
typedef VOID (ELOAMAPI *ELOAM_SHENZHENTONGCARDALLBACK)(LONG RET, LPVOID userData);

// global
EXTERN_C BOOL ELOAMAPI EloamGlobal_DestroyString(BSTR str);
EXTERN_C HELOAMIMAGE ELOAMAPI EloamGlobal_CreateImage(LONG width, LONG height, LONG channels);
EXTERN_C HELOAMIMAGE ELOAMAPI EloamGlobal_CreateImageFromFile(BSTR fileName, LONG flag);
EXTERN_C HELOAMIMAGE ELOAMAPI EloamGlobal_CreateImageFromBase64(BSTR base64, LONG flag);
EXTERN_C HELOAMIMAGELIST ELOAMAPI EloamGlobal_CreateImageList(void);
EXTERN_C HELOAMIMAGELIST ELOAMAPI EloamGlobal_CreateImageListFromFile(BSTR fileName, LONG flag);
EXTERN_C HELOAMFTP ELOAMAPI EloamGlobal_CreateFtp(BSTR ftpPath);
EXTERN_C HELOAMHTTP ELOAMAPI EloamGlobal_CreateHttp(BSTR httpPath);
EXTERN_C HELOAMRECT ELOAMAPI EloamGlobal_CreateRect(LONG x, LONG y, LONG width, LONG height);
EXTERN_C HELOAMREGION ELOAMAPI EloamGlobal_CreateRegion(LONG x1, LONG y1, LONG x2, LONG y2, LONG x3, LONG y3, LONG x4, LONG y4);
EXTERN_C HELOAMREGIONLIST ELOAMAPI EloamGlobal_CreateRegionList(void);
EXTERN_C HELOAMFONT ELOAMAPI EloamGlobal_CreateTypeface(LONG height, LONG width, LONG escap, LONG orien, LONG thickness, LONG italic, LONG underline, LONG strike, BSTR font);
EXTERN_C HELOAMMEMORY ELOAMAPI EloamGlobal_CreateMemory(void);
EXTERN_C HELOAMMEMORY ELOAMAPI EloamGlobal_CreateMemoryFromFile(BSTR fileName);
EXTERN_C HELOAMVIEW ELOAMAPI EloamGlobal_CreateView(HWND hWnd, HELOAMRECT rect, LONG flag);
EXTERN_C HELOAMTHUMBNAIL ELOAMAPI EloamGlobal_CreateThumbnail(HWND hWnd, HELOAMRECT rect, LONG flag);
EXTERN_C BOOL ELOAMAPI EloamGlobal_PlayCaptureVoice(void);
EXTERN_C BOOL ELOAMAPI EloamGlobal_DelFile(BSTR fileName);
EXTERN_C BSTR ELOAMAPI EloamGlobal_GetTempName(BSTR ext);
EXTERN_C BOOL ELOAMAPI EloamGlobal_CreateDir(BSTR dirPath);
EXTERN_C BOOL ELOAMAPI EloamGlobal_RemoveDir(BSTR dirPath);
EXTERN_C BOOL ELOAMAPI EloamGlobal_InitDevs(ELOAM_DEVCHANGECALLBACK fun, LPVOID userData);
EXTERN_C BOOL ELOAMAPI EloamGlobal_DeinitDevs(void);
EXTERN_C LONG ELOAMAPI EloamGlobal_GetDevCount(LONG type);
EXTERN_C BSTR ELOAMAPI EloamGlobal_GetDisplayName(LONG type, LONG idx);
EXTERN_C BSTR ELOAMAPI EloamGlobal_GetFriendlyName(LONG type, LONG idx);
EXTERN_C LONG ELOAMAPI EloamGlobal_GetEloamType(LONG type, LONG idx);
EXTERN_C HELOAMDEVICE ELOAMAPI EloamGlobal_CreateDevice(LONG type, LONG idx);
EXTERN_C BOOL ELOAMAPI EloamGlobal_InitIdCard(ELOAM_IDCARDCHANGECALLBACK fun, LPVOID userData);
EXTERN_C BOOL ELOAMAPI EloamGlobal_DeinitIdCard(void);
EXTERN_C BOOL ELOAMAPI EloamGlobal_DiscernIdCard(ELOAM_IDCARDCALLBACK fun, LPVOID userData);
EXTERN_C BOOL ELOAMAPI EloamGlobal_ReadIdCard(void);
EXTERN_C HELOAMIMAGE ELOAMAPI EloamGlobal_GetIdCardImage(LONG flag);
EXTERN_C BSTR ELOAMAPI EloamGlobal_GetIdCardData(LONG flag);
EXTERN_C HELOAMMEMORY ELOAMAPI EloamGlobal_GetIdCardFingerprint();
EXTERN_C BOOL ELOAMAPI EloamGlobal_StopIdCardDiscern(void);
EXTERN_C BOOL ELOAMAPI EloamGlobal_InitBarcode(void);
EXTERN_C BOOL ELOAMAPI EloamGlobal_DeinitBarcode(void);
EXTERN_C BOOL ELOAMAPI EloamGlobal_DiscernBarcode(HELOAMIMAGE img);
EXTERN_C LONG ELOAMAPI EloamGlobal_GetBarcodeCount(void);
EXTERN_C LONG ELOAMAPI EloamGlobal_GetBarcodeType(LONG idx);
EXTERN_C BSTR ELOAMAPI EloamGlobal_GetBarcodeData(LONG idx);
EXTERN_C BOOL ELOAMAPI EloamGlobal_InitFaceDetect(void);
EXTERN_C BOOL ELOAMAPI EloamGlobal_DeinitFaceDetect(void);
EXTERN_C HELOAMRECT ELOAMAPI EloamGlobal_GetFaceRect(HELOAMIMAGE img);
EXTERN_C LONG ELOAMAPI EloamGlobal_DiscernFaceDetect(HELOAMIMAGE img1, HELOAMIMAGE img2);
EXTERN_C BOOL ELOAMAPI EloamGlobal_EnableFaceRectCrop(HELOAMVIDEO video, LONG flag);
EXTERN_C BOOL ELOAMAPI EloamGlobal_DisableFaceRectCrop(HELOAMVIDEO video);
EXTERN_C BOOL ELOAMAPI EloamGlobal_InitOcr(void);
EXTERN_C BOOL ELOAMAPI EloamGlobal_DeinitOcr(void);
EXTERN_C BOOL ELOAMAPI EloamGlobal_SetOcrLanguage(LONG lang);
EXTERN_C BOOL ELOAMAPI EloamGlobal_DiscernOcr(LONG flag, HELOAMIMAGE img, ELOAM_OCRCALLBACK fun, LPVOID userData);
EXTERN_C BOOL ELOAMAPI EloamGlobal_DiscernOcrList(LONG flag, HELOAMIMAGELIST list, ELOAM_OCRCALLBACK fun, LPVOID userData);
EXTERN_C LONG ELOAMAPI EloamGlobal_GetOcrPageCount(void);
EXTERN_C LONG ELOAMAPI EloamGlobal_GetOcrBlockCount(LONG page);
EXTERN_C LONG ELOAMAPI EloamGlobal_GetOcrBarcodeType(LONG page, LONG blk);
EXTERN_C BSTR ELOAMAPI EloamGlobal_GetOcrPlainText(LONG page);
EXTERN_C BSTR ELOAMAPI EloamGlobal_GetOcrData(LONG page, LONG blk);
EXTERN_C BOOL ELOAMAPI EloamGlobal_SaveOcr(BSTR fileName, LONG flag);
EXTERN_C BOOL ELOAMAPI EloamGlobal_StopOcrDiscern(void);
EXTERN_C BOOL ELOAMAPI EloamGlobal_WaitOcrDiscern(void);
EXTERN_C BOOL ELOAMAPI EloamGlobal_InitBiokey(ELOAM_BIOKEYCHANGECALLBACK fun, LPVOID userData);
EXTERN_C BOOL ELOAMAPI EloamGlobal_DeinitBiokey(void);
EXTERN_C BOOL ELOAMAPI EloamGlobal_GetBiokeyTemplate(ELOAM_BIOKEYCALLBACK fun, LPVOID userData);
EXTERN_C BOOL ELOAMAPI EloamGlobal_StopGetBiokeyTemplate(void);
EXTERN_C HELOAMMEMORY ELOAMAPI EloamGlobal_GetBiokeyTemplateData(void);
EXTERN_C BOOL ELOAMAPI EloamGlobal_GetBiokeyFeature(ELOAM_BIOKEYCALLBACK fun, LPVOID userData);
EXTERN_C BOOL ELOAMAPI EloamGlobal_StopGetBiokeyFeature(void);
EXTERN_C HELOAMMEMORY ELOAMAPI EloamGlobal_GetBiokeyFeatureData(void);
EXTERN_C HELOAMIMAGE ELOAMAPI EloamGlobal_GetBiokeyImage(void);
EXTERN_C LONG ELOAMAPI EloamGlobal_BiokeyVerify(HELOAMMEMORY mem1, HELOAMMEMORY mem2);
EXTERN_C LONG ELOAMAPI EloamGlobal_BiokeyVerifyFromString(BSTR str1, BSTR str2);
EXTERN_C HELOAMTEMPL ELOAMAPI EloamGlobal_CreateTempl(BSTR templName);
EXTERN_C HELOAMTEMPL ELOAMAPI EloamGlobal_CreateTemplFromFile(BSTR fileName, LONG flag);
EXTERN_C HELOAMTEMPL ELOAMAPI EloamGlobal_CreateTemplFromData(BSTR data, LONG flag);
EXTERN_C BOOL ELOAMAPI EloamGlobal_InitTemplOcr(void);
EXTERN_C BOOL ELOAMAPI EloamGlobal_DeinitTemplOcr(void);
EXTERN_C BOOL ELOAMAPI EloamGlobal_SetTemplOcrLanguage(LONG lang);
EXTERN_C BOOL ELOAMAPI EloamGlobal_DiscernTempl(HELOAMIMAGE image, HELOAMTEMPL templ, ELOAM_TEMPLOCRCALLBACK fun, LPVOID userData);
EXTERN_C HELOAMTEMPL ELOAMAPI EloamGlobal_GetTemplResult(void);
EXTERN_C BOOL ELOAMAPI EloamGlobal_StopTemplDiscern(void);
EXTERN_C BOOL ELOAMAPI EloamGlobal_WaitTemplDiscern(void);
EXTERN_C BOOL ELOAMAPI EloamGlobal_QuickOcr(BSTR fileName, BSTR resultPath, LONG flag);
EXTERN_C BOOL ELOAMAPI EloamGlobal_InitReader(ELOAM_READERCHANGECALLBACK fun, LPVOID userData);
EXTERN_C BOOL ELOAMAPI EloamGlobal_DeinitReader(void);
EXTERN_C BOOL ELOAMAPI EloamGlobal_ReaderStart(ELOAM_READERCALLBACK fun, LPVOID userData);
EXTERN_C BOOL ELOAMAPI EloamGlobal_ReaderStop(void);
EXTERN_C BSTR ELOAMAPI EloamGlobal_ReaderGetCpuId(void);

EXTERN_C BOOL ELOAMAPI EloamGlobal_CpuGetBankCardTrack(BSTR *info);
EXTERN_C LONG ELOAMAPI EloamGlobal_CpuGetRecordNumber();
EXTERN_C BOOL ELOAMAPI EloamGlobal_CpuGetankCardRecord(LONG index, BSTR *data);

EXTERN_C BSTR ELOAMAPI EloamGlobal_ReaderGetCpuCreditCardNumber(void);
EXTERN_C BSTR ELOAMAPI EloamGlobal_ReaderGetMemoryId(void);
EXTERN_C BSTR ELOAMAPI EloamGlobal_ReaderGetQuickPassData(LONG type);
EXTERN_C BSTR ELOAMAPI EloamGlobal_ReaderGetM1Id(void);
EXTERN_C BSTR ELOAMAPI EloamGlobal_ReaderGetSocialCardData(LONG flag);
EXTERN_C BOOL ELOAMAPI EloamGlobal_InitMagneticCard(void);
EXTERN_C BOOL ELOAMAPI EloamGlobal_DeinitMagneticCard(void);
EXTERN_C BOOL ELOAMAPI EloamGlobal_MagneticCardReaderStart(ELOAM_MAGNETIICCARDALLBACK fun, LPVOID userData);
EXTERN_C BOOL ELOAMAPI EloamGlobal_MagneticCardReaderStop(void);
EXTERN_C BSTR ELOAMAPI EloamGlobal_MagneticCardGetData(LONG type);
EXTERN_C BSTR ELOAMAPI EloamGlobal_MagneticCardGetNumber();

EXTERN_C BOOL EloamGlobal_InitShenZhenTong(	void);
EXTERN_C BOOL EloamGlobal_DeinitShenZhenTong(void);
EXTERN_C BOOL EloamGlobal_StartShenZhenTongCard(ELOAM_SHENZHENTONGCARDALLBACK fun, void *param);
EXTERN_C BOOL EloamGlobal_StopShenZhenTongCard(void);
EXTERN_C BOOL EloamGlobal_GetShenZhenTongNumber(BSTR *number,BSTR *amount);
EXTERN_C LONG EloamGlobal_GetShenZhenTongCardRecordNumber();
EXTERN_C BOOL EloamGlobal_GetShenZhenTongCardRecord(LONG index, BSTR *data);


EXTERN_C BOOL ELOAMAPI EloamGlobal_IsUserAdmin();
EXTERN_C LONG ELOAMAPI EloamGlobal_GetPrinterCount();
EXTERN_C BSTR ELOAMAPI EloamGlobal_GetPrinterName(LONG idx);

EXTERN_C BOOL EloamGlobal_VideoCapInit();
EXTERN_C HELOAMVIDEOCAP EloamGlobal_CreatVideoCap();
EXTERN_C BOOL EloamGlobal_DestroyVideoCap(HELOAMVIDEOCAP hVideoCap);
EXTERN_C BOOL EloamGlobal_VideoCapPreCap(
	HELOAMVIDEOCAP hVideoCap,
	BSTR outputFileName,
	int micIndex,	
	int frameRate,				
	int compressMode,	
	int videoWidth, 
	int videoHeight,	
	BOOL bCapVideo);
EXTERN_C BOOL EloamGlobal_VideoCapStart(
	HELOAMVIDEOCAP hVideoCap
	);
EXTERN_C BOOL  EloamGlobal_VideoCapStop(HELOAMVIDEOCAP hVideoCap);
EXTERN_C BOOL  EloamGlobal_VideoCapPause(HELOAMVIDEOCAP hVideoCap);
EXTERN_C BOOL  EloamGlobal_VideoCapSetWatermark(
	HELOAMVIDEOCAP hVideoCap,
	LONG bWatermark,	
	LONG bAddTime ,	
	LONG mode,
	LONG pos,	
	LONG alpha,	
	BSTR imgPath ,	
	BSTR pStrText,	
	LONG Color = RGB(255,0,0),	
	BSTR faceName =(BSTR) "宋体",
	LONG lHeight= 42,	
	LONG lWeight = FW_NORMAL,
	LONG lItalic= FALSE);
EXTERN_C BOOL EloamGlobal_VideoCapAddVideoSrc(HELOAMVIDEOCAP hVideoCap, HELOAMVIDEO video);
EXTERN_C BOOL EloamGlobal_VideoCapAddVideoSrcEx(HELOAMVIDEOCAP hVideoCap, HELOAMVIDEO video, LONG dstX, LONG dstY, LONG dstWidth, LONG dstHeight, LONG bProportion, LONG alpha);
EXTERN_C BOOL EloamGlobal_VideoCapRemoveAllVideoSrc(HELOAMVIDEOCAP hVideoCap);
EXTERN_C LONG EloamGlobal_VideoCapGetAudioDevNum();
EXTERN_C BSTR EloamGlobal_VideoCapGetAudioDevName( LONG devIndex);
EXTERN_C LONG EloamGlobal_VideoCapGetState(HELOAMVIDEOCAP hVideoCap);
EXTERN_C HELOAMMEMORY EloamGlobal_CreateMemoryFromString(BSTR data);
EXTERN_C BOOL ELOAMAPI EloamGlobal_SetSoftDog(BOOL open);

// image
EXTERN_C LONG ELOAMAPI EloamImage_AddRef(HELOAMIMAGE img);
EXTERN_C LONG ELOAMAPI EloamImage_Release(HELOAMIMAGE img);
EXTERN_C HELOAMIMAGEDATA ELOAMAPI EloamImage_GetImageData(HELOAMIMAGE img);
EXTERN_C HELOAMMEMORY ELOAMAPI EloamImage_CreateMemory(HELOAMIMAGE img, LONG fmt, LONG flag);
EXTERN_C BOOL ELOAMAPI EloamImage_Copy(HELOAMIMAGE imgDest, HELOAMIMAGE imgSrc);
EXTERN_C BOOL ELOAMAPI EloamImage_Save(HELOAMIMAGE img, BSTR fileName, LONG flag);
EXTERN_C BOOL ELOAMAPI EloamImage_SaveToPDF(HELOAMIMAGE img, LONG fmt, BSTR fileName, LONG flag);
EXTERN_C BOOL ELOAMAPI EloamImage_SetDiscernRect(HELOAMIMAGE img, HELOAMRECT rect);
EXTERN_C BOOL ELOAMAPI EloamImage_Print(HELOAMIMAGE img, FLOAT x, FLOAT y, FLOAT width, FLOAT height, BSTR printer);
EXTERN_C BOOL ELOAMAPI EloamImage_PrintByDPI(HELOAMIMAGE img, FLOAT x, FLOAT y, BSTR printer);
EXTERN_C BOOL ELOAMAPI EloamImage_AdaptivePrint(HELOAMIMAGE img, FLOAT width, FLOAT height, BSTR printer);
EXTERN_C BOOL ELOAMAPI EloamImage_AdaptivePrintByDPI(HELOAMIMAGE img, BSTR printer);
EXTERN_C LONG ELOAMAPI EloamImage_GetWidth(HELOAMIMAGE img);
EXTERN_C LONG ELOAMAPI EloamImage_GetHeight(HELOAMIMAGE img);
EXTERN_C LONG ELOAMAPI EloamImage_GetChannels(HELOAMIMAGE img);
EXTERN_C LONG ELOAMAPI EloamImage_GetWidthStep(HELOAMIMAGE img);
EXTERN_C LONG ELOAMAPI EloamImage_GetXDPI(HELOAMIMAGE img);
EXTERN_C LONG ELOAMAPI EloamImage_GetYDPI(HELOAMIMAGE img);
EXTERN_C BOOL ELOAMAPI EloamImage_SetXDPI(HELOAMIMAGE img, LONG value);
EXTERN_C BOOL ELOAMAPI EloamImage_SetYDPI(HELOAMIMAGE img, LONG value);
EXTERN_C BSTR ELOAMAPI EloamImage_GetBase64(HELOAMIMAGE img, LONG fmt, LONG flag);
EXTERN_C BSTR ELOAMAPI EloamImage_GetMD5(HELOAMIMAGE img, LONG fmt, LONG flag);
EXTERN_C BOOL ELOAMAPI EloamImage_DrawText(HELOAMIMAGE img, HELOAMFONT font, LONG x, LONG y, BSTR text, COLORREF clr, LONG weight);
EXTERN_C BOOL ELOAMAPI EloamImage_DrawTextEx(HELOAMIMAGE img, LONG pos, BSTR text, COLORREF clr, LONG weight);
EXTERN_C BOOL ELOAMAPI EloamImage_Rotate(HELOAMIMAGE img, FLOAT angle, COLORREF clr, LONG flag);
EXTERN_C BOOL ELOAMAPI EloamImage_Crop(HELOAMIMAGE img, HELOAMRECT rect);
EXTERN_C BOOL ELOAMAPI EloamImage_Resize(HELOAMIMAGE img, LONG width, LONG height, LONG flag);
EXTERN_C BOOL ELOAMAPI EloamImage_Blend(HELOAMIMAGE imgDest, HELOAMRECT rectDest, HELOAMIMAGE imgSrc, HELOAMRECT rectSrc, LONG weight, LONG flag);
EXTERN_C BOOL ELOAMAPI EloamImage_ToColor(HELOAMIMAGE img);
EXTERN_C BOOL ELOAMAPI EloamImage_ToGray(HELOAMIMAGE img);
EXTERN_C BOOL ELOAMAPI EloamImage_Threshold(HELOAMIMAGE img, LONG threshold);
EXTERN_C BOOL ELOAMAPI EloamImage_AdaptiveThreshold(HELOAMIMAGE img, LONG flag);
EXTERN_C BOOL ELOAMAPI EloamImage_Deskew(HELOAMIMAGE img, LONG flag);
EXTERN_C BOOL ELOAMAPI EloamImage_Reverse(HELOAMIMAGE img);
EXTERN_C BOOL ELOAMAPI EloamImage_Smooth(HELOAMIMAGE img, LONG flag);
EXTERN_C BOOL ELOAMAPI EloamImage_Rectify(HELOAMIMAGE img, LONG flag);
EXTERN_C BOOL ELOAMAPI EloamImage_DelImageBackgrand(HELOAMIMAGE img, HELOAMIMAGE dest);
EXTERN_C BOOL ELOAMAPI EloamImage_Denoising(HELOAMIMAGE img, HELOAMIMAGE dest);
EXTERN_C BOOL ELOAMAPI EloamImage_Balance(HELOAMIMAGE img, HELOAMIMAGE dest);
EXTERN_C BOOL ELOAMAPI EloamImage_IdCardProcess(HELOAMIMAGE img);
EXTERN_C BOOL ELOAMAPI EloamImage_FormProcess(HELOAMIMAGE img);
EXTERN_C BOOL ELOAMAPI EloamImage_Whiten(HELOAMIMAGE img, LONG flag, LONG threshold, FLOAT autoThresholdRatio, LONG aroundNum, LONG lowestBrightness);
EXTERN_C BOOL ELOAMAPI EloamImage_EraseBlackEdge(HELOAMIMAGE img, LONG flag);
// memory
EXTERN_C LONG ELOAMAPI EloamMemory_AddRef(HELOAMMEMORY mem);
EXTERN_C LONG ELOAMAPI EloamMemory_Release(HELOAMMEMORY mem);
EXTERN_C BSTR ELOAMAPI EloamMemory_GetBase64(HELOAMMEMORY mem);
EXTERN_C HELOAMIMAGE ELOAMAPI EloamMemory_CreateImage(HELOAMMEMORY mem, LONG flag);
EXTERN_C BOOL ELOAMAPI EloamMemory_Save(HELOAMMEMORY mem, BSTR fileName);
EXTERN_C BSTR ELOAMAPI EloamMemory_GetString(HELOAMMEMORY mem);

//MD5
EXTERN_C BSTR ELOAMAPI EloamMemory_GetMD5(HELOAMMEMORY mem);

// image list
EXTERN_C LONG ELOAMAPI EloamImageList_AddRef(HELOAMIMAGELIST list);
EXTERN_C LONG ELOAMAPI EloamImageList_Release(HELOAMIMAGELIST list);
EXTERN_C BOOL ELOAMAPI EloamImageList_Add(HELOAMIMAGELIST list, HELOAMIMAGE img);
EXTERN_C BOOL ELOAMAPI EloamImageList_Insert(HELOAMIMAGELIST list, HELOAMIMAGE img, LONG pos);
EXTERN_C BOOL ELOAMAPI EloamImageList_Remove(HELOAMIMAGELIST list, LONG idx);
EXTERN_C BOOL ELOAMAPI EloamImageList_Clear(HELOAMIMAGELIST list);
EXTERN_C LONG ELOAMAPI EloamImageList_GetCount(HELOAMIMAGELIST list);
EXTERN_C HELOAMIMAGE ELOAMAPI EloamImageList_GetImage(HELOAMIMAGELIST list, LONG idx);
EXTERN_C BOOL ELOAMAPI EloamImageList_Save(HELOAMIMAGELIST list, BSTR fileName, LONG flag);
EXTERN_C BOOL ELOAMAPI EloamImageList_SaveToPDF(HELOAMIMAGELIST list, LONG fmt, BSTR fileName, LONG flag);

// http
EXTERN_C LONG ELOAMAPI EloamHttp_AddRef(HELOAMHTTP http);
EXTERN_C LONG ELOAMAPI EloamHttp_Release(HELOAMHTTP http);
EXTERN_C BOOL ELOAMAPI EloamHttp_Upload(HELOAMHTTP http, LONG flag, BSTR localPath, BSTR headers, BSTR predata, BSTR taildata, ELOAM_HTTPCALLBACK fun, LPVOID userData);
EXTERN_C BOOL ELOAMAPI EloamHttp_UploadMemory(HELOAMHTTP http, LONG flag, HELOAMMEMORY mem, BSTR headers, BSTR predata, BSTR taildata, ELOAM_HTTPCALLBACK fun, LPVOID userData);
EXTERN_C BOOL ELOAMAPI EloamHttp_UploadImageFile(HELOAMHTTP http, BSTR fileName, BSTR remoteName, ELOAM_HTTPCALLBACK fun, LPVOID userData);
EXTERN_C BOOL ELOAMAPI EloamHttp_UploadImageMemory(HELOAMHTTP http, HELOAMMEMORY mem, BSTR remoteName, ELOAM_HTTPCALLBACK fun, LPVOID userData);
EXTERN_C BOOL ELOAMAPI EloamHttp_UploadImage(HELOAMHTTP http, HELOAMIMAGE img, int fmt, int flag, BSTR remoteName, ELOAM_HTTPCALLBACK fun, LPVOID userData);
EXTERN_C BOOL ELOAMAPI EloamHttp_StopUpload(HELOAMHTTP http);
EXTERN_C BOOL ELOAMAPI EloamHttp_WaitUpload(HELOAMHTTP http);
EXTERN_C BSTR ELOAMAPI EloamHttp_GetServerInfo(HELOAMHTTP http);

// ftp
EXTERN_C LONG ELOAMAPI EloamFtp_AddRef(HELOAMFTP ftp);
EXTERN_C LONG ELOAMAPI EloamFtp_Release(HELOAMFTP ftp);
EXTERN_C BOOL ELOAMAPI EloamFtp_Upload(HELOAMFTP ftp, LONG flag, BSTR localPath, BSTR remotePath, ELOAM_FTPCALLBACK fun, LPVOID userData);
EXTERN_C BOOL ELOAMAPI EloamFtp_StopUpload(HELOAMFTP ftp);
EXTERN_C BOOL ELOAMAPI EloamFtp_WaitUpload(HELOAMFTP ftp);
EXTERN_C BOOL ELOAMAPI EloamFtp_CreateDir(HELOAMFTP ftp, BSTR dirPath);
EXTERN_C BOOL ELOAMAPI EloamFtp_RemoveDir(HELOAMFTP ftp, BSTR dirPath);

// device
EXTERN_C LONG ELOAMAPI EloamDevice_AddRef(HELOAMDEVICE dev);
EXTERN_C LONG ELOAMAPI EloamDevice_Release(HELOAMDEVICE dev);
EXTERN_C LONG ELOAMAPI EloamDevice_GetType(HELOAMDEVICE dev);
EXTERN_C LONG ELOAMAPI EloamDevice_GetIndex(HELOAMDEVICE dev);
EXTERN_C LONG ELOAMAPI EloamDevice_GetState(HELOAMDEVICE dev);
EXTERN_C BSTR ELOAMAPI EloamDevice_GetFriendlyName(HELOAMDEVICE dev);
EXTERN_C BSTR ELOAMAPI EloamDevice_GetDisplayName(HELOAMDEVICE dev);
EXTERN_C LONG ELOAMAPI EloamDevice_GetEloamType(HELOAMDEVICE dev);
EXTERN_C LONG ELOAMAPI EloamDevice_GetScanSize(HELOAMDEVICE dev);
EXTERN_C LONG ELOAMAPI EloamDevice_GetSubtype(HELOAMDEVICE dev);
EXTERN_C LONG ELOAMAPI EloamDevice_GetResolutionCount(HELOAMDEVICE dev);
EXTERN_C LONG ELOAMAPI EloamDevice_GetResolutionWidth(HELOAMDEVICE dev, LONG idx);
EXTERN_C LONG ELOAMAPI EloamDevice_GetResolutionHeight(HELOAMDEVICE dev, LONG idx);
EXTERN_C LONG ELOAMAPI EloamDevice_GetResolutionCountEx(HELOAMDEVICE dev, LONG subType);
EXTERN_C LONG ELOAMAPI EloamDevice_GetResolutionWidthEx(HELOAMDEVICE dev, LONG subType, LONG idx);
EXTERN_C LONG ELOAMAPI EloamDevice_GetResolutionHeightEx(HELOAMDEVICE dev, LONG subType,LONG idx);
EXTERN_C BOOL ELOAMAPI EloamDevice_PausePreview(HELOAMDEVICE dev);
EXTERN_C BOOL ELOAMAPI EloamDevice_ResumePreview(HELOAMDEVICE dev);
EXTERN_C LONG ELOAMAPI EloamDevice_GetVideoProcAmp(HELOAMDEVICE dev, LONG prop, LONG value);
EXTERN_C BOOL ELOAMAPI EloamDevice_SetVideoProcAmp(HELOAMDEVICE dev, LONG prop, LONG value, BOOL isAuto);
EXTERN_C LONG ELOAMAPI EloamDevice_GetCameraControl(HELOAMDEVICE dev, LONG prop, LONG value);
EXTERN_C BOOL ELOAMAPI EloamDevice_SetCameraControl(HELOAMDEVICE dev, LONG prop, LONG value, BOOL isAuto);
EXTERN_C BOOL ELOAMAPI EloamDevice_ShowProperty(HELOAMDEVICE dev, HWND hWnd);
EXTERN_C BOOL ELOAMAPI EloamDevice_ShowPropertyEx(HELOAMDEVICE dev, HELOAMVIEW view);
EXTERN_C BSTR ELOAMAPI EloamDevice_GetSonixSerialNumber(HELOAMDEVICE dev);
EXTERN_C HELOAMVIDEO ELOAMAPI EloamDevice_CreateVideo(HELOAMDEVICE dev, LONG resolution, LONG subtype, ELOAM_ARRIVALCALLBACK funArrival, LPVOID userArrival, ELOAM_TOUCHCALLBACK funTouch, LPVOID userTouch);

// video
EXTERN_C LONG ELOAMAPI EloamVideo_AddRef(HELOAMVIDEO video);
EXTERN_C LONG ELOAMAPI EloamVideo_Release(HELOAMVIDEO video);
EXTERN_C HELOAMDEVICE ELOAMAPI EloamVideo_GetDevice(HELOAMVIDEO video);
EXTERN_C LONG ELOAMAPI EloamVideo_GetResolution(HELOAMVIDEO video);
EXTERN_C LONG ELOAMAPI EloamVideo_GetSubtype(HELOAMVIDEO video);
EXTERN_C LONG ELOAMAPI EloamVideo_GetWidth(HELOAMVIDEO video);
EXTERN_C LONG ELOAMAPI EloamVideo_GetHeight(HELOAMVIDEO video);
EXTERN_C HELOAMIMAGE ELOAMAPI EloamVideo_CreateImage(HELOAMVIDEO video, LONG scanSize, HELOAMVIEW view);
EXTERN_C HELOAMIMAGELIST ELOAMAPI EloamVideo_CreateImageList(HELOAMVIDEO video, LONG scanSize, HELOAMVIEW view);
EXTERN_C BOOL ELOAMAPI EloamVideo_RotateLeft(HELOAMVIDEO video);
EXTERN_C BOOL ELOAMAPI EloamVideo_RotateRight(HELOAMVIDEO video);
EXTERN_C BOOL ELOAMAPI EloamVideo_Rotate180(HELOAMVIDEO video);
EXTERN_C BOOL ELOAMAPI EloamVideo_Flip(HELOAMVIDEO video);
EXTERN_C BOOL ELOAMAPI EloamVideo_Mirror(HELOAMVIDEO video);
EXTERN_C BOOL ELOAMAPI EloamVideo_FlipAndMirror(HELOAMVIDEO video);
EXTERN_C BOOL ELOAMAPI EloamVideo_EnableGray(HELOAMVIDEO video);
EXTERN_C BOOL ELOAMAPI EloamVideo_DisableGray(HELOAMVIDEO video);
EXTERN_C BOOL ELOAMAPI EloamVideo_EnableAdaptiveThreshold(HELOAMVIDEO video, LONG flag);
EXTERN_C BOOL ELOAMAPI EloamVideo_DisableAdaptiveThreshold(HELOAMVIDEO video);
EXTERN_C BOOL ELOAMAPI EloamVideo_EnableThreshold(HELOAMVIDEO video, LONG threshold);
EXTERN_C BOOL ELOAMAPI EloamVideo_DisableThreshold(HELOAMVIDEO video);
EXTERN_C BOOL ELOAMAPI EloamVideo_EnableDelBkColor(HELOAMVIDEO video, LONG flag);
EXTERN_C BOOL ELOAMAPI EloamVideo_DisableDelBkColor(HELOAMVIDEO video);
EXTERN_C BOOL ELOAMAPI EloamVideo_EnableAddText(HELOAMVIDEO video, HELOAMFONT font, LONG x, LONG y, BSTR text, COLORREF clr, LONG weight);
EXTERN_C BOOL ELOAMAPI EloamVideo_EnableAddTextEx(HELOAMVIDEO video, LONG pos, BSTR text, COLORREF clr, LONG weight);
EXTERN_C BOOL ELOAMAPI EloamVideo_DisableAddText(HELOAMVIDEO video);
EXTERN_C BOOL ELOAMAPI EloamVideo_EnableDeskew(HELOAMVIDEO video, LONG flag);
EXTERN_C BOOL ELOAMAPI EloamVideo_EnableDeskewEx(HELOAMVIDEO video, LONG flag);
EXTERN_C BOOL ELOAMAPI EloamVideo_EnableDeskew2(HELOAMVIDEO video, LONG flag, ELOAM_DESKEWCALLBACK fun, LPVOID userData);	// 回调版本
EXTERN_C BOOL ELOAMAPI EloamVideo_EnableDeskew2Ex(HELOAMVIDEO video, LONG flag, ELOAM_DESKEWCALLBACK fun, LPVOID userData); // 回调版本
EXTERN_C BOOL ELOAMAPI EloamVideo_DisableDeskew(HELOAMVIDEO video);
EXTERN_C BOOL ELOAMAPI EloamVideo_EnableReverse(HELOAMVIDEO video);
EXTERN_C BOOL ELOAMAPI EloamVideo_DisableReverse(HELOAMVIDEO video);
EXTERN_C BOOL ELOAMAPI EloamVideo_EnableSmooth(HELOAMVIDEO video, LONG flag);
EXTERN_C BOOL ELOAMAPI EloamVideo_DisableSmooth(HELOAMVIDEO video);
EXTERN_C BOOL ELOAMAPI EloamVideo_EnableMoveDetec(HELOAMVIDEO video, LONG flag, ELOAM_MOVEDETECCALLBACK fun, LPVOID userData);
EXTERN_C BOOL ELOAMAPI EloamVideo_DisableMoveDetec(HELOAMVIDEO video);
EXTERN_C BOOL ELOAMAPI EloamVideo_EnableDate(HELOAMVIDEO video, HELOAMFONT font, LONG x, LONG y, COLORREF clr, LONG weight);
EXTERN_C BOOL ELOAMAPI EloamVideo_EnableDateEx(HELOAMVIDEO video, LONG pos, COLORREF clr, LONG weight);
EXTERN_C BOOL ELOAMAPI EloamVideo_DisableDate(HELOAMVIDEO video);
EXTERN_C BOOL ELOAMAPI EloamVideo_SetCropState(HELOAMVIDEO video, LONG state);

EXTERN_C BOOL ELOAMAPI EloamVideo_StartRecord(HELOAMVIDEO video, BSTR fileName, LONG flag);
EXTERN_C BOOL ELOAMAPI EloamVideo_StopRecord(HELOAMVIDEO video);

EXTERN_C BOOL EloamVideo_FindRect(HELOAMVIDEO video,HELOAMRECT rect);
EXTERN_C BOOL EloamVideo_SetDisplayRect(	HELOAMVIDEO video,HELOAMRECT rect,BOOL enable);

// view
EXTERN_C LONG ELOAMAPI EloamView_AddRef(HELOAMVIEW view);
EXTERN_C LONG ELOAMAPI EloamView_Release(HELOAMVIEW view);
EXTERN_C BOOL ELOAMAPI EloamView_SetCallback(HELOAMVIEW view, ELOAM_VIEWCALLBACK fun, LPVOID userData);
EXTERN_C BOOL ELOAMAPI EloamView_SelectImage(HELOAMVIEW view, HELOAMIMAGE image);
EXTERN_C BOOL ELOAMAPI EloamView_SelectVideo(HELOAMVIEW view, HELOAMVIDEO video, ELOAM_ATTACHCALLBAK fun, LPVOID userData);
EXTERN_C BOOL ELOAMAPI EloamView_SelectNull(HELOAMVIEW view);
EXTERN_C BOOL ELOAMAPI EloamView_SetZoomIn(HELOAMVIEW view);
EXTERN_C BOOL ELOAMAPI EloamView_SetZoomOut(HELOAMVIEW view);
EXTERN_C BOOL ELOAMAPI EloamView_SetCarpete(HELOAMVIEW view);
EXTERN_C BOOL ELOAMAPI EloamView_SetOriginal(HELOAMVIEW view);
EXTERN_C BOOL ELOAMAPI EloamView_SetAdapt(HELOAMVIEW view);
EXTERN_C VOID ELOAMAPI EloamView_EnableFullScreen(HELOAMVIEW view, BOOL bFull);
EXTERN_C BOOL ELOAMAPI EloamView_SetFullScreen(HELOAMVIEW view, BOOL bFull);
EXTERN_C LONG ELOAMAPI EloamView_GetState(HELOAMVIEW view);
EXTERN_C BOOL ELOAMAPI EloamView_SetState(HELOAMVIEW view, LONG state);
EXTERN_C BOOL ELOAMAPI EloamView_SetBkColor(HELOAMVIEW view, COLORREF clr);
EXTERN_C BOOL ELOAMAPI EloamView_SetText(HELOAMVIEW view, BSTR text, COLORREF clr);
EXTERN_C HELOAMRECT ELOAMAPI EloamView_GetSelectedRect(HELOAMVIEW view);
EXTERN_C HELOAMRECT ELOAMAPI EloamView_GetImageRect(HELOAMVIEW view);
EXTERN_C BOOL ELOAMAPI EloamView_SetSelectedRect(HELOAMVIEW view, HELOAMRECT rect);
EXTERN_C BOOL ELOAMAPI EloamView_PlayCaptureEffect(HELOAMVIEW view);
EXTERN_C BOOL ELOAMAPI EloamView_SetScale(HELOAMVIEW view, LONG scale);
EXTERN_C BOOL ELOAMAPI EloamView_SetRatio(HELOAMVIEW view, LONG ratio);
EXTERN_C BOOL ELOAMAPI EloamView_SetMessage(HELOAMVIEW view, BSTR msg);
EXTERN_C BOOL ELOAMAPI EloamView_SetRectangleFormat(HELOAMVIEW view, int flag, int lineSize, COLORREF lineColor,
	int lineStyle, int pointSize, COLORREF pointColor);

EXTERN_C BOOL ELOAMAPI EloamView_DrawCustomRect(HELOAMVIEW view, int flag,HELOAMRECT rect,int cWidth,COLORREF color);

// rect
EXTERN_C LONG ELOAMAPI EloamRect_AddRef(HELOAMRECT rect);
EXTERN_C LONG ELOAMAPI EloamRect_Release(HELOAMRECT rect);
EXTERN_C LONG ELOAMAPI EloamRect_GetLeft(HELOAMRECT rect);
EXTERN_C BOOL ELOAMAPI EloamRect_SetLeft(HELOAMRECT rect, LONG left);
EXTERN_C LONG ELOAMAPI EloamRect_GetTop(HELOAMRECT rect);
EXTERN_C BOOL ELOAMAPI EloamRect_SetTop(HELOAMRECT rect, LONG top);
EXTERN_C LONG ELOAMAPI EloamRect_GetWidth(HELOAMRECT rect);
EXTERN_C BOOL ELOAMAPI EloamRect_SetWidth(HELOAMRECT rect, LONG width);
EXTERN_C LONG ELOAMAPI EloamRect_GetHeight(HELOAMRECT rect);
EXTERN_C BOOL ELOAMAPI EloamRect_SetHeight(HELOAMRECT rect, LONG height);
EXTERN_C BOOL ELOAMAPI EloamRect_Copy(HELOAMRECT rectDest, HELOAMRECT rectSrc);

// region
EXTERN_C LONG ELOAMAPI EloamRegion_AddRef(HELOAMREGION region);
EXTERN_C LONG ELOAMAPI EloamRegion_Release(HELOAMREGION region);
EXTERN_C LONG ELOAMAPI EloamRegion_GetX1(HELOAMREGION region);
EXTERN_C LONG ELOAMAPI EloamRegion_GetY1(HELOAMREGION region);
EXTERN_C LONG ELOAMAPI EloamRegion_GetX2(HELOAMREGION region);
EXTERN_C LONG ELOAMAPI EloamRegion_GetY2(HELOAMREGION region);
EXTERN_C LONG ELOAMAPI EloamRegion_GetX3(HELOAMREGION region);
EXTERN_C LONG ELOAMAPI EloamRegion_GetY3(HELOAMREGION region);
EXTERN_C LONG ELOAMAPI EloamRegion_GetX4(HELOAMREGION region);
EXTERN_C LONG ELOAMAPI EloamRegion_GetY4(HELOAMREGION region);
EXTERN_C LONG ELOAMAPI EloamRegion_GetWidth(HELOAMREGION region);
EXTERN_C LONG ELOAMAPI EloamRegion_GetHeight(HELOAMREGION region);

// region list
EXTERN_C LONG ELOAMAPI EloamRegionList_AddRef(HELOAMREGIONLIST list);
EXTERN_C LONG ELOAMAPI EloamRegionList_Release(HELOAMREGIONLIST list);
EXTERN_C BOOL ELOAMAPI EloamRegionList_Add(HELOAMREGIONLIST list, HELOAMREGION region);
EXTERN_C BOOL ELOAMAPI EloamRegionList_Insert(HELOAMREGIONLIST list, HELOAMREGION region, LONG pos);
EXTERN_C BOOL ELOAMAPI EloamRegionList_Remove(HELOAMREGIONLIST list, LONG idx);
EXTERN_C BOOL ELOAMAPI EloamRegionList_Clear(HELOAMREGIONLIST list);
EXTERN_C LONG ELOAMAPI EloamRegionList_GetCount(HELOAMREGIONLIST list);
EXTERN_C HELOAMREGION ELOAMAPI EloamRegionList_GetRegion(HELOAMREGIONLIST list, LONG idx);

// thumbnail
EXTERN_C LONG ELOAMAPI EloamThumbnail_AddRef(HELOAMTHUMBNAIL thumb);
EXTERN_C LONG ELOAMAPI EloamThumbnail_Release(HELOAMTHUMBNAIL thumb);
EXTERN_C BOOL ELOAMAPI EloamThumbnail_Add(HELOAMTHUMBNAIL thumb, BSTR imagePath);
EXTERN_C BOOL ELOAMAPI EloamThumbnail_Insert(HELOAMTHUMBNAIL thumb, BSTR imagePath, LONG pos);
EXTERN_C BOOL ELOAMAPI EloamThumbnail_Remove(HELOAMTHUMBNAIL thumb, LONG idx, BOOL bDel);
EXTERN_C BOOL ELOAMAPI EloamThumbnail_Clear(HELOAMTHUMBNAIL thumb, BOOL bDel);
EXTERN_C LONG ELOAMAPI EloamThumbnail_GetCount(HELOAMTHUMBNAIL thumb);
EXTERN_C BSTR ELOAMAPI EloamThumbnail_GetFileName(HELOAMTHUMBNAIL thumb, LONG idx);
EXTERN_C LONG ELOAMAPI EloamThumbnail_GetSelected(HELOAMTHUMBNAIL thumb);
EXTERN_C BOOL ELOAMAPI EloamThumbnail_SetLanguage(HELOAMTHUMBNAIL thumb, LONG langId);
EXTERN_C BOOL ELOAMAPI EloamThumbnail_SetMenuItem(HELOAMTHUMBNAIL thumb, LONG menuId, LONG flag);
EXTERN_C BOOL ELOAMAPI EloamThumbnail_GetCheck(HELOAMTHUMBNAIL thumb, LONG idx);
EXTERN_C BOOL ELOAMAPI EloamThumbnail_SetCheck(HELOAMTHUMBNAIL thumb, LONG idx, BOOL bCheck);
EXTERN_C BOOL ELOAMAPI EloamThumbnail_HttpUploadCheckImage(HWND hWnd, HELOAMTHUMBNAIL thumb, BSTR serverAddress, LONG flag);
EXTERN_C BSTR ELOAMAPI EloamThumbnail_GetHttpServerInfo();
// font
EXTERN_C LONG ELOAMAPI EloamFont_AddRef(HELOAMFONT font);
EXTERN_C LONG ELOAMAPI EloamFont_Release(HELOAMFONT font);

// templ
EXTERN_C LONG ELOAMAPI EloamTempl_AddRef(HELOAMTEMPL templ);
EXTERN_C LONG ELOAMAPI EloamTempl_Release(HELOAMTEMPL templ);
EXTERN_C BSTR ELOAMAPI EloamTempl_GetName(HELOAMTEMPL templ);
EXTERN_C BSTR ELOAMAPI EloamTempl_GetId(HELOAMTEMPL templ);
EXTERN_C BOOL ELOAMAPI EloamTempl_AppendField(HELOAMTEMPL templ, BSTR fieldname, LONG type, FLOAT left, FLOAT top, FLOAT right, FLOAT bottom);
EXTERN_C LONG ELOAMAPI EloamTempl_GetFieldCount(HELOAMTEMPL templ);
EXTERN_C BOOL ELOAMAPI EloamTempl_ClearField(HELOAMTEMPL templ);
EXTERN_C BSTR ELOAMAPI EloamTempl_GetFieldName(HELOAMTEMPL templ, LONG idx);
EXTERN_C LONG ELOAMAPI EloamTempl_GetFieldType(HELOAMTEMPL templ, LONG idx);
EXTERN_C FLOAT ELOAMAPI EloamTempl_GetFieldRectLeft(HELOAMTEMPL templ, LONG idx);
EXTERN_C FLOAT ELOAMAPI EloamTempl_GetFieldRectTop(HELOAMTEMPL templ, LONG idx);
EXTERN_C FLOAT ELOAMAPI EloamTempl_GetFieldRectRight(HELOAMTEMPL templ, LONG idx);
EXTERN_C FLOAT ELOAMAPI EloamTempl_GetFieldRectBottom(HELOAMTEMPL templ, LONG idx);
EXTERN_C BSTR ELOAMAPI EloamTempl_GetFieldResult(HELOAMTEMPL templ, LONG idx);
EXTERN_C BOOL ELOAMAPI EloamTempl_SetFieldResult(HELOAMTEMPL templ, LONG idx, BSTR result);
EXTERN_C BSTR ELOAMAPI EloamTempl_GetData(HELOAMTEMPL templ, LONG flag);
EXTERN_C BOOL ELOAMAPI EloamTempl_Save(HELOAMTEMPL templ, BSTR filename, LONG flag);


#endif