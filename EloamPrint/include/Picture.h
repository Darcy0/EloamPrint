#ifndef _PICUTRE_H
#define _PICUTRE_H



#ifdef PICTURE_EXPORTS
#define PICTURE_API __declspec(dllexport)
#else
#define PICTURE_API 
#endif

PICTURE_API void ModifyBackGound (unsigned char *pdatain,
							  long width,
							  long height,
							  int   mode,
							  int   erzhihua
							  );

PICTURE_API int  AdjustParam(unsigned char *buf,int width,int height);
#endif