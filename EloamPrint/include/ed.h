/*
 * Copyright (C) 2014 eloam technology
 * Auther: luoliangyi
 * CreateTime:2014.9.20
 * Email:luoliangyi@eloam.com.cn
 */

#ifndef __ED_H__
#define __ED_H__

#include "el.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef void *EDview;
typedef void *EDvideo;
typedef void *EDpdf;

#define ED_VIEW_STAT_DRAG	0x1
#define ED_VIEW_STAT_CUSTOM 0x2

#define ED_VIEW_EVENT_SCALE	0x1
#define ED_VIEW_EVENT_RATIO 0x2
#define ED_VIEW_EVENT_RECT	0x3
#define ED_VIEW_EVENT_STATE	0x4

#define ED_VIEW_FACE_RECT_CROP_DISABLE		0x0
#define ED_VIEW_FACE_RECT_CROP_NORMAL		0x1
#define ED_VIEW_FACE_RECT_CROP_ONLY_SHOW	0x2
#define ED_VIEW_FACE_RECT_CROP_2inch		0x3

#define ED_VIEW_RECTANGLE_CROP		0X1
#define ED_VIEW_RECTANGLE_DESKEW	0X2
#define ED_VIEW_RECTANGLE_FACE_CROP	0X3

#define ED_VIEW_BOX_OPEN    0X0
#define ED_VIEW_BOX_CLOSE   0X1

/* video cap */
typedef void *EDVideoCap;


typedef void (CDECL *viewEventCallback)(
	EDview view,
	int flag,
	int value,
	void *param
	);

typedef void (CDECL *videoArrivalCallback)(
	EDvideo video,
	int id,
	void *param
	);

typedef void (CDECL *videoTouchCallback)(
	EDvideo video,
	void *param
	);

typedef void (CDECL *videoAttachCallback)(
	EDvideo video, 
	int videoId,
	EDview view,
	int viewId,
	void *param
	);

typedef void (CDECL *videoMoveDetecCallback)(
	EDvideo video,
	int id,
	void *param
	);

typedef void (CDECL *videoEnumImage)(
	EDvideo video,
	ELimage image,
	int id,
	void *param
	);

typedef void (CDECL *deskewCallback)(
	EDvideo video,
	EDview view,
	ELregion *region,
	int count,
	void *param
	);

typedef void (EL_CDECL *videoRecordCallback)(
	EDvideo video,
	int type,
	int flag,
	void *param
	);

ELAPI(int)
edSetRemoteParameter(
	int flag,
	const char * ip,
	int port
	);

ELAPI(int)
edInitDevs(
	devCallback devFun EL_DEFAULT(NULL),
	void *devParam EL_DEFAULT(NULL)
	);

ELAPI(int)
edDeinitDevs(
	void
	);

ELAPI(EDview)
edCreateView(
	HWND hWnd,
	const RECT *rc EL_DEFAULT(NULL)
	);

ELAPI(int)
edReleaseView(
	EDview view
	);

ELAPI(int)
edSetViewCallback(
	EDview view,
	viewEventCallback fun, 
	void *param
	);

ELAPI(int)
edSetViewUserData(
	EDview view,
	void *userData
	);

ELAPI(void *)
edGetViewUserData(
	EDview view
	);

ELAPI(void)
edSelectImage(
	EDview view,
	HBITMAP hBmp
	);

ELAPI(HBITMAP)
edGetSelectedImage(
	EDview view
	);

ELAPI(void)
edSelectVideo(
	EDview view,
	EDvideo video,
	videoAttachCallback fun EL_DEFAULT(NULL),
	void *param EL_DEFAULT(NULL)
	);

ELAPI(EDvideo)
edGetSelectedVideo(
	EDview view
	);

ELAPI(void)
edSelectNull(
	EDview view
	);

ELAPI(void)
edSetColor(
	EDview view,
	ELcolor clr EL_DEFAULT(EL_RGB(160, 160, 210))
	);

ELAPI(void)
edSetRectangleFormat(
	EDview view,
	int flag,
	int lineSize,
	ELcolor lineColor,
	int lineStyle,
	int pointSize,
	ELcolor pointColor
	);

ELAPI(void)
edSetText(
	EDview view,
	const char *text,
	ELcolor clr EL_DEFAULT(EL_RGB(250, 250, 250))
	);

ELAPI(int)
edSetZoomIn(
	EDview view
	);

ELAPI(int)
edSetZoomOut(
	EDview view
	);

ELAPI(int)
	edCarpete(
	EDview view
	);

ELAPI(int)
edSetOriginal(
	EDview view
	);

ELAPI(int)
edSetAdapt(
	EDview view
	);
ELAPI(void)
	edEnableFullScreen(
	EDview view,
	BOOL bFull
	);
ELAPI(int)
edSetFullScreen(
	EDview view,
	BOOL bFull
	);

ELAPI(int)
edGetState(
	EDview view
	);

ELAPI(int)
edSetState(
	EDview view,
	int stat
	);

ELAPI(int)
edGetSelectRect(
	EDview view,
	LPRECT rc
	);

ELAPI(int)
	edGetSelectRect2(
	EDview view,
	LPRECT rc,
	LPRECT rcImage
	);

ELAPI(int)
edSetSelectRect(
	EDview view,
	const RECT *rc
	);

ELAPI(int)
edPlayCaptureEffect(
	EDview view
	);

ELAPI(int)
edSetScale(
	EDview view,
	int scale
	);

ELAPI(int)
edSetRatio(
	EDview view,
	int ratio
	);

ELAPI(void)
edSetMessage(
	EDview view,
	const char *msg
	);

ELAPI(int)
edDrawCustomRect(
	EDview view,
	int flag,
	ELrect *rect,
	int cWidth,
	ELcolor clr
	);

ELAPI(int)
edPlayCaptureVoice(
	void
	);

ELAPI(EDvideo)
edCreateVideo(
	ELdevice video,
	int resolution,
	int subtype,
	videoArrivalCallback funArrival EL_DEFAULT(NULL),
	void *paramArrival EL_DEFAULT(NULL),
	videoTouchCallback funTouch EL_DEFAULT(NULL),
	void *paramTouch EL_DEFAULT(NULL)
	);

ELAPI(int)
edReleaseVideo(
	EDvideo video
	);

ELAPI(int)
edSetVideoUserData(
	EDvideo video,
	void *userData
	);

ELAPI(void *)
edGetVideoUserData(
	EDvideo video
	);

ELAPI(ELimage)
edCreateImage(
	EDvideo video,
	EDview view EL_DEFAULT(NULL),
	int scanSize EL_DEFAULT(EL_SCAN_SIZE_A4)
	);

ELAPI(int)
edSetCropState(
	EDvideo video,
	int state
	);

ELAPI(int)
edEnumImage(
	EDvideo video,
	EDview view,
	videoEnumImage fun,
	void *param,
	int scanSize EL_DEFAULT(EL_SCAN_SIZE_A4)
	);

ELAPI(ELdevice)
edGetDevice(
	EDvideo video
	);

ELAPI(int)
edGetResolution(
	EDvideo video
	);

ELAPI(int)
edGetSubtype(
	EDvideo video
	);

ELAPI(int)
edGetVideoWidth(
	EDvideo video
	);

ELAPI(int)
edGetVideoHeight(
	EDvideo video
	);

ELAPI(int)
edRotateLeft(
	EDvideo video
	);

ELAPI(int)
edRotateRight(
	EDvideo video
	);

ELAPI(int)
edRotate180(
	EDvideo video
	);

ELAPI(int)
edFlip(
	EDvideo video
	);

ELAPI(int)
edMirror(
	EDvideo video
	);

ELAPI(int)
edFlipAndMirror(
	EDvideo video
	);

ELAPI(int)
edEnableGray(
	EDvideo video
	);

ELAPI(int)
edDisableGray(
	EDvideo video
	);

ELAPI(int)
edEnableAdaptiveThreshold(
	EDvideo video,
	int flag EL_DEFAULT(0)
	);

ELAPI(int)
edDisableAdaptiveThreshold(
	EDvideo video
	);

ELAPI(int)
edEnableThreshold(
	EDvideo video,
	int threshold
	);

ELAPI(int)
edDisableThreshold(
	EDvideo video
	);

ELAPI(int)
edEnableAddText(
	EDvideo video,
	const char *text, 
	const ELpoint *pt, 
	HFONT hFont, 
	COLORREF clr EL_DEFAULT(0), 
	int weight EL_DEFAULT(0)
	);

ELAPI(int)
edEnableAddTextEx(
	EDvideo video,
	const char *text,
	int pos,
	COLORREF clr EL_DEFAULT(0), 
	int weight EL_DEFAULT(0)
	);

ELAPI(int)
edDisableAddText(
	EDvideo video
	);

ELAPI(int)
edEnableDeskew(
	EDvideo video,
	int flag EL_DEFAULT(EL_DESKEW_CLEAN),
	deskewCallback fun EL_DEFAULT(NULL),
	void *param EL_DEFAULT(NULL)
	);

ELAPI(int)
edEnableDeskewEx(
	EDvideo video,
	int flag EL_DEFAULT(EL_DESKEW_CLEAN),
	deskewCallback fun EL_DEFAULT(NULL),
	void *param EL_DEFAULT(NULL)
	);

ELAPI(int)
edDisableDeskew(
	EDvideo video
	);

ELAPI(int)
edEnableDelBkColor(
	EDvideo video
	);

ELAPI(int)
edDisableDelBkColor(
	EDvideo video
	);

ELAPI(int)
edEnableReverse(
	EDvideo video
	);

ELAPI(int)
edDisableReverse(
	EDvideo video
	);

ELAPI(int)
edEnableSmooth(
	EDvideo video,
	int flag
	);

ELAPI(int)
edDisableSmooth(
	EDvideo video
	);

ELAPI(int)
	edEnableHighLightRemove(
	EDvideo video,
	int flag
	);

ELAPI(int)
	edDisableHighLightRemove(
	EDvideo video
	);

ELAPI(int)
edEnableMoveDetec(
	EDvideo video,
	videoMoveDetecCallback fun,
	void *param
	);

ELAPI(int)
edDisableMoveDetec(
	EDvideo video
	);

ELAPI(int)
edEnableDate(
	EDvideo video,
	const ELpoint *pt, 
	HFONT hFont, 
	COLORREF clr EL_DEFAULT(0), 
	int weight EL_DEFAULT(0),
	int flag EL_DEFAULT(0)
	);

ELAPI(int)
edEnableDateEx(
	EDvideo video,
	int pos,
	COLORREF clr EL_DEFAULT(0), 
	int weight EL_DEFAULT(0),
	int flag EL_DEFAULT(0)
	);

ELAPI(int)
edDisableDate(
	EDvideo video
	);

ELAPI(EDpdf)
edCreatePdf(
	const char *fileName
	);

ELAPI(int)
edAddImage(
	EDpdf pdf,
	int fmt,
	ELimage image,
	int flag
	);

ELAPI(int)
edReleasePdf(
	EDpdf pdf
	);

ELAPI(int)
edSaveImageToPDF(
	int fmt,
	const char *fileName,
	ELimage *list,
	int count,
	int flag
	);

ELAPI(int) 
	edStartRecord(
	EDvideo video,
	const char *filePath, 
	int flag
	);

ELAPI(int) 
	edStopRecord(
	EDvideo video
	);

/********** 新版录音录像 ***********/
ELAPI(int) edVideoCapInit();
ELAPI(EDVideoCap) edCreatVideoCap();
ELAPI(int) edDestroyVideoCap(EDVideoCap hVideoCap);
ELAPI(int) edVideoCapPreCap(
	EDVideoCap hVideoCap,
	char * outputFileName,//录制文件的完整路径，包括文件名和后缀，例如"D:\weike\videoCap\test.mp4"
	int micIndex = 0,		//麦克风设备索引
	int frameRate = 10,					//录制视频帧率
	int compressMode = EL_VIDEO_CAP_COMPRESS_MEDIUM,	//视频压缩率, 压缩率越高, 文件越小但画面质量越差。三种, 0:高; 1:中; 2:低
	int videoWidth = 800, 
	int videoHeight = 600,		//输出视频大小
	BOOL bCapVideo = TRUE);
ELAPI(int) edVideoCapStart(
	EDVideoCap hVideoCap
	);
ELAPI(int)  edVideoCapStop(EDVideoCap hVideoCap);
ELAPI(int)  edVideoCapPause(EDVideoCap hVideoCap);
ELAPI(int)  edVideoCapSetWatermark(
	EDVideoCap hVideoCap,
	int bWatermark = FALSE,		//是否添加水印
	int bAddTime = FALSE,			//是否增加时间水印, 例如: 2016.05.07 12.35.5.237
	int mode = 0,				//0:文字水印, 1:图片水印
	int pos = EL_VIDEO_CAP_TOPLEFT,	//0:左上角; 1:右上角; 2:左下角; 3:右下角; 4:中间
	int alpha = 255,				//0~255, 透明度控制
	char * imgPath = "",			//水印图片路径, 格式支持: jpg/bmp/png/jpeg/gif
	char * pStrText = NULL,					//水印文字
	DWORD Color = RGB(255,0,0),				//字体颜色
	char * faceName = "宋体",		//字体名称
	LONG lHeight = 42,						//字体大小
	LONG lWeight = FW_NORMAL,					//字体轻重
	int lItalic = FALSE);
ELAPI(int) edVideoCapAddVideoSrc(EDVideoCap hVideoCap, EDvideo video, int dstX, int dstY, int dstWidth, int dstHeight, int bProportion = 0, int alpha=255);
ELAPI(int) edVideoCapRemoveAllVideoSrc(EDVideoCap hVideoCap);
ELAPI(int)  edVideoCapGetAudioDevNum();
ELAPI(int)  edVideoCapGetAudioDevName( int devIndex, char * pDevName, int bufSize);
ELAPI(int) edVideoCapGetState(EDVideoCap hVideoCap);
/********** 新版录音录像 end***********/

/************人脸接口*****************/
ELAPI(int)
	edInitFaceDetect(
	void
	);

ELAPI(int)
	edDeinitFaceDetect(
	void
	);

ELAPI(int)
	edDiscernFaceDetect(
	ELimage image1,
	ELimage image2
	);

ELAPI(int)
	edSetFaceRectCropState(
	EDvideo video,
	int flag
	);

ELAPI(int)
	edGetFaceRectCropState(
	EDvideo video
	);

ELAPI(int)
	edGetFaceRect(
	ELimage image,
	ELrect * rect
	);

ELAPI(int)
	edVideoFindRect(
	EDvideo video,
	ELrect *rect);

ELAPI(int)
	edSetDisplayRect(
	EDvideo video,
	ELrect rect,
	int enable);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*__ED_H__*/