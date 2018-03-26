#define VideoRecord_API __declspec(dllexport)

#include "cv.h"
#include "cxcore.h"
#include "highgui.h"
#include "stdlib.h"
#include <iostream>
using namespace std;

VideoRecord_API void startRecord(char* strFilename);
VideoRecord_API void stopRecord();