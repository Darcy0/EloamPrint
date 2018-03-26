/*
 * Copyright (C) 2014 eloam technology
 * Auther: luoliangyi
 * CreateTime:2014.11.21
 * Email:luoliangyi@eloam.com.cn
 */

#ifndef __ELR_H__
#define __ELR_H__

#include "el.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#pragma pack(push, 1)

typedef struct ELRrect
{
	ELimage img;
	const ELrect *rc;
}ELRrect;

#pragma pack(pop)

typedef void *ELRtempl;

/* barcode */

#define ELR_BARCODE_TYPE_NONE			0
#define ELR_BARCODE_TYPE_PARTIAL		1
#define ELR_BARCODE_TYPE_EAN2			2
#define ELR_BARCODE_TYPE_EAN5			5
#define ELR_BARCODE_TYPE_EAN8			8
#define ELR_BARCODE_TYPE_UPCE			9
#define ELR_BARCODE_TYPE_ISBN10			10
#define ELR_BARCODE_TYPE_UPCA			12
#define ELR_BARCODE_TYPE_EAN13			13
#define ELR_BARCODE_TYPE_ISBN13			14
#define ELR_BARCODE_TYPE_COMPOSITE		15
#define ELR_BARCODE_TYPE_I25			25
#define ELR_BARCODE_TYPE_DATABAR		34
#define ELR_BARCODE_TYPE_DATABAR_EXP	35
#define ELR_BARCODE_TYPE_CODABAR		38
#define ELR_BARCODE_TYPE_CODE39			39
#define ELR_BARCODE_TYPE_PDF417			57
#define ELR_BARCODE_TYPE_QRCODE			64
#define ELR_BARCODE_TYPE_CODE93			93
#define ELR_BARCODE_TYPE_CODE128		128

/* ocr */

#define ELR_OCR_LANG_ENGLISH			0x00000001
#define ELR_OCR_LANG_CHINESE_PRC		0x00000002
#define ELR_OCR_LANG_CHINESE_TAIWAN		0x00000004
#define ELR_OCR_LANG_THAI				0x00000008
#define ELR_OCR_LANG_GERMAN				0x00000010
#define ELR_OCR_LANG_RUSSIAN			0x00000020
#define ELR_OCR_LANG_GREEK				0x00000040
#define ELR_OCR_LANG_KOREAN				0x00000080
#define ELR_OCR_LANG_SPANISH			0x00000100
#define ELR_OCR_LANG_PORTUGUESESTANDARD 0x00000200
#define ELR_OCR_LANG_ITALIAN			0x00000400
#define ELR_OCR_LANG_TURKISH			0x00000800
#define ELR_OCR_LANG_SWEDISH			0x00001000

#define ELR_OCR_FLAG_TEXT				1
#define ELR_OCR_FLAG_BARCODE			2
#define ELR_OCR_FLAG_IMAGE				3
#define ELR_OCR_RESULT					1
#define ELR_OCR_PROGRESS				2

#define ELR_OCR_BARCODE_TYPE_UNKOWN			0x00000000
#define ELR_OCR_BARCODE_TYPE_CODE39			0x00000001
#define ELR_OCR_BARCODE_TYPE_INTERLEAVED25	0x00000002
#define ELR_OCR_BARCODE_TYPE_EAN13			0x00000004
#define ELR_OCR_BARCODE_TYPE_CODE128		0x00000008
#define ELR_OCR_BARCODE_TYPE_EAN8			0x00000010
#define ELR_OCR_BARCODE_TYPE_PDF417			0x00000020
#define ELR_OCR_BARCODE_TYPE_CODABAR		0x00000040
#define ELR_OCR_BARCODE_TYPE_UPCE			0x00000080
#define ELR_OCR_BARCODE_TYPE_INDUSTRIAL125	0x00000100
#define ELR_OCR_BARCODE_TYPE_IATA25			0x00000200
#define ELR_OCR_BARCODE_TYPE_MATRIX25		0x00000400
#define ELR_OCR_BARCODE_TYPE_CODE93			0x00000800
#define ELR_OCR_BARCODE_TYPE_POSTNET		0x00001000
#define ELR_OCR_BARCODE_TYPE_UCC128			0x00002000
#define ELR_OCR_BARCODE_TYPE_PATCH			0x00004000
#define ELR_OCR_BARCODE_TYPE_AZTEC			0x00008000
#define ELR_OCR_BARCODE_TYPE_DATAMATRIX		0x00010000
#define ELR_OCR_BARCODE_TYPE_QRCODE			0x00020000
#define ELR_OCR_BARCODE_TYPE_UPCA			0x00040000

/* quick ocr */

#define ELR_QUICK_OCR_CHS					0
#define ELR_QUICK_OCR_CHT					1
#define ELR_QUICK_OCR_EN					2

/* idcard */

#define ELR_IDCARD_DETEC					1

#define	ELR_IDCARD_NAME						1
#define	ELR_IDCARD_GENDER					2
#define	ELR_IDCARD_NATIONALITY				3
#define	ELR_IDCARD_BIRTHYEAR				4
#define	ELR_IDCARD_BIRTHMONTH				5
#define	ELR_IDCARD_BIRTHDAY					6
#define	ELR_IDCARD_ADDRESS					7
#define	ELR_IDCARD_IDNUMBER					8
#define	ELR_IDCARD_VISAOFFICE				9
#define	ELR_IDCARD_STARTYEAR				10
#define	ELR_IDCARD_STARTMONTH				11
#define	ELR_IDCARD_STARTDAY					12
#define	ELR_IDCARD_ENDYEAR					13
#define	ELR_IDCARD_ENDMONTH					14
#define	ELR_IDCARD_ENDDAY					15
#define	ELR_IDCARD_SN						16


#define ELR_IDCARD_IMAGE_PHOTO				1
#define ELR_IDCARD_IMAGE_FRONT				2
#define ELR_IDCARD_IMAGE_BACK				3
#define ELR_IDCARD_IMAGE_FRONT_BACK_V		4
#define ELR_IDCARD_IAMGE_BACK_FRONT_V		5
#define ELR_IDCARD_IMAGE_FRONT_BACK_H		6
#define ELR_IDCARD_IAMGE_BACK_FRONT_H		7

/* biokey */

#define ELR_BIOKEY_TEMPLATE1				1
#define ELR_BIOKEY_TEMPLATE2				2
#define ELR_BIOKEY_TEMPLATE3				3
#define ELR_BIOKEY_TEMPLATE_OK				4
#define ELR_BIOKEY_FEATURE1					5
#define ELR_BIOKEY_FEATURE2					6
#define ELR_BIOKEY_FEATURE3					7
#define ELR_BIOKEY_FEATURE_OK				8

#define ELR_BIOKEY_IMAGE_GOOD				9
#define ELR_BIOKEY_IMAGE_BAD				10
#define ELR_BIOKEY_IMAGE_LOW				11
#define ELR_BIOKEY_IMAGE_FAST				12
#define ELR_BIOKEY_IMAGE_SLOW				13
#define ELR_BIOKEY_IMAGE_OTHER				14


/* reader */

#define ELR_READER_M1						2
#define ELR_READER_MEMORY					3
#define ELR_READER_CPU						4
#define ELR_READER_SOCIAL					5//社保卡
#define ELR_READER_SHENZHENTONG				7//深圳通

//Contact 
#define  CONTACT							0
#define  CONTACT_LESS						1	

#define ELR_READER_SOCIA_NAME				1
#define ELR_READER_SOCIA_NUMBER				2

/* idcard dicern callback */
typedef void (CDECL *idCardCallback)(
	int ret, 
	void *param
	);
/*idcard Finger Print callback*/
typedef void (CDECL *idCardFPCallback)(
	int ret, 
	void *param
	);
/* ocr dicern callback */
typedef void (CDECL *ocrCallback)(
	int flag, 
	int ret,
	void *param
	);

/* templ dicern callback */
typedef void (CDECL *templCallback)( 
	int ret,
	void *param
	);

/* biokey get template and get feature callback */
typedef void (CDECL *biokeyCallback)(
	int ret,
	void *param
	);

typedef void (CDECL *readerCallback)(
	int type,
	int subtype,
	void *param
	);

/*MagneticCard dicern callback*/
typedef void (CDECL *MagneticCardCallback)(
	int ret, 
	void *param
	);

/*ShenZhenTong dicern callback*/
typedef void (CDECL *ShenZhenTongCardCallback)(
	int ret, 
	void *param
	);

ELAPI(int)
elrInitIdCard(
	void
	);

ELAPI(int)
elrDeinitIdCard(
	void
	);

ELAPI(int)
elrDiscernIdCard(
	idCardCallback fun, 
	void *param
	);

ELAPI(int)
elrReadIdCard(
	void
	);

ELAPI(HBITMAP)
elrGetIdCardImage(
	int flag
	);

ELAPI(int)
elrIdCardFPVerify(
);

ELAPI(int)
elrGetIdCardData(
	int type,
	char *data
	);

ELAPI(int)
elrGetIdCardFPData(
	void *result,
	int & len
	);

ELAPI(int)
elrStopIdCardDiscern(
	void
	);

ELAPI(int)
	elrDiscernIdCardFP(
	idCardFPCallback fun, 
	void *param
	);

ELAPI(int)
	elrStopIdCardFPDiscern(
	void
	);


ELAPI(int)
elrInitBarcode(
	void
	);

ELAPI(int)
elrDeinitBarcode(
	void
	);

ELAPI(int)
elrDiscernBarcode(
	ELimage image,
	const ELrect *rc EL_DEFAULT(NULL)
	);

ELAPI(int)
elrGetBarcodeCount(
	void
	);

ELAPI(int)
elrGetBarcodeType(
	int idx
	);

ELAPI(int)
elrGetBarcodeData(
	int idx,
	wchar_t *data
	);

ELAPI(int)
elrGetBarcodeLocation(
	int idx,
	ELpoint *pt
	);

ELAPI(int)
elrInitOcr(
	void
	);

ELAPI(int)
elrDeinitOcr(
	void
	);

ELAPI(int)
elrSetOcrLanguage(
	int lang
	);

ELAPI(int)
elrDiscernOcrList(
	int flag,
	ELRrect *list, 
	int count,
	ocrCallback fun,
	void *param
	);

ELAPI(int)
elrDiscernOcr(
	int flag,
	ELimage image,
	const ELrect *rc,
	ocrCallback fun,
	void *param
	);

ELAPI(int)
elrGetOcrPageCount(
	void
	);

ELAPI(int)
elrGetOcrBlockCount(
	int page
	);

ELAPI(int)
elrGetOcrBarcodeType(
	int page,
	int block
	);

ELAPI(int)
elrGetOcrPlainText(
	int page,
	wchar_t *data
	);

ELAPI(int)
elrGetOcrData(
	int page,
	int block, 
	wchar_t *data
	);

ELAPI(int)
elrGetOcrLocation(
	int page,
	int block,
	ELrect *rc
	);

ELAPI(int)
elrSaveOcr(
	const char *filePath,
	int flag EL_DEFAULT(0)
	);

ELAPI(int)
elrStopOcrDiscern(
	void
	);

ELAPI(int)
elrWaitOcrDiscern(
	void
	);

ELAPI(int)
elrInitTemplOcr(
	void
	);

ELAPI(int)
elrDeinitTemplOcr(
	void
	);

ELAPI(int)
elrSetTemplOcrLanguage(
	int lang
	);

ELAPI(int)
elrDiscernTempl(
	ELimage image,
	ELRtempl templ,
	templCallback fun,
	void *param
	);

ELAPI(ELRtempl)
elrGetTemplResult(
	void
	);

ELAPI(int)
elrStopTemplDiscern(
	void
	);

ELAPI(int)
elrWaitTemplDiscern(
	void
	);

ELAPI(int)
elrInitReader(
	void
	);

ELAPI(int)
elrDeinitReader(
	void
	);

ELAPI(int)
elrReaderStart(
	readerCallback fun, 
	void *param
	);

ELAPI(int)
elrReaderStop(
	void
	);

ELAPI(int)
elrReaderGetCpuId(
	char *id
	);

ELAPI(int)
	elrCpuGetBankCardTrack(
	char *info
	);
ELAPI(int) 
	elrCpuGetRecordNumber(
	);
ELAPI(int) 
	elrCpuGetankCardRecord(
	int index, 
	char *data
	);

ELAPI(int)
elrReaderGetCpuCreditCardNumber(
	char *number
	);

ELAPI(int)
elrReaderGetMemoryId(
	char *id
	);

ELAPI(int)
elrReaderGetM1Id(
	char *id
	);

ELAPI(int)
	elrGetSocialCardData(
	int type,
	char *result
	);

ELAPI(int)
	elrGetQuickPassData(
	int type,
	char *result
	);

ELAPI(int)
elrInitBiokey(
	void
	);

ELAPI(int)
elrDeinitBiokey(
	void
	);

ELAPI(int)
elrGetBiokeyTemplate(
	biokeyCallback fun,
	void *param
	);

ELAPI(int)
elrStopGetBiokeyTemplate(
	void
	);

ELAPI(int)
elrGetBiokeyTemplateData(
	void *mem
	);

ELAPI(int)
elrGetBiokeyFeature(
	biokeyCallback fun,
	void *param
	);

ELAPI(int)
elrStopGetBiokeyFeature(
	void
	);

ELAPI(int)
elrGetBiokeyFeatureData(
	void *mem
	);

ELAPI(ELimage)
elrGetBiokeyImage();

ELAPI(int)
elrBiokeyVerify(
	const void *mem1,
	const void *mem2
	);

ELAPI(ELRtempl)
elrCreateTempl(
	const char *templName
	);

ELAPI(ELRtempl)
elrLoadTempl(
	const char *fileName,
	int flag EL_DEFAULT(0)
	);

ELAPI(ELRtempl)
elrLoadTemplFromXML(
	const wchar_t *data,
	int flag EL_DEFAULT(0)
	);

ELAPI(int)
elrReleaseTempl(
	ELRtempl templ
	);

ELAPI(ELRtempl)
elrCloneTempl(
	ELRtempl templ
	);

ELAPI(int)
elrCopyTempl(
	ELRtempl dest,
	ELRtempl src
	);

ELAPI(int)
elrGetTemplName(
	ELRtempl templ,
	char *name
	);

ELAPI(int)
elrGetTemplId(
	ELRtempl templ,
	char *id
	);

ELAPI(int)
elrTemplAppendField(
	ELRtempl templ,
	const char *fieldName,
	int type,
	float left,
	float top, 
	float right, 
	float bottom
	);

ELAPI(int)
elrTemplClearField(
	ELRtempl templ
	);

ELAPI(int)
elrTemplGetFieldCount(
	ELRtempl templ
	);

ELAPI(int)
elrGetTemplFieldName(
	ELRtempl templ,
	int idx,
	char *name
	);

ELAPI(int)
elrGetTemplFieldType(
	ELRtempl templ,
	int idx
	);

ELAPI(int)
elrGetTemplFieldRect(
	ELRtempl templ,
	int idx,
	float *left, 
	float *top, 
	float *right, 
	float *bottom
	);

ELAPI(int)
elrSetTemplFieldResult(
	ELRtempl templ,
	int idx, 
	const wchar_t *result
	);

ELAPI(int)
elrGetTemplFieldResult(
	ELRtempl templ,
	int idx,
	wchar_t *result
	);

ELAPI(int)
elrGetTemplXML(
	ELRtempl templ,
	wchar_t *data,
	int flag EL_DEFAULT(0)
	);

ELAPI(int)
elrSaveTempl(
	ELRtempl templ,
	const char *fileName,
	int flag EL_DEFAULT(0)
	);

ELAPI(int)
elrQuickOcr(
	const char *fileName,
	const char *resultName,
	int flag
	);

ELAPI(int) 
	elrInitMagneticCard(
	void
	);

ELAPI(int) 
	elrDeinitMagneticCard(
	void
	);

ELAPI(int)
	elrMagneticCardReaderStart(
	MagneticCardCallback fun, 
	void *param
	);

ELAPI(int) 	
	elrMagneticCardReaderStop(
	void
	);

ELAPI(int) 
	elrMagneticCardGetData(
	int type,
	char *data
	);

ELAPI(int) 
	elrMagneticCardGetNumber(
	char *number
	);


ELAPI(int) 
	elrInitShenZhenTong(
	void
	);

ELAPI(int) 
	elrDeinitShenZhenTong(
	void
	);

ELAPI(int) 
	elrStartShenZhenTongCard(
	ShenZhenTongCardCallback fun, 
	void *param
	);
ELAPI(int) 
	elrStopShenZhenTongCard(
	void
	);
ELAPI(int) 
	elrGetShenZhenTongNumber(
	char *number,
	char *amount
	);
ELAPI(int) 
	elrGetShenZhenTongCardRecordNumber(
	);
ELAPI(int) 
	elrGetShenZhenTongCardRecord(
	int index, 
	char *data
	);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __ELR_H__ */