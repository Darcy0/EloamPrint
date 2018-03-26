// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 ETRONWEBCAMSDK_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// ELOAMSDK_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。




#ifndef _ETRONSDK_H
#define _ETRONSDK_H


extern "C"
{

#ifndef ELOAMSDK_API
#define ELOAMSDK_API
#endif



#define DRAIN_MESSAGE             0x00000001


#ifndef BOOL
#define BOOL int
#endif

#ifndef TRUE
#define TRUE 1
#endif


#ifndef FALSE
#define FALSE 0
#endif



typedef void (*ETRONCALLBACK)(void *);
typedef  int __stdcall  VIDEOFILTERCALLBACKFUN(unsigned char * pbuf, unsigned long width, unsigned long height,void *p);
typedef  VIDEOFILTERCALLBACKFUN  *VIDEOFILTERCALLBACK;	

typedef void (*ETRONCALLBACK1)(void );
typedef  void __stdcall  MONITORFILECALLBACK(void *p);




//record format select
enum 
{
  FORMAT_AVI =0,
  FORMAT_WMV,
};

enum
{

	FORMAT_BMP = 0,
    FORMAT_JPEG,
    FORMAT_GIF,
    FORMAT_PNG,
    FORMAT_ICO,
    FORMAT_TIF,
    FORMAT_TGA,
    FORMAT_PCX,
    FORMAT_WBMP,
    FORMAT_WMF,
    FORMAT_J2K,
    FORMAT_JBG,
    FORMAT_JP2,
    FORMAT_JPC,
    FORMAT_PGX,
    FORMAT_RAS,
	FORMAT_PNM,
	FORMAT_PDF,
};




typedef struct __DIGITAL_ZOOM_INFO 
{
  long max;
  long min;
  long step;
  long val;
  long def;
  long flags;

}DIGITAL_ZOOM_INFO,*PDIGITAL_ZOOM_INFO;

#define ZEROA 0x0000
#define ZEROB 0x0001
#define A3    0x0002
#define A4    0x0003
#define A5    0x0004
#define SPECA4 0x0005
#define SPEC_A3 0x0006


typedef struct
{
	int w;
	int h;
    int pos;
}PAPERSIZEPOS;




#define ID_BASE                           0x0001000
#define ID_NoEffect                      (ID_BASE+1)
#define ID_Emboss                        (ID_BASE+2)
#define ID_Wind                          (ID_BASE+3) 
#define ID_Shear                         (ID_BASE+4)
#define ID_Monitor                       (ID_BASE+5) 
#define ID_GlassTile                     (ID_BASE+6) 
#define ID_Invert                        (ID_BASE+7)
#define ID_Canvas                        (ID_BASE+8)
#define ID_AgedPhoto                     (ID_BASE+9)
#define ID_Spread                        (ID_BASE+10)
#define ID_Mosaic                        (ID_BASE+11) 
#define ID_Warm                          (ID_BASE+12) 
#define ID_Cold                          (ID_BASE+13) 
#define ID_Magnifier                     (ID_BASE+14)
#define ID_NoFrame                       (ID_BASE+20)
#define ID_Frame1                        (ID_BASE+21)  
#define ID_Frame2                        (ID_BASE+22) 
#define ID_Frame3                        (ID_BASE+23)
#define ID_Frame4                        (ID_BASE+24)
#define ID_DZENABLE                      (ID_BASE+30)   
#define ID_DZUP                          (ID_BASE+31)    
#define ID_DZDOWN                        (ID_BASE+32)    
#define ID_DZLEFT                        (ID_BASE+33)    
#define ID_DZRIGHT                       (ID_BASE+34)    
#define ID_DZCENTER                      (ID_BASE+35)    
#define ID_DIGITALGAIN                   (ID_BASE+36)
#define ID_Brightness                    (ID_BASE+37)
#define ID_Contrast                      (ID_BASE+38)  
#define ID_Hue                           (ID_BASE+39)  // 2011.10.25修改
#define ID_Saturation                    (ID_BASE+40) 
#define ID_Sharpness                     (ID_BASE+41) 
#define ID_Gamma                         (ID_BASE+42) 


ELOAMSDK_API BOOL                   InitSDK();

ELOAMSDK_API BOOL                   InitVideoForPreview(void *p);
ELOAMSDK_API void                   ReleaseWebCamSdk(void);
ELOAMSDK_API BOOL                   StartPreview();
ELOAMSDK_API BOOL                   StopPreview();
ELOAMSDK_API int                    GetVideoDeivcenum();
ELOAMSDK_API int                    GetAudioDeivcenum();
ELOAMSDK_API void *                 GetVideoDeivceString(int index) ;
ELOAMSDK_API void *                 GetAudioDeivceString(int index) ;
ELOAMSDK_API BOOL                   IsUnicode();
ELOAMSDK_API void                   SetVideoSize(long left,long top,long right,long bottom);
ELOAMSDK_API void                   GetVideoSize(long *pleft,long *ptop,long *pright,long *pbottom);
ELOAMSDK_API void                   SetSnapPath(void *fullpathname);
ELOAMSDK_API void                   SetRecoderPath(void *fullpathname);
ELOAMSDK_API void *                 GetSnapPath();
ELOAMSDK_API void *                 GetRecoderPath();
ELOAMSDK_API void *                 GetCurrentPicWorkdir();





ELOAMSDK_API void                   ResetFileIndex();
ELOAMSDK_API void                   SetSnapshotSize(int width,int height);
ELOAMSDK_API void                   DoSnapshot(bool b);
ELOAMSDK_API void                   SetRecord(int format);
ELOAMSDK_API void                   SetAviIndex(int index);
ELOAMSDK_API void                   StartRecord();
ELOAMSDK_API void                   StopRecord();
ELOAMSDK_API void                   GetMediaSize(int *pWidth,int *pHeigh);
ELOAMSDK_API double                 GetFrameRate(void);
ELOAMSDK_API void                   SetShotWaveEnable(int enable);
ELOAMSDK_API void                   SetDelayforSnap(int dly);
ELOAMSDK_API void                   EnableAutoReloadMode();
ELOAMSDK_API void                   DisableAutoReloadMode();
ELOAMSDK_API long                   SetDefaultEffet();
ELOAMSDK_API	long                  SetEffect(unsigned int  id);
ELOAMSDK_API	long                  GetEffect(unsigned int id,unsigned long *pVal);
ELOAMSDK_API	long                  SetEffectExByStep(long id,int incdec);
ELOAMSDK_API	long                  SetEffectEx(long id,long val);
ELOAMSDK_API	long                  GetEffectEx(long id,PDIGITAL_ZOOM_INFO  pDigitalZoomInfo);

ELOAMSDK_API  void                  SetSnapFormat(int format);
ELOAMSDK_API  void                  SetTiggerFormat(int format);
ELOAMSDK_API  void                  SetHardTigerPath(void *fullpathname);
ELOAMSDK_API  void                  EnableHardTigger(BOOL b);
ELOAMSDK_API	void                  Setsnapfilename(void *filename);
ELOAMSDK_API  void                  Setrecordfilename(void *filename);
ELOAMSDK_API  int                   GetRecoderPathSize();
ELOAMSDK_API  int                   GetSnapPathSize();
ELOAMSDK_API  int                   GetCurrentPicWorkdirSize();
ELOAMSDK_API void *                GetCurrentVideoWorkdir() ;
ELOAMSDK_API int                    GetCurrentVideoWorkdirSize() ;


ELOAMSDK_API  void                  SetErZhiHua();
ELOAMSDK_API  void                  SetHuidu();
ELOAMSDK_API  void                  SetReverseColor();
ELOAMSDK_API  void                  UndoSpec();
ELOAMSDK_API  void                  ClrSpec();
ELOAMSDK_API BOOL                   CmdIsEmpty();
ELOAMSDK_API  void                  SetErZhiHuaQuezhi(unsigned char val);
ELOAMSDK_API  void                  SetVideoWndAngle(unsigned char a);
ELOAMSDK_API  void                  SetNoDeivceHint(BOOL a);
ELOAMSDK_API  BOOL                  GetUsbConnetctState();
ELOAMSDK_API  void                  SetUsbCallBackFuction(ETRONCALLBACK usbplugin,
																 void          *usbpluginparam,
																 ETRONCALLBACK usbplugout,
																 void          *usbplugoutparam);

ELOAMSDK_API int					Get_FullSnapLen();
ELOAMSDK_API void*                  Get_FullSnapName();
ELOAMSDK_API void                   WaitForCatptureFinish();
ELOAMSDK_API void                   SetVideoFilterCallBack(VIDEOFILTERCALLBACK pfun,void *p);
ELOAMSDK_API void					SetVideoFilterCallBackPost(VIDEOFILTERCALLBACK pfun,void *p);
ELOAMSDK_API void                   SetVideoRendCallBack(VIDEOFILTERCALLBACK pfun,void *p);
ELOAMSDK_API void                   SetSnapshotCallBackpost(VIDEOFILTERCALLBACK pfun,void *p);
ELOAMSDK_API void                   SetWindowWithEditMode(BOOL a);
ELOAMSDK_API void                   *GetCurrentWorkdir();
ELOAMSDK_API int                    GetCurrentWorkdirLen();
ELOAMSDK_API void                   RevertFileFormat(void *szsrc,void *szdes);
//ELOAMSDK_API unsigned char *        GetPicturePixelData(void *szfile,long *pwidth,long *pheigth,int *bitcount,int *plen);
ELOAMSDK_API unsigned char *        GetPicturePixelData8(void *szfile,long *pwidth,long *pheigth,int *bitcount,int *plen);
ELOAMSDK_API void                   ClearTempBuffer(void);
ELOAMSDK_API BOOL                   ShowVideoOutpsize(int width,int height);
ELOAMSDK_API void                   GetVideoCapbility(int *size,int len);
ELOAMSDK_API void                   SetSelRect(int left,int top,int right,int bottom);
ELOAMSDK_API void                   SetJpgQuality(unsigned char a);
ELOAMSDK_API void                   SetBitCount(unsigned char a);
ELOAMSDK_API void                   SetGrayPic(unsigned char a);
ELOAMSDK_API void                   GetSysScreenSize(int *px,int *py);
ELOAMSDK_API void                   GetSysDesktopSize(int *px,int *py,int *pw,int *ph);
ELOAMSDK_API void                   OPendir(void *s);
ELOAMSDK_API int                    UsbCameraExist(void);
ELOAMSDK_API void                   SetPicArray(short *data,short size);
ELOAMSDK_API void                   SetSaveIndex(int index);
ELOAMSDK_API void                   SetMagnify(int magnify);
ELOAMSDK_API void                   SetMagnifyRect(int x,int y,int width,int height);
ELOAMSDK_API void                   DoSnapshotGongge(int gridnum,char *sel);
ELOAMSDK_API void                   SetFileFolderMonitor(void *filepah,MONITORFILECALLBACK *filemonitorcallback,void *p);
ELOAMSDK_API void                   StopFileFolderMonitor();
ELOAMSDK_API void                   WaitforFoldermonitEnd();
ELOAMSDK_API void                   MoveFileFun(void *filepath,void *dir);
ELOAMSDK_API void                   RevertFileFormatEx(unsigned char *pbuf,long width,long height,void *szdes);
ELOAMSDK_API int                    InitLanuageList(int language);
ELOAMSDK_API void *                 GetLanguageString(void *idstr1,int *pLen);
ELOAMSDK_API void                   ShowVideo(int a);
ELOAMSDK_API int                    GetOutSizenum();
ELOAMSDK_API void                   GetOutSizeindex(short *width,short *height,int index);
ELOAMSDK_API void                   GetVideoFormatRender(int *pWidth,int *pHeight);
ELOAMSDK_API void                   Forceend();
ELOAMSDK_API void                   SetHMirror(int hmirror);
ELOAMSDK_API void                   SetVMirror(int vmirror);
ELOAMSDK_API void                   SetFullScreen(long a);
ELOAMSDK_API void                   VideoJust(unsigned char *pbuf,int w,int h);
ELOAMSDK_API void                   AntiFlicker(long *lVal, int iOpt);
ELOAMSDK_API void                   RevertFileFormatJpegEx(unsigned char *pbuf,long width,long height,void *szdes,int jpegquantity,int IsGray);
ELOAMSDK_API int                    CheckOfficeIsInstall();
ELOAMSDK_API int                    GetEloamVStringSize(int index);  
ELOAMSDK_API void *                 GetEloamVString(int index);
ELOAMSDK_API void *                 GetOfficePath();
ELOAMSDK_API int                    GetOfficePathLen();
//ELOAMSDK_API int                    RunCommand(void *app,void *command);

//ELOAMSDK_API int                    IshighVista();
ELOAMSDK_API int                    GetEloamDeviceType(int index);
ELOAMSDK_API int                    GetEloamDeviceRotate(int index);
ELOAMSDK_API void                   SetPartVideo(int width,int height);
ELOAMSDK_API void                   SetXYPos(int x_pos,int y_pos);
ELOAMSDK_API void                   SavePictureRoate(int r);
ELOAMSDK_API int                    GetPictureRoate();
ELOAMSDK_API int                    GetOcrList();
ELOAMSDK_API HRESULT                SetExposure(LONG val,int bManual);
ELOAMSDK_API int                    GetExposure(PDIGITAL_ZOOM_INFO  pDigitalZoomInfo);
ELOAMSDK_API int                    IsAdjustAutoExposure();
ELOAMSDK_API void                   DirectSendEmail(void **addr,int addrcount,char *subject,char *Content,void **attachment,int attachmentcount);
ELOAMSDK_API void                   GetPaperSizeList(int index,int *num,PAPERSIZEPOS**videolist);
ELOAMSDK_API  int                   GetStartangle(int index);
ELOAMSDK_API void                   SetPicIndex(int d);  
ELOAMSDK_API void                   SetVideoIndex(int d) ;
ELOAMSDK_API int                    GetPicIndex();
ELOAMSDK_API int                    GetVideoIndex() ;
ELOAMSDK_API void                   SetHardTiggerMode(int a);
ELOAMSDK_API void                   ReBuildVideo();
//ELOAMSDK_API void                   *GetWordPadPath(int *len);
ELOAMSDK_API int                    GetOrgPICGray();


ELOAMSDK_API void                   OpenOfficeOCR( void* fileName );
ELOAMSDK_API void                   OpenOfficeWord( void* fileName );
ELOAMSDK_API void                   MulTiffs(void * savePath, void *path[], int num);
ELOAMSDK_API int					FindExtension123(TCHAR * name);
ELOAMSDK_API void 					ZanTing(int iTime);
ELOAMSDK_API bool                   IsSaveFile();
ELOAMSDK_API bool                   IsCurrentFolder(void *s, void *foldername);
ELOAMSDK_API bool                   IsAssistCameraOpen(void *s);
ELOAMSDK_API void					closeFileWindow(void *s);
ELOAMSDK_API bool 					isFileWindow(void *s);
ELOAMSDK_API void 					setFileWindowPos(void *s, int ix, int iy);
ELOAMSDK_API void					setParentProgram(void *s, void *sParent);
ELOAMSDK_API bool					reLayoutImage(void *filePath, int iHNum, int iVNum);
ELOAMSDK_API bool					Savegif(void *fileName,void *Savepath);
//ELOAMSDK_API bool				    DeviceIsEloam(IPropertyBag *pBag);

;
//////////////////////////////////////////////////////////////////////////////////////////////////// 2011.10.18
/*
ELOAMSDK_API void                   SetExtension( CString szExtension );
ELOAMSDK_API void					SetShellOpenCommand( CString szShellOpenCommand );
ELOAMSDK_API void					SetDocumentShellOpenCommand( CString szDocumentShellOpenCommand );
ELOAMSDK_API void					SetDocumentClassName( CString szDocumentClassName );
ELOAMSDK_API void					SetDocumentDefaultIcon( CString szDocumentDefaultIcon ); 
ELOAMSDK_API BOOL					SetRegistryValue(
													HKEY hOpenKey,
													CString szKey,
													CString szValue,
													CString szData
												     );
ELOAMSDK_API BOOL RegSetExtension(void);
ELOAMSDK_API BOOL RegSetDocumentType(void);
ELOAMSDK_API BOOL RegSetAllInfo(void);
ELOAMSDK_API void RegisterFileAndProgram();*/

}

#define        Afrikaans_AFK                       0x00000436
#define        Albanian_SQI                        0x0000041c
#define        Arabic_Algeria_ARG                  0x00001401
#define        Arabic_Bahrain_ARH                  0x00003C01
#define        Arabic_Egypt_ARE                    0x00000C01
#define        Arabic_Iraq_ARI                     0x00000801
#define        Arabic_Jordan_ARJ                   0x00002C01
#define        Arabic_Kuwait_ARK                   0x00003401
#define        Arabic_Lebanon_ARB                  0x00003001
#define        Arabic_Libya_ARL                    0x00001001
#define        Arabic_Morocco_ARM                  0x00001801
#define        Arabic_Oman_ARO                     0x00002001
#define        Arabic_Qatar_ARQ                    0x00004001
#define        Arabic_Saudi_Arabia_ARA             0x00000401
#define        Arabic_Syria_ARS                    0x00002801
#define        Arabic_Tunisia_ART                  0x00001C01
#define        Arabic_UAE_ARU                      0x00003801
#define        Arabic_Yemen_ARY                    0x00002401
#define        Armenian_HYE                        0x0000042b
#define        Assamese_ASM                        0x0000044d
#define        Azeri_Cyrillic_AZC                  0x0000082c
#define        Azeri_Latin_AZE                     0x0000042c
#define        Basque_EUQ                          0x0000042D
#define        Belarusian_BEL                      0x00000423
#define        Bengali_BEN                         0x00000445
#define        Bulgarian_BGR                       0x00000402
#define        Catalan_CAT                         0x00000403
#define        Chinese_Hong_Kong_SAR_ZHH           0x00000C04
#define        Chinese_Macau_ZHM                   0x00001404
#define        Chinese_PRC_CHS                     0x00000804
#define        Chinese_Singapore_ZHI               0x00001004
#define        Chinese_Taiwan_CHT                  0x00000404
#define        Croatian_HRV                        0x0000041a
#define        Czech_CSY                           0x00000405
#define        Danish_DAN                          0x00000406
#define        Dutch_Belgium_NLB                   0x00000813
#define        Dutch_Netherlands_NLD               0x00000413
#define        English_Australia_ENA               0x00000c09
#define        English_Belize_ENL                  0x00002809
#define        English_Canada_ENC                  0x00001009
#define        English_Caribbean_ENB               0x00002409
#define        English_Ireland_ENI                 0x00001809
#define        English_Jamaica_ENJ                 0x00002009
#define        English_New_Zealand_ENZ             0x00001409
#define        English_Philippines_ENP             0x00003409
#define        English_South_Africa_ENS            0x00001c09
#define        English_Trinidad_ENT                0x00002c09
#define        English_United_Kingdom_ENG          0x00000809
#define        English_United_States_ENU           0x00000409
#define        English_Zimbabwe_ENW                0x00003009
#define        Estonian_ETI                        0x00000425
#define        Faeroese_FOS                        0x00000438
#define        Farsi_FAR                           0x00000429
#define        Finnish_FIN                         0x0000040b
#define        French_Belgium_FRB                  0x0000080c
#define        French_Canada_FRC                   0x00000c0c
#define        French_France_FRA                   0x0000040c
#define        French_Luxembourg_FRL               0x0000140c
#define        French_Monaco_FRM                   0x0000180c
#define        French_Switzerland_FRS              0x0000100c
#define        Georgian_KAT                        0x00000437
#define        German_Austria_DEA                  0x00000c07
#define        German_Germany_DEU                  0x00000407
#define        German_Liechtenstein_DEC            0x00001407
#define        German_Luxembourg_DEL               0x00001007
#define        German_Switzerland_DES              0x00000807
#define        Greek_ELL                           0x00000408
#define        Gujarati_GUJ                        0x00000447
#define        Hebrew_HEB                          0x0000040D
#define        Hindi_HIN                           0x00000439
#define        Hungarian_HUN                       0x0000040e
#define        Icelandic_ISL                       0x0000040F
#define        Indonesian_IND                      0x00000421
#define        Italian_Italy_ITA                   0x00000410
#define        Italian_Switzerland_ITS             0x00000810
#define        Japanese_JPN                        0x00000411
#define        Kannada_KAN                         0x0000044b
#define        Kashmiri_India_KAI                  0x00000860
#define        Kazakh_KAZ                          0x0000043f
#define        Konkani_KOK                         0x00000457
#define        Korean_KOR                          0x00000412
#define        Latvian_LVI                         0x00000426
#define        Lithuanian_Classic_LTH              0x00000827
#define        Lithuanian_LTH                      0x00000427
#define        Macedonian_MKD                      0x0000042f
#define        Malay_Brunei_Darussalam_MSB         0x0000083e
#define        Malayalam_MAL                       0x0000044c
#define        Malaysian_MSL                       0x0000043e
#define        Manipuri                            0x00000458
#define        Marathi_MAR                         0x0000044e
#define        Nepali_India_NEI                    0x00000861
#define        Norwegian_Bokmal_NOR                0x00000414
#define        Norwegian_Nynorsk_NON               0x00000814
#define        Oriya_ORI                           0x00000448
#define        Polish_PLK                          0x00000415
#define        Portuguese_Brazil_PTB               0x00000416
#define        Portuguese_Portugal_PTG             0x00000816
#define        Punjabi_PAN                         0x00000446
#define        Romanian_ROM                        0x00000418
#define        Russian_RUS                         0x00000419
#define        Sanskrit_SAN                        0x0000044f
#define        Serbian_Cyrillic_SRB                0x00000c1a
#define        Serbian_Latin_SRL                   0x0000081a
#define        Sindhi_SND                          0x00000459
#define        Slovak_SKY                          0x0000041b
#define        Slovenian_SLV                       0x00000424
#define        Spanish_Argentina_ESS               0x00002c0a
#define        Spanish_Bolivia_ESB                 0x0000400a
#define        Spanish_Chile_ESL                   0x0000340a
#define        Spanish_Colombia_ESO                0x0000240a
#define        Spanish_Costa_Rica_ESC              0x0000140a
#define        Spanish_Dominican_Republic_ESD      0x00001c0a
#define        Spanish_Ecuador_ESF                 0x0000300a
#define        Spanish_El_Salvador_ESE             0x0000440a
#define        Spanish_Guatemala_ESG               0x0000100a
#define        Spanish_Honduras_ESH                0x0000480a
#define        Spanish_Mexico_ESM                  0x0000080a
#define        Spanish_Nicaragua_ESI               0x00004c0a
#define        Spanish_Panama_ESA                  0x0000180a
#define        Spanish_Paraguay_ESZ                0x00003c0a
#define        Spanish_Peru_ESR                    0x0000280a
#define        Spanish_Puerto_Rico_ESU             0x0000500a
#define        Spanish_Spain_Modern_Sort_ESN       0x00000c0a
#define        Spanish_Spain_Traditional_Sort_ESP  0x0000040a
#define        Spanish_Uruguay_ESY                 0x0000380a
#define        Spanish_Venezuela_ESV               0x0000200a
#define        Swahili_Kenya_SWK                   0x00000441
#define        Swedish_Finland_SVF                 0x0000081d
#define        Swedish_SVE                         0x0000041d
#define        Tamil_TAM                           0x00000449
#define        Tatar_Tatarstan_TAT                 0x00000444
#define        Telugu_TEL                          0x0000044a
#define        Thai_THA                            0x0000041E
#define        Turkish_TRK                         0x0000041f
#define        Ukrainian_UKR                       0x00000422
#define        Urdu_Pakistan_URD                   0x00000420
#define        Uzbek_Cyrillic_UZC                  0x00000843
#define        Uzbek_Latin_UZB                     0x00000443
#define        Vietnamese_VIT                      0x0000042a

typedef struct _LANUAGE_ID
{
	int     langid;
    wchar_t  *idstr;
}LANGUAGE_ID_STRING;

static const  LANGUAGE_ID_STRING   Language_id_string[]=
{
                    Afrikaans_AFK                          ,_T("Afrikaans_AFK"),                     
                    Albanian_SQI                           ,_T("Albanian_SQI"),                      
                    Arabic_Algeria_ARG                     ,_T("Arabic_Algeria_ARG"),                
                    Arabic_Bahrain_ARH                     ,_T("Arabic_Bahrain_ARH"),                
                    Arabic_Egypt_ARE                       ,_T("Arabic_Egypt_ARE"),                  
                    Arabic_Iraq_ARI                        ,_T("Arabic_Iraq_ARI"),                   
                    Arabic_Jordan_ARJ                      ,_T("Arabic_Jordan_ARJ"),                 
                    Arabic_Kuwait_ARK                      ,_T("Arabic_Kuwait_ARK"),                 
                    Arabic_Lebanon_ARB                     ,_T("Arabic_Lebanon_ARB"),                
                    Arabic_Libya_ARL                       ,_T("Arabic_Libya_ARL"),                  
                    Arabic_Morocco_ARM                     ,_T("Arabic_Morocco_ARM"),                
                    Arabic_Oman_ARO                        ,_T("Arabic_Oman_ARO"),                   
                    Arabic_Qatar_ARQ                       ,_T("Arabic_Qatar_ARQ"),                  
                    Arabic_Saudi_Arabia_ARA                ,_T("Arabic_Saudi_Arabia_ARA"),           
                    Arabic_Syria_ARS                       ,_T("Arabic_Syria_ARS"),                  
                    Arabic_Tunisia_ART                     ,_T("Arabic_Tunisia_ART"),                
                    Arabic_UAE_ARU                         ,_T("Arabic_UAE_ARU"),                    
                    Arabic_Yemen_ARY                       ,_T("Arabic_Yemen_ARY"),                  
                    Armenian_HYE                           ,_T("Armenian_HYE"),                      
                    Assamese_ASM                           ,_T("Assamese_ASM"),                      
                    Azeri_Cyrillic_AZC                     ,_T("Azeri_Cyrillic_AZC"),                
                    Azeri_Latin_AZE                        ,_T("Azeri_Latin_AZE"),                   
                    Basque_EUQ                             ,_T("Basque_EUQ"),                        
                    Belarusian_BEL                         ,_T("Belarusian_BEL"),                    
                    Bengali_BEN                            ,_T("Bengali_BEN"),                       
                    Bulgarian_BGR                          ,_T("Bulgarian_BGR"),                     
                    Catalan_CAT                            ,_T("Catalan_CAT"),                       
                    Chinese_Hong_Kong_SAR_ZHH              ,_T("Chinese_Hong_Kong_SAR_ZHH"),         
                    Chinese_Macau_ZHM                      ,_T("Chinese_Macau_ZHM"),                 
                    Chinese_PRC_CHS                        ,_T("Chinese_PRC_CHS"),                   
                    Chinese_Singapore_ZHI                  ,_T("Chinese_Singapore_ZHI"),             
                    Chinese_Taiwan_CHT                     ,_T("Chinese_Taiwan_CHT"),                
                    Croatian_HRV                           ,_T("Croatian_HRV"),                      
                    Czech_CSY                              ,_T("Czech_CSY"),                         
                    Danish_DAN                             ,_T("Danish_DAN"),                        
                    Dutch_Belgium_NLB                      ,_T("Dutch_Belgium_NLB"),                 
                    Dutch_Netherlands_NLD                  ,_T("Dutch_Netherlands_NLD"),             
                    English_Australia_ENA                  ,_T("English_Australia_ENA"),             
                    English_Belize_ENL                     ,_T("English_Belize_ENL"),                
                    English_Canada_ENC                     ,_T("English_Canada_ENC"),                
                    English_Caribbean_ENB                  ,_T("English_Caribbean_ENB"),             
                    English_Ireland_ENI                    ,_T("English_Ireland_ENI"),               
                    English_Jamaica_ENJ                    ,_T("English_Jamaica_ENJ"),               
                    English_New_Zealand_ENZ                ,_T("English_New_Zealand_ENZ"),           
                    English_Philippines_ENP                ,_T("English_Philippines_ENP"),           
                    English_South_Africa_ENS               ,_T("English_South_Africa_ENS"),          
                    English_Trinidad_ENT                   ,_T("English_Trinidad_ENT"),              
                    English_United_Kingdom_ENG             ,_T("English_United_Kingdom_ENG"),        
                    English_United_States_ENU              ,_T("English_United_States_ENU"),         
                    English_Zimbabwe_ENW                   ,_T("English_Zimbabwe_ENW"),              
                    Estonian_ETI                           ,_T("Estonian_ETI"),                      
                    Faeroese_FOS                           ,_T("Faeroese_FOS"),                      
                    Farsi_FAR                              ,_T("Farsi_FAR"),                         
                    Finnish_FIN                            ,_T("Finnish_FIN"),                       
                    French_Belgium_FRB                     ,_T("French_Belgium_FRB"),                
                    French_Canada_FRC                      ,_T("French_Canada_FRC"),                 
                    French_France_FRA                      ,_T("French_France_FRA"),                 
                    French_Luxembourg_FRL                  ,_T("French_Luxembourg_FRL"),             
                    French_Monaco_FRM                      ,_T("French_Monaco_FRM"),                 
                    French_Switzerland_FRS                 ,_T("French_Switzerland_FRS"),            
                    Georgian_KAT                           ,_T("Georgian_KAT"),                      
                    German_Austria_DEA                     ,_T("German_Austria_DEA"),                
                    German_Germany_DEU                     ,_T("German_Germany_DEU"),                
                    German_Liechtenstein_DEC               ,_T("German_Liechtenstein_DEC"),          
                    German_Luxembourg_DEL                  ,_T("German_Luxembourg_DEL"),             
                    German_Switzerland_DES                 ,_T("German_Switzerland_DES"),            
                    Greek_ELL                              ,_T("Greek_ELL"),                         
                    Gujarati_GUJ                           ,_T("Gujarati_GUJ"),                      
                    Hebrew_HEB                             ,_T("Hebrew_HEB"),                        
                    Hindi_HIN                              ,_T("Hindi_HIN"),                         
                    Hungarian_HUN                          ,_T("Hungarian_HUN"),                     
                    Icelandic_ISL                          ,_T("Icelandic_ISL"),                     
                    Indonesian_IND                         ,_T("Indonesian_IND"),                    
                    Italian_Italy_ITA                      ,_T("Italian_Italy_ITA"),                 
                    Italian_Switzerland_ITS                ,_T("Italian_Switzerland_ITS"),           
                    Japanese_JPN                           ,_T("Japanese_JPN"),                      
                    Kannada_KAN                            ,_T("Kannada_KAN"),                       
                    Kashmiri_India_KAI                     ,_T("Kashmiri_India_KAI"),                
                    Kazakh_KAZ                             ,_T("Kazakh_KAZ"),                        
                    Konkani_KOK                            ,_T("Konkani_KOK"),                       
                    Korean_KOR                             ,_T("Korean_KOR"),                        
                    Latvian_LVI                            ,_T("Latvian_LVI"),                       
                    Lithuanian_Classic_LTH                 ,_T("Lithuanian_Classic_LTH"),            
                    Lithuanian_LTH                         ,_T("Lithuanian_LTH"),                    
                    Macedonian_MKD                         ,_T("Macedonian_MKD"),                    
                    Malay_Brunei_Darussalam_MSB            ,_T("Malay_Brunei_Darussalam_MSB"),       
                    Malayalam_MAL                          ,_T("Malayalam_MAL"),                     
                    Malaysian_MSL                          ,_T("Malaysian_MSL"),                     
                    Manipuri                               ,_T("Manipuri"),                          
                    Marathi_MAR                            ,_T("Marathi_MAR"),                       
                    Nepali_India_NEI                       ,_T("Nepali_India_NEI"),                  
                    Norwegian_Bokmal_NOR                   ,_T("Norwegian_Bokmal_NOR"),              
                    Norwegian_Nynorsk_NON                  ,_T("Norwegian_Nynorsk_NON"),             
                    Oriya_ORI                              ,_T("Oriya_ORI"),                         
                    Polish_PLK                             ,_T("Polish_PLK"),                        
                    Portuguese_Brazil_PTB                  ,_T("Portuguese_Brazil_PTB"),             
                    Portuguese_Portugal_PTG                ,_T("Portuguese_Portugal_PTG"),           
                    Punjabi_PAN                            ,_T("Punjabi_PAN"),                       
                    Romanian_ROM                           ,_T("Romanian_ROM"),                      
                    Russian_RUS                            ,_T("Russian_RUS"),                       
                    Sanskrit_SAN                           ,_T("Sanskrit_SAN"),                      
                    Serbian_Cyrillic_SRB                   ,_T("Serbian_Cyrillic_SRB"),              
                    Serbian_Latin_SRL                      ,_T("Serbian_Latin_SRL"),                  
                    Sindhi_SND                             ,_T("Sindhi_SND"),                         
                    Slovak_SKY                             ,_T("Slovak_SKY"),                         
                    Slovenian_SLV                          ,_T("Slovenian_SLV"),                      
                    Spanish_Argentina_ESS                  ,_T("Spanish_Argentina_ESS"),              
                    Spanish_Bolivia_ESB                    ,_T("Spanish_Bolivia_ESB"),                
                    Spanish_Chile_ESL                      ,_T("Spanish_Chile_ESL"),                  
                    Spanish_Colombia_ESO                   ,_T("Spanish_Colombia_ESO"),               
                    Spanish_Costa_Rica_ESC                 ,_T("Spanish_Costa_Rica_ESC"),             
                    Spanish_Dominican_Republic_ESD         ,_T("Spanish_Dominican_Republic_ESD"),     
                    Spanish_Ecuador_ESF                    ,_T("Spanish_Ecuador_ESF"),                
                    Spanish_El_Salvador_ESE                ,_T("Spanish_El_Salvador_ESE"),            
                    Spanish_Guatemala_ESG                  ,_T("Spanish_Guatemala_ESG"),              
                    Spanish_Honduras_ESH                   ,_T("Spanish_Honduras_ESH"),               
                    Spanish_Mexico_ESM                     ,_T("Spanish_Mexico_ESM"),                 
                    Spanish_Nicaragua_ESI                  ,_T("Spanish_Nicaragua_ESI"),              
                    Spanish_Panama_ESA                     ,_T("Spanish_Panama_ESA"),                 
                    Spanish_Paraguay_ESZ                   ,_T("Spanish_Paraguay_ESZ"),               
                    Spanish_Peru_ESR                       ,_T("Spanish_Peru_ESR"),                   
                    Spanish_Puerto_Rico_ESU                ,_T("Spanish_Puerto_Rico_ESU"),            
                    Spanish_Spain_Modern_Sort_ESN          ,_T("Spanish_Spain_Modern_Sort_ESN"),      
                    Spanish_Spain_Traditional_Sort_ESP     ,_T("Spanish_Spain_Traditional_Sort_ESP"), 
                    Spanish_Uruguay_ESY                    ,_T("Spanish_Uruguay_ESY"),                
                    Spanish_Venezuela_ESV                  ,_T("Spanish_Venezuela_ESV"),              
                    Swahili_Kenya_SWK                      ,_T("Swahili_Kenya_SWK"),                  
                    Swedish_Finland_SVF                    ,_T("Swedish_Finland_SVF"),                
                    Swedish_SVE                            ,_T("Swedish_SVE"),                        
                    Tamil_TAM                              ,_T("Tamil_TAM"),                          
                    Tatar_Tatarstan_TAT                    ,_T("Tatar_Tatarstan_TAT"),                
                    Telugu_TEL                             ,_T("Telugu_TEL"),                         
                    Thai_THA                               ,_T("Thai_THA"),                           
                    Turkish_TRK                            ,_T("Turkish_TRK"),                        
                    Ukrainian_UKR                          ,_T("Ukrainian_UKR"),                      
                    Urdu_Pakistan_URD                      ,_T("Urdu_Pakistan_URD"),                  
                    Uzbek_Cyrillic_UZC                     ,_T("Uzbek_Cyrillic_UZC"),                 
                    Uzbek_Latin_UZB                        ,_T("Uzbek_Latin_UZB"),                    
                    Vietnamese_VIT                         ,_T("Vietnamese_VIT"),                     
                              
};





#define  Language_English
#define  Language_Chinese
#define  Language_French
#define  Language_Japanese
#define  Language_Spanish
#define  Language_German
#define  Language_Portuguese
#define  Language_Polish
#define  Language_Chinese_Taiwan






#endif