#ifndef _OCR_H
#define _OCR_H







#ifdef OCR_EXPORTS
#define OCR_API __declspec(dllexport)
#else
#define OCR_API 
#endif
typedef  void __stdcall  OCRPROGRAMME(int step,void *p);

  OCR_API int InitOCREngine(void  *keyFile,void *password,void *devsn);
  OCR_API int ProcessImage(void  *imagefile,void *rtffile);
  OCR_API int RelaseOCREngine();
  OCR_API void SetOcrStepCallBack(OCRPROGRAMME *stepfun,void *p);
  OCR_API void SetLanguage(void *str);
  OCR_API void  SetFastMode(int a);
  OCR_API void  SetCurrentDir();

#endif