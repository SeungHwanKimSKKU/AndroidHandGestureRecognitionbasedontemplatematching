//
// Created by user on 2017-05-31.
//

#ifndef OPENCVTEST_NATIVE_LIB_H
#define OPENCVTEST_NATIVE_LIB_H
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
void Init_ImageProcess(CvSize img_size);
void Init_ImageProcess_template_cam(CvSize img_size);///template matching용
void Init_ImageProcess_template_picture(CvSize img_size);///template matching용
void ImageProcess(void);
IplImage* img_Cam = 0;
IplImage* img_Process = 0;
IplImage* img_HSV = 0;
IplImage* img_Gray = 0;
IplImage* img_label = 0;
IplImage* img_Show = 0;
IplImage* img_clone = 0;
IplImage* img_clone2 = 0;
//////////////////////////code가 돌아가게 하기 위해 추가하는 변수
IplImage* img_matching_picture = 0;
/////////////////template matching
IplImage* img_matching_cam = 0;//단지 matching되는 사진 보기 위함

#endif //OPENCVTEST_NATIVE_LIB_H
