
#ifndef _TOOLS_H
#define _TOOLS_H


#ifdef  SDK_TOOLS
#ifdef  PDF_EXPORTS
#define PDF_API __declspec(dllexport)
#else
#define PDF_API __declspec(dllimport)
#endif
#else
#define PDF_API
#endif



PDF_API void InitPdfConvert(float pdfw,float pdfh);
PDF_API void AddJpegdata(unsigned char *pBuf, int w,int h);
PDF_API void SavePdf(void  *s );























#endif

