/*
 * Copyright (C) 2014 eloam technology
 * Auther: luoliangyi
 * CreateTime: 2014.8.5
 * Email: luoliangyi@eloam.com
 */

#ifndef __EL_H__
#define __EL_H__

#include <stddef.h>
#ifdef _WIN32
#include <windows.h>
#endif /* _WIN32 */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* export */
#ifdef _WIN32
#define EL_CDECL __cdecl
#define EL_STDCALL __stdcall
#define EL_EXPORT __declspec(dllexport)
#else
#define EL_CDECL
#define EL_STDCALL
#define EL_EXPORT
#endif

#ifndef ELAPI
#define ELAPI(rettype) EL_EXPORT rettype EL_CDECL
#endif

#ifdef __cplusplus
#define EL_DEFAULT(val) = val
#else
#define EL_DEFAULT(val)
#endif

/* custom types */

/* represent image */
typedef void *ELimage;
/* represent device,video or audio */
typedef void *ELdevice;
/* record */
typedef void *ELvideoWriter;
/* represent ftp */
typedef void *ELftp;
/* represent http */
typedef void *ELhttp;
/* image file */
typedef void *ELfile;
/* video cap */
typedef void *ELVideoCap;

#pragma pack(push, 1)

typedef struct ELsize
{
	int width;
	int height;
}ELsize;typedef struct ELpoint
{
	int x;
	int y;
}ELpoint;

typedef struct ELrect
{
	int x;
	int y;
	int width;
	int height;
}ELrect;

typedef struct ELregion
{
	ELpoint pt[4];
}ELregion;


//压缩率选项
enum elCompressMode{
	EL_VIDEO_CAP_COMPRESS_HIGH=0,
	EL_VIDEO_CAP_COMPRESS_HIGHER,
	EL_VIDEO_CAP_COMPRESS_MEDIUM,
	EL_VIDEO_CAP_COMPRESS_LOWER,
	EL_VIDEO_CAP_COMPRESS_LOW
};

//位置选项
enum elWatermarkPosMode{
	EL_VIDEO_CAP_TOPLEFT = 0,
	EL_VIDEO_CAP_TOPRIGHT,
	EL_VIDEO_CAP_BOTTOMLEFT,
	EL_VIDEO_CAP_BOTTOMRIGHT,
	EL_VIDEO_CAP_CENTER
};

//状态机
enum elVideoCapState
{
	EL_VIDEO_CAP_STATE_STOP = 0,
	EL_VIDEO_CAP_STATE_READY,
	EL_VIDEO_CAP_STATE_RUN,
	EL_VIDEO_CAP_STATE_PAUSE,
};

//录像参数配置
typedef struct ELvideoCapConf 
{
	char outputFileName[MAX_PATH];		//录制文件的完整路径，包括文件名和后缀，例如"D:\weike\videoCap\test.mp4"
	int micIndex;			//麦克风设备索引

	int frameRate;						//录制视频帧率
	elCompressMode compressMode;	//视频压缩率, 压缩率越高, 文件越小但画面质量越差。0:高; 1:较高; 2:中; 3:较低; 4:低
	int videoWidth, videoHeight;		//输出视频大小

	BOOL bCapVideo;
	HWND hWnd;				//消息接收窗口句柄, 录屏过程中若出现错误，可以向此窗口发送消息, 通知上层程序

	ELvideoCapConf()
	{
		ZeroMemory(outputFileName, sizeof(outputFileName));
		
		micIndex = -1;
		frameRate = 10;
		compressMode = EL_VIDEO_CAP_COMPRESS_MEDIUM;
		videoWidth = 800;
		videoHeight = 600;

		bCapVideo = TRUE;
		hWnd = NULL;
	}
}ELvideoCapConf;

//水印配置
typedef struct ELvideoCapWatermark
{
	BOOL bWatermark;		//是否添加水印
	BOOL bAddTime;			//是否增加时间水印, 例如: 2016.05.07 12.35.5.237
	int mode;				//0:文字水印, 1:图片水印
	elWatermarkPosMode pos;	//0:左上角; 1:右上角; 2:左下角; 3:右下角; 4:中间
	BYTE alpha;				//0~255, 透明度控制

	char imgPath[MAX_PATH];	//水印图片路径, 格式支持: jpg/bmp/png/jpeg/gif
	char *pStrText;			//水印文字
	COLORREF textColor;					//字体颜色
	char faceName[LF_FACESIZE];		//字体名称
	LONG lHeight;						//字体大小
	LONG lWeight;						//字体轻重
	BYTE lItalic;						//是否倾斜

	ELvideoCapWatermark()
	{
		bWatermark = FALSE;
		bAddTime = FALSE;
		mode = 0;
		pos = EL_VIDEO_CAP_TOPLEFT;
		alpha = 255;
		pStrText = NULL;
		ZeroMemory(imgPath, sizeof(imgPath));
		textColor = RGB(255,0,0);
		lHeight = 42;
		lWeight = FW_NORMAL;
		lItalic = FALSE;
		(GetUserDefaultLangID()==0x0804) ? strcpy_s(faceName, sizeof(faceName), "宋体")/*简体中文*/
			:strcpy_s(faceName, sizeof(faceName), "Courier New");
	}
}ELvideoCapWatermark;


//片头片尾
typedef struct ELvideoCapFilmTitleTail
{
	char imgPathTitle[MAX_PATH];
	char imgPathTail[MAX_PATH];
	int tailTimeLen, titleTimeLen;
	int tailFeatureType, titleFeatureType;

	ELvideoCapFilmTitleTail()
	{
		ZeroMemory(imgPathTail, sizeof(imgPathTail));
		ZeroMemory(imgPathTitle, sizeof(imgPathTitle));
		tailTimeLen = titleTimeLen = 0;
		tailFeatureType = titleFeatureType = 0;
	}
}ELvideoCapFilmTitleTail;

#pragma pack(pop)

typedef unsigned int ELcolor;

/* info level */
#define EL_INFO_LEVEL_FATAL		0x1
#define EL_INFO_LEVEL_ERROR		0x2
#define EL_INFO_LEVEL_WARNING	0x3
#define EL_INFO_LEVEL_INFO		0x4

/* image format */
#define EL_FORMAT_UNKNOWN		-1
#define EL_FORMAT_BMP			0
#define EL_FORMAT_ICO			1
#define EL_FORMAT_JPEG			2
#define EL_FORMAT_PNG			13
#define EL_FORMAT_TIFF			18
#define EL_FORMAT_GIF			25

/* image load and save flag */
#define EL_JPEG_QUALITYSUPERB	0x80	//! save with superb quality (100:1)
#define EL_JPEG_QUALITYGOOD		0x0100	//! save with good quality (75:1)
#define EL_JPEG_QUALITYNORMAL	0x0200	//! save with normal quality (50:1)
#define EL_JPEG_QUALITYAVERAGE	0x0400	//! save with average quality (25:1)
#define EL_JPEG_QUALITYBAD		0x0800	//! save with bad quality (10:1)
//1~100 decimal integer x, save with x:1 

#define EL_TIFF_DEFAULT			0
#define EL_TIFF_PACKBITS		0x0100  //! save using PACKBITS compression
#define EL_TIFF_DEFLATE			0x0200  //! save using DEFLATE compression (a.k.a. ZLIB compression)
#define EL_TIFF_ADOBE_DEFLATE	0x0400  //! save using ADOBE DEFLATE compression
#define EL_TIFF_NONE			0x0800  //! save without any compression
#define EL_TIFF_CCITTFAX3		0x1000  //! save using CCITT Group 3 fax encoding
#define EL_TIFF_CCITTFAX4		0x2000  //! save using CCITT Group 4 fax encoding
#define EL_TIFF_LZW				0x4000	//! save using LZW compression
#define EL_TIFF_JPEG			0x8000	//! save using JPEG compression
#define EL_TIFF_LOGLUV			0x10000	//! save using LogLuv compression

#define EL_PNG_DEFAULT					0
#define EL_PNG_Z_BEST_SPEED				0x0001	//! save using ZLib level 1 compression flag (default value is 6)
#define EL_PNG_Z_DEFAULT_COMPRESSION	0x0006	//! save using ZLib level 6 compression flag (default recommended value)
#define EL_PNG_Z_BEST_COMPRESSION		0x0009	//! save using ZLib level 9 compression flag (default value is 6)
#define EL_PNG_Z_NO_COMPRESSION			0x0100	//! save without ZLib compression
#define EL_PNG_INTERLACED				0x0200	//! save using Adam7 interlacing (use | to combine with other save flags)

/* image create flag */
#define EL_CREATE_TOP_LEFT		0x1
#define EL_CREATE_BOTTOM_LEFT	0x2

/* image channels */
#define EL_CHANNELS_GRAY		0x1
#define EL_CHANNELS_COLOR		0x3

/* image prop */
#define EL_PROP_WIDTH			0x1
#define EL_PROP_HEIGHT			0x2
#define EL_PROP_CHANNELS		0x3
#define EL_PROP_WIDTHSTEP		0x4
#define EL_PROP_XDPI			0x5
#define EL_PROP_YDPI			0x6

/* image cvt */
#define EL_CVT_COLOR2GRAY		0x6
#define	EL_CVT_GRAY2COLOR		0x8

/* image rotate */
#define EL_ROTATE_KEEP_SIZE		0x0
#define EL_ROTATE_KEEP_INFO		0x1

/* image flip */
#define EL_FLIP_HORIZONTAL		0
#define EL_FLIP_VERTICAL		1
#define EL_FLIP_BOTH			-1

#define EL_DESKEW_CLEAN			0
#define EL_DESKEW_MORE			1

/* image rectify */
#define EL_RECTIFY_NONE			0
#define EL_RECTIFY_IDCARD		1

/* color */
#define EL_RGB(r, g, b) ((ELcolor)(((unsigned char)(r) \
	| ((unsigned short)((unsigned char)(g)) << 8)) \
	| (((unsigned int)(unsigned char)(b)) << 16)))

/* dev type */
#define EL_DEV_TYPE_VIDEO			0x1
#define EL_DEV_TYPE_AUDIO			0x2

#define EL_ELOAM_TYPE_VIDEO1		0x1
#define EL_ELOAM_TYPE_VIDEO2		0x2
#define EL_ELOAM_TYPE_VIDEO3		0x3

#define EL_DEV_DBT_ARRIVAL			0x1
#define EL_DEV_DBT_REMOVE			0x2

#define EL_DEV_SUBTYPE_YUY2			0x1
#define EL_DEV_SUBTYPE_MJPG			0x2
#define EL_DEV_SUBTYPE_UYVY			0x4

#define EL_SCAN_SIZE_A1				0x01
#define EL_SCAN_SIZE_A2				0x02
#define EL_SCAN_SIZE_A3				0x04
#define EL_SCAN_SIZE_A4				0x08
#define EL_SCAN_SIZE_A5				0x10
#define EL_SCAN_SIZE_A6				0x20
#define EL_SCAN_SIZE_A7				0x40

/* dev stat */
#define EL_DEV_STAT_READY			0x0
#define EL_DEV_STAT_VIEW_PENDING	0x1
#define EL_DEV_STAT_VIEWING			0x2
#define EL_DEV_STAT_PAUSE			0x3

/* dev video procamp */
#define EL_VP_BRIGHTNESS			0x1 
#define	EL_VP_CONTRAST				0x2  
#define	EL_VP_SATURATION			0x3 
#define	EL_VP_HUE					0x4 
#define	EL_VP_SHARPNESS				0x5 
#define	EL_VP_GAMMA					0x6 
#define	EL_VP_WHITEBALANCE			0x7 
#define	EL_VP_BACKLIGHTCOMPENSATION 0x8 
#define	EL_VP_COLORENABLE			0x9 
#define	EL_VP_GAIN					0xA

/*record video*/
#define EL_RV_BEGIN					0x0
#define EL_RV_END					0x1
#define EL_RV_ERR					0x2

/* dev camera control */
#define EL_CC_EXPOSURE				0x1
#define	EL_CC_PAN					0x2
#define	EL_CC_TILT					0x3
#define	EL_CC_ROLL					0x4
#define	EL_CC_ZOOM					0x5
#define	EL_CC_IRIS					0x6
#define	EL_CC_FOCUS					0x7

/* dev webcam value */
#define EL_WV_MINIMUM				0x1
#define	EL_WV_MAXIMUM				0x2
#define	EL_WV_STEP					0x3
#define	EL_WV_DEFAULT				0x4
#define	EL_WV_VALUE					0x5
#define	EL_WV_AUTO					0x6

/* dev mode*/
#define EL_DEV_MODE_LOCAL			0x0
#define	EL_DEV_MODE_REMOTE			0x1

/* [device callback]
 * type: EL_DEV_TYPE_VIDEO or EL_DEV_TYPE_AUDIO
 * idx: index of device
 * dbt: EL_DEV_DBT_ARRIVAL or EL_DEV_DBT_REMOVE
 * param: user data
 * you can change ui in the function
 */
typedef void (EL_CDECL *devCallback)(
	int type, 
	int idx, 
	int dbt, 
	void *param
	);

/* [start preview callback]
 * dev: device
 * img: image of frame
 * id: id of frame
 * param: user data
 */
typedef void (EL_CDECL *viewCallback)(
	ELdevice dev, 
	ELimage img, 
	int id, 
	void *param
	);

/* [touch capture callback]
 * dev: device
 * param: user data
 */
typedef void (EL_CDECL *touchCallback)(
	ELdevice dev,
	void *param
	);

/* [http callback]
 * http: http object
 * type: upload or download
 * flag: progress or result
 * value: value for flag
 * param: user data
 */
typedef void (EL_CDECL *httpCallback)(
	ELhttp http,
	int type,
	int flag,
	int value,
	void *param
	);

/* [ftp callback]
 * ftp: ftp object
 * type: upload or download
 * flag: progress or result
 * value: value for flag
 * param: user data
 */


typedef void (EL_CDECL *ftpCallback)(
	ELftp ftp,
	int type,
	int flag,
	int value,
	void *param
	);

/* [record callback]
 * dev: device
 * type: record state
 * flag: 
 * param: user data
 */
typedef void (EL_CDECL *recordCallback)(
	ELdevice dev,
	int type,
	int flag,
	void *param
	);

ELAPI(int)
elWriteInfo(
	int level,
	const char *format,
	...
	);

ELAPI(int)
	elSetLogNameFormat(
	const char * prefix,
	int flag EL_DEFAULT(0)
	);

ELAPI(ELimage)
elLoadImage(
	const char *fileName,
	int flag EL_DEFAULT(0)
	);

ELAPI(ELimage)
elCreateImage(
	const ELsize *size,
	int channels EL_DEFAULT(EL_CHANNELS_COLOR),
	void *data EL_DEFAULT(NULL),
	int step EL_DEFAULT(-1),
	int flag EL_DEFAULT(0)
	);

#ifdef _WIN32
ELAPI(ELimage)
elCreateImageFromDDB(
	HBITMAP bmp,
	HDC hdc EL_DEFAULT(NULL)
	);

ELAPI(ELimage)
elCreateImageFromDIB(
	HBITMAP bmp
	);
#endif

ELAPI(ELimage)
elCloneImage(
	ELimage image
	);

ELAPI(int)
elCopyImage(
	ELimage dest,
	ELimage src
	);

ELAPI(int)
elSaveImage(
	ELimage image,
	const char *fileName,
	int flag EL_DEFAULT(0)
	);

ELAPI(int)
	elSaveList(
	const char *fileName,
	ELimage *list,
	int len,
	int flag
	);

//ELAPI(int)
//	elSaveListEx(
//	const char *fileName,
//	ELimage *list,
//	int len,
//	int flag
//	);

ELAPI(int)
	elSaveList(
	const char *fileName,
	ELimage *list,
	int len,
	int flag EL_DEFAULT(0)
	);

ELAPI(int)
elReleaseImage(
	ELimage image
	);

ELAPI(int)
elGetImageProperty(
	ELimage image,
	int flag
	);

ELAPI(int)
elSetImageProperty(
	ELimage image,
	int flag,
	int value
	);

ELAPI(void *)
elGetImageData(
	ELimage image
	);

#ifdef _WIN32
ELAPI(HBITMAP)
elCreateDDB(
	ELimage image,
	HDC hdc EL_DEFAULT(NULL)
	);

ELAPI(HBITMAP)
elCreateDIB(
	ELimage image
	);

ELAPI(int)
elDrawText(
	ELimage image,
	HFONT font,
	const ELpoint *pt,
	const char *text,
	ELcolor clr EL_DEFAULT(EL_RGB(0, 0, 0)),
	int weight EL_DEFAULT(0),
	ELimage *dest EL_DEFAULT(NULL)
	);

ELAPI(int)
elDrawTextOnHBITMAP(
	HBITMAP hBmp,
	HFONT font,
	const ELpoint *pt,
	const char *text,
	ELcolor clr EL_DEFAULT(EL_RGB(0, 0, 0)),
	int weight EL_DEFAULT(0)
	);
#endif

ELAPI(int)
elRotateImage(
	ELimage image,
	float angle,
	int flag EL_DEFAULT(EL_ROTATE_KEEP_INFO),
	ELcolor clr EL_DEFAULT(EL_RGB(0, 0, 0)),
	ELimage *dest EL_DEFAULT(NULL)
	);

ELAPI(int)
elCropImage(
	ELimage image,
	const ELrect *rect,
	ELimage *dest EL_DEFAULT(NULL)
	);

ELAPI(int)
elResizeImage(
	ELimage image,
	const ELsize *size,
	ELimage *dest EL_DEFAULT(NULL)
	);

ELAPI(int)
elBlendImage(
	ELimage image1,
	const ELrect *rect1,
	ELimage image2,
	const ELrect *rect2,
	int weight EL_DEFAULT(0),
	ELimage *dest EL_DEFAULT(NULL)
	);

ELAPI(int)
elCvtImageColor(
	ELimage image,
	int flag,
	ELimage *dest EL_DEFAULT(NULL)
	);

ELAPI(int)
elThresholdImage(
	ELimage image,
	int threshold,
	ELimage *dest EL_DEFAULT(NULL)
	);

ELAPI(int)
elAdaptiveThresholdImage(
	ELimage image,
	int flag EL_DEFAULT(0),
	ELimage *dest EL_DEFAULT(NULL)
	);

ELAPI(int)
elDeskewImage(
	ELimage image,
	int flag EL_DEFAULT(EL_DESKEW_CLEAN),
	ELregion *region EL_DEFAULT(NULL),
	ELimage *dest EL_DEFAULT(NULL)
	);

ELAPI(int)
elDelImageBkColor(
	ELimage image,
	ELimage *dest EL_DEFAULT(NULL)
	);

ELAPI(int)
elDelImageBackgrand(
	ELimage image,
	ELimage *dest EL_DEFAULT(NULL)
	);

ELAPI(int)
elImageDenoising(
	ELimage image,
	ELimage *dest EL_DEFAULT(NULL)
	);

ELAPI(int)
	elHighLightRremove(
	ELimage image,
	ELimage *dest EL_DEFAULT(NULL)
	);

ELAPI(int)
elFlipImage(
	ELimage image,
	int flag,
	ELimage *dest EL_DEFAULT(NULL)
	);

ELAPI(int)
elSmoothImage(
	ELimage image,
	int flag EL_DEFAULT(0),
	ELimage *dest EL_DEFAULT(NULL)
	);

ELAPI(int)
elCompareImage(
	ELimage image,
	ELimage image2
	);

ELAPI(int)
elReverseImage(
	ELimage image,
	ELimage *dest EL_DEFAULT(NULL)
	);

ELAPI(int)
elRectifyImage(
	ELimage image,
	int flag EL_DEFAULT(0),
	ELimage *dest EL_DEFAULT(NULL)
	);

ELAPI(int)
elMultiDeskewImage(
	ELimage image,
	int flag,
	ELregion *regionList EL_DEFAULT(NULL),
	ELimage *imagelist EL_DEFAULT(NULL)
	);

ELAPI(int)
elEmbossImage(
	ELimage image,
	ELimage *dest EL_DEFAULT(NULL)
	);

ELAPI(int)
elSharpenImage(
	ELimage image,
	ELimage *dest EL_DEFAULT(NULL)
	);

ELAPI(int)
	elBalanceImage(
	ELimage image,
	ELimage *dest EL_DEFAULT(NULL)
	);

ELAPI(int)
elFind_rect(
ELimage image,
ELrect *rect);

ELAPI(int)
	elIdCardProcess(
	ELimage image);

ELAPI(int)
	elFormProcess(
	ELimage image);

ELAPI(void *)
elCreateMemory(
	int fmt,
	ELimage image,
	int *len,
	int flag EL_DEFAULT(0)
	);

ELAPI(ELimage)
elCreateImageFromMemory(
	const void *mem,
	int len,
	int flag EL_DEFAULT(0)
	);

ELAPI(int)
elReleaseMemory(
	void *mem
	);

ELAPI(char *)
elCreateBase64(
	void *mem,
	int len
	);

ELAPI(void *)
elCreateMemoryFromBase64(
	const char *data,
	int *len
	);

ELAPI(int)
elReleaseBase64(
	char *data
	);

ELAPI(ELfile)
elOpenFile(
	const char *fileName,
	int flag EL_DEFAULT(0)
	);

ELAPI(int)
elGetImageCount(
	ELfile file
	);

ELAPI(ELimage)
elGetImage(
	ELfile file,
	int idx
	);

ELAPI(ELfile)
elCreateFile(
	const char *fileName,
	int flag EL_DEFAULT(0)
	);

ELAPI(int)
elAppendImage(
	ELfile file,
	ELimage image
	);

ELAPI(int)
elReleaseFile(
	ELfile file
	);

ELAPI(int)
elLoadList(
	const char *fileName,
	ELimage *list,
	int *len,
	int flag EL_DEFAULT(0)
	);


ELAPI(int)
elSetRemoteParameter(
	int flag,
	const char * ip,
	int port
	);

ELAPI(int)
elInitDevs(
	devCallback devFun EL_DEFAULT(NULL),
	void *devParam EL_DEFAULT(NULL)
	);

ELAPI(int)
elDeinitDevs(
	void
	);

ELAPI(int)
elGetDevCount(
	int type
	);

ELAPI(int)
elGetDisplayName(
	int type,
	int idx,
	char *name
	);

ELAPI(int)
elGetFriendlyName(
	int type,
	int idx,
	char *name
	);

ELAPI(int)
elGetEloamType(
	int type,
	int idx
	);

ELAPI(ELdevice)
elCreateDevice(
	int type,
	int idx EL_DEFAULT(0)
	);

ELAPI(int)
elReleaseDevice(
	ELdevice dev
	);

ELAPI(int)
elGetDevType(
	ELdevice dev
	);

ELAPI(int)
elGetDevIndex(
	ELdevice dev
	);

ELAPI(int)
elGetDevState(
	ELdevice dev
	);

ELAPI(int)
elGetDevFriendlyName(
	ELdevice dev,
	char *name
	);

ELAPI(int)
elGetDevDisplayName(
	ELdevice dev,
	char *name
	);

ELAPI(int)
elGetDevEloamType(
	ELdevice dev
	);

ELAPI(int)
elGetDevScanSize(
	ELdevice dev
	);

ELAPI(int)
	elGetSDKtype(
	ELdevice dev
	);

ELAPI(int)
elGetSubtype(
	ELdevice dev
	);

ELAPI(int)
	elGetFrameRate(
	ELdevice dev
	);

ELAPI(int)
elGetResolutionCount(
	ELdevice dev
	);

ELAPI(int)
elGetResolutionWidth(
	ELdevice dev,
	int idx
	);

ELAPI(int)
elGetResolutionHeight(
	ELdevice dev,
	int idx
	);


ELAPI(int)
	elGetResolutionCountEx(
	ELdevice dev,
	int subType
	);

ELAPI(int)
	elGetResolutionWidthEx(
	ELdevice dev,
	int subType,
	int idx
	);

ELAPI(int)
	elGetResolutionHeightEx(
	ELdevice dev,
	int subType,
	int idx);

ELAPI(int)
	elGetCurrentResolutionWidth(
	ELdevice dev
	);

ELAPI(int)
	elGetCurrentResolutionHeight(
	ELdevice dev
	);

ELAPI(int)
elStartPreview(
	ELdevice dev,
	viewCallback viewFun,
	void *viewParam,
	touchCallback touchFun,
	void *touchParam,
	int resolution EL_DEFAULT(0),
	int subtype EL_DEFAULT(EL_DEV_SUBTYPE_YUY2)
	);

ELAPI(int)
elStopPreview(
	ELdevice dev
	);

ELAPI(int)
	elImageProcess(
	ELdevice dev,
	char * pData, 
	int width,
	int height
	);

ELAPI(int)
elPausePreview(
	ELdevice dev
	);

ELAPI(int)
elResumePreview(
	ELdevice dev
	);

ELAPI(ELimage)
	elGetImageFromDev(
	ELdevice dev
	);

#ifdef _WIN32
ELAPI(int)
elShowProperty(
	ELdevice dev,
	HWND hOwner
	);
#endif

#ifdef _WIN32
ELAPI(int)
	elShowVideoCapturePin(
	ELdevice dev,
	HWND hOwner
	);
#endif

ELAPI(int)
elGetVideoProcAmp(
	ELdevice dev,
	int prop, 
	int value
	);

ELAPI(int)
elSetVideoProcAmp(
	ELdevice dev,
	int prop, 
	int value, 
	int isAuto
	);

ELAPI(int)
elGetCameraControl(
	ELdevice dev,
	int prop, 
	int value
	);

ELAPI(int)
elSetCameraControl(
	ELdevice dev,
	int prop, 
	int value, 
	int isAuto
	);

ELAPI(int)
elGetSonixSerialNumber(
	ELdevice dev,
	char *number
	);

ELAPI(int) 
	elStartRecord(
	ELdevice dev,
	const char *filePath,
	int flag
	);

ELAPI(int) 
	elStopRecord(
	ELdevice dev
	);

ELAPI(ELvideoWriter)
elCreateVideoWriter(
	const char *fileName,
	double fps, 
	const ELsize *size,
	int flag EL_DEFAULT(0)
	);

ELAPI(int)
elWriteVideoFrame(
	ELvideoWriter writer,
	ELimage image
	);

ELAPI(int)
elReleaseVideoWriter(
	ELvideoWriter writer
	);

ELAPI(ELftp)
elCreateFtp(
	const char *ftpPath
	);

ELAPI(int)
elReleaseFtp(
	ELftp ftp
	);

ELAPI(int)
elUploadFtp(
	ELftp ftp,
	const char *localPath,
	const char *remotePath,
	ftpCallback fun,
	void *param
	);

ELAPI(int)
elStopFtpUpload(
	ELftp ftp
	);

ELAPI(int)
elWaitFtpUpload(
	ELftp ftp
	);

ELAPI(int)
elFtpCreateDir(
	ELftp ftp,
	const char *dir
	);

ELAPI(int)
elFtpRemoveDir(
	ELftp ftp,
	const char *dir
	);

ELAPI(ELhttp)
elCreateHttp(
	const char *httpPath
	);

ELAPI(int)
elReleaseHttp(
	ELhttp http
	);

ELAPI(int)
elUploadFileHttp(
	ELhttp http,
	const char *localPath,
	const char *headers, 
	const char *predata,
	const char *taildata,
	httpCallback fun,
	void *param
	);

ELAPI(int)
elUploadMemoryHttp(
	ELhttp http,
	const void *mem,
	int len,
	const char *headers, 
	const char *predata,
	const char *taildata,
	httpCallback fun,
	void *param
	);

ELAPI(int)
elUploadImageFileHttp(
	ELhttp http,
	const char *fileName, 
	const char *remoteName, 
	httpCallback fun,
	void *param
	);

ELAPI(int)
elUploadImageHttp(
	ELhttp http,
	const void *mem, 
	int len, 
	const char *remoteName, 
	httpCallback fun, 
	void *param
	);

ELAPI(int)
elStopHttpUpload(
	ELhttp http
	);

ELAPI(int)
elWaitHttpUpload(
	ELhttp http
	);

ELAPI(int)
elGetHttpServerInfo(
	ELhttp http,
	char *info
	);

#ifdef _WIN32

ELAPI(int)
	elGetPrinterCount(
	);

ELAPI(int)
	elGetPrinterName(
	int printerId,
	char * printerName
	);

ELAPI(int)
elPrintHBITMAP(
	HBITMAP hBmp,
	float x,
	float y,
	float width,
	float height,
	const char *printer EL_DEFAULT(NULL)
	);

ELAPI(int)
elAdaptivePrintHBITMAP(
	HBITMAP hBmp,
	float width,
	float height,
	const char *printer EL_DEFAULT(NULL)
	);
#endif

ELAPI(int)
elEncrypt(
	char *data,
	int len,
	const char *keyStr
	);

ELAPI(int)
elDecrypt(
	char *data,
	int len,
	const char *keyStr
	);

ELAPI(int)
elGetMD5(
	char *result,
	const char *data,
	int len
	);

ELAPI(ELimage)
elGetRemoteImage(
	void
	);

ELAPI(int)
	elGetDeviceMode(
	void
	);


/********** 新版录音录像 ***********/
/*
* 录像库初始化
*/
ELAPI(int) elVideoCapInit(/*void*/);

/*
* 创建录像实例
*/
ELAPI(ELVideoCap) elCreatVideoCap(/*void*/);

/*
* 销毁录像实例
* hVideoCap: in, 录像实例句柄
*/
ELAPI(int) elDestroyVideoCap(ELVideoCap hVideoCap);

/*
* 录像参数配置
* hVideoCap: in, 录像实例句柄
* pCapConf: in, 参数配置
*/
ELAPI(int) elVideoCapPreCap(ELVideoCap hVideoCap, ELvideoCapConf *pCapConf);

/*
* 启动录像
* hVideoCap: in, 录像实例句柄
*/
ELAPI(int) elVideoCapStart(ELVideoCap hVideoCap);

/*
* 停止录像
* hVideoCap: in, 录像实例句柄
*/
ELAPI(int) elVideoCapStop(ELVideoCap hVideoCap);

/*
* 暂停录像
* hVideoCap: in, 录像实例句柄
*/
ELAPI(int) elVideoCapPause(ELVideoCap hVideoCap);

/*
* 获取当前状态
* hVideoCap: in, 录像实例句柄
*/
ELAPI(int) elVideoCapGetState(ELVideoCap hVideoCap);

/*
* 录像水印设置, 启动录像之前设置
* hVideoCap: in, 录像实例句柄
* pWatermarkConf: in, 水印参数
*/
ELAPI(int) elVideoCapSetWatermark(ELVideoCap hVideoCap, ELvideoCapWatermark *pWatermarkConf);

/*
* 片头片尾参数设置, 启动录像之前设置
* hVideoCap: in, 录像实例句柄
* pFilmTitleTailConf: in, 参数
*/
ELAPI(int) elVideoCapSetFilmTitleTail(ELVideoCap hVideoCap, ELvideoCapFilmTitleTail *pFilmTitleTailConf);

/*
* 移除视频源
* hVideoCap: in, 录像实例句柄
* hCamera: in, 摄像头设备句柄
*/
ELAPI(int) elVideoCapRemoveVideoSrc(ELVideoCap hVideoCap, ELdevice dev);

/*
* 添加视频源
* hVideoCap: in, 录像实例句柄
* dev: in, 摄像头设备句柄
* dstX/dstY/dstWidth/dstHeight: in, 视频源位于最终视频图像的位置
* alpha: in, 透明度设置(0~255)
*/
ELAPI(int) elVideoCapAddVideoSrc(ELVideoCap hVideoCap, ELdevice dev, 
	int dstX, int dstY, int dstWidth, int dstHeight, int bProportion, int alpha=255);

/*
* 清除所有视频源
* hVideoCap: in, 录像实例句柄
*/
ELAPI(int) elVideoCapRemoveAllVideoSrc(ELVideoCap hVideoCap);

/*
* 获取系统音频设备数量
*/
ELAPI(int) elVideoCapGetAudioDevNum();

/*
* 获取系统音频设备名称
* devIndex: in, 音频设备索引
* pDevName: in, 音频设备名称
* bufSize: in, pDevName缓存大小
*/
ELAPI(int) elVideoCapGetAudioDevName(int devIndex, char *pDevName, int bufSize);

/*
* 添加WIFI视频源
* hVideoCap: in, 录像实例句柄
* dstX/dstY/dstWidth/dstHeight: in, 视频源位于最终视频图像的位置
* alpha: in, 透明度设置(0~255)
*/
ELAPI(int) elVideoCapAddVideoSrcWIFI(ELVideoCap hVideoCap, int dstX, int dstY, int dstWidth, int dstHeight, int bProportion, int alpha=255);

/*
* 写入一帧WIFI摄像头图像数据
* hVideoCap: in, 录像实例句柄
* pFrameData: in, 图像数据缓存
* frameSize: in, 帧数据大小
* lineBytes: in, 图像每行字节数
* bytesPerPixel: in, 每点字节数
*/
ELAPI(int) elVideoCapWriteVideoFrameWIFI(ELVideoCap hVideoCap, UINT8 *pFrameData, int frameSize, int lineBytes, int bytesPerPixel);
/********** 新版录音录像 end***********/

ELAPI(int)
	elInitFaceDetect(
	void
	);

ELAPI(int)
	elDeinitFaceDetect(
	void
	);

ELAPI(int)
	elGetFaceRect(
	ELimage image,
	ELrect * rect
	);

ELAPI(int)
	elDiscernFaceDetect(
	ELimage image1,
	ELimage image2
	);

ELAPI(int)
	elFaceDetectIsVaild(
	void
	);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __EL_H__ */