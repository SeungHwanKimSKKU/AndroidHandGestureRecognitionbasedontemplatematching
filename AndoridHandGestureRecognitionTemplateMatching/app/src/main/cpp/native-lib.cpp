#include <jni.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <native-lib.h>
using namespace cv;
using namespace std;
extern "C" {
JNIEXPORT void JNICALL
Java_com_example_user_opencvtest_MainActivity_ConvertRGBtoGray(JNIEnv *env, jobject instance,
                                                               jlong matAddrInput,
                                                               jlong matAddrResult) {

    // TODO
   // Mat &matInput = *(Mat *)matAddrInput;
   // Mat &matResult = *(Mat *)matAddrResult;

    IplImage *matInput=(IplImage *)matAddrInput;
    IplImage *matResult=(IplImage *)matResult;

    //cvtColor(matInput, matResult, CV_RGBA2GRAY);//matResult로 작업
int r, c=0;
	char H, S, V;
	int width = matResult->width;
	int height = matResult->height;
	int widthStep = matResult->widthStep;

	//Copy Cam Image
	//cvCopy(matInput, img_Process);
	CvSize img_size;
	double min, max, min2, max2;
	CvPoint left_top, left_top2;
	//IplImage *B = cvLoadImage("SunBlock.jpg", -1); // 썬크림 사진을 읽는다.
	//////////////////////////////////////////B와 C를 비교할거임
	//IplImage *C = cvLoadImage("Lyan.jpg", -1);//카드 사진을 읽는다.
	IplImage *D = cvCreateImage(cvSize(matResult->width - B->width + 1, matResult->height - B->height + 1), IPL_DEPTH_32F, 1); // 상관계수를 구할 이미지(D)
	IplImage *E = cvCreateImage(cvSize(matResult->width - C->width + 1, matResult->height - C->height + 1), IPL_DEPTH_32F, 1); // 상관계수를 구할 이미지(E)

	//Change Gray Image
	//cvCvtColor(img_Process, img_Gray, CV_BGR2GRAY);

	//img_clone = (IplImage*)cvClone(img_Process);
	cvSetImageROI(matResult, cvRect(200, 60, 300, 300));
	//cvSetImageROI(img_Process, cvRect(300, 200, 200, 200));
	//img_clone = (IplImage*)cvClone(img_Process);
	cvSubS(matResult, cvScalar(100, 100, 100), matResult);
	//cvResetImageROI(img_Process);
	//cvSubS(img_Process, cvScalar(600, 600, 4), img_Process);
	cvResetImageROI(matResult);
 cvMatchTemplate(matResult, B, D, CV_TM_CCOEFF_NORMED); // 상관계수를 구하여 C 에 그린다.
	cvMinMaxLoc(D, &min, &max, NULL, &left_top); // 상관계수가 최대값을 값는 위치 찾기
	//cvRectangle(img_Process, left_top, cvPoint(left_top.x + B->width, left_top.y + B->height), CV_RGB(255, 0, 0)); // 찾은 물체에 사격형을 그린다.
	///////////////////////////////////////////////////////////
	cvMatchTemplate(matResult, C, E, CV_TM_CCOEFF_NORMED); // 상관계수를 구하여 C 에 그린다.
	cvMinMaxLoc(E, &min2, &max2, NULL, &left_top2); // 상관계수가 최대값을 값는 위치 찾기
	//cvRectangle(img_Process, left_top2, cvPoint(left_top2.x + C->width, left_top2.y + C->height), CV_RGB(255, 0, 0)); // 찾은 물체에 사격형을 그린다.
	////////////////////////////////////////////출력할 이미지를 고른다.



	CvSize sz = cvGetSize(matResult);
	///cout << sz.height << ".." << sz.width;
	//IplImage* src = cvCreateImage(sz, 8, 3);
	IplImage* gray = cvCreateImage(cvSize(270, 270), 8, 1);;

		//src = cvQueryFrame(capture);//src대신 img_Process
		cvSetImageROI(matResult, cvRect(340, 100, 270, 270));

		cvCvtColor(matResult, gray, CV_BGR2GRAY);
		cvSmooth(gray, gray, CV_BLUR, (12, 12), 0);
		//cvNamedWindow("Blur", 1); cvShowImage("Blur", gray);   //blur-not-clear
		cvThreshold(gray, gray, 0, 255, (CV_THRESH_BINARY_INV + CV_THRESH_OTSU));
		//cvNamedWindow("Threshold", 1); cvShowImage("Threshold", gray);  //black-white
		CvMemStorage* storage = cvCreateMemStorage();
		CvSeq* first_contour = NULL;
		CvSeq* maxitem = NULL;
		int cn = cvFindContours(gray, storage, &first_contour, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));


		double area, max_area = 0.0;
		//int maxn=0,n=0;

		CvSeq* ptr = 0;
		if (cn>0)
		{

			for (ptr = first_contour; ptr != NULL; ptr = ptr->h_next)
			{
				area = fabs(cvContourArea(ptr, CV_WHOLE_SEQ, 0));
				if (area>max_area)
				{
					max_area = area;
					maxitem = ptr;
					//maxn=n;
				}
				//n++;
			}
			if (max_area > 1000)
			{
				CvPoint pt0;
				CvMemStorage* storage1 = cvCreateMemStorage();
				CvMemStorage* storage2 = cvCreateMemStorage(0);
				CvSeq* ptseq = cvCreateSeq(CV_SEQ_KIND_GENERIC | CV_32SC2, sizeof(CvContour), sizeof(CvPoint), storage1);
				CvSeq* hull;
				CvSeq* defects;

				for (int i = 0; i < maxitem->total; i++)
				{
					CvPoint* p = CV_GET_SEQ_ELEM(CvPoint, maxitem, i);
					pt0.x = p->x;
					pt0.y = p->y;
					cvSeqPush(ptseq, &pt0);
				}
				hull = cvConvexHull2(ptseq, 0, CV_CLOCKWISE, 0);
				int hullcount = hull->total;
				defects = cvConvexityDefects(ptseq, hull, storage2);
				//pt0 = **CV_GET_SEQ_ELEM( CvPoint*, hull, hullcount - 1 );
				//printf("** : %d :**",hullcount);
				CvConvexityDefect* defectArray;
				//	int j=0;
				for (int i = 1; i <= hullcount; i++)
				{
					CvPoint pt = **CV_GET_SEQ_ELEM(CvPoint*, hull, i);
					cvLine(matResult, pt0, pt, CV_RGB(255, 0, 0), 1, CV_AA, 0);
					pt0 = pt;
				}
				for (; defects; defects = defects->h_next)
				{
					int nomdef = defects->total; // defect amount
					//outlet_float( m_nomdef, nomdef );
					//printf(" defect no %d \n",nomdef);
					if (nomdef == 0)
						continue;
					// Alloc memory for defect set.
					//fprintf(stderr,"malloc\n");
					defectArray = (CvConvexityDefect*)malloc(sizeof(CvConvexityDefect)*nomdef);
					// Get defect set.
					//fprintf(stderr,"cvCvtSeqToArray\n");
					cvCvtSeqToArray(defects, defectArray, CV_WHOLE_SEQ);
					// Draw marks for all defects.
					int con = 0;
					for (int i = 0; i<nomdef; i++)
					{
						if (defectArray[i].depth > 40)
						{
							con = con + 1;
							//printf(" defect depth for defect %d %f \n",i,defectArray[i].depth);
							cvLine(matResult, *(defectArray[i].start), *(defectArray[i].depth_point), CV_RGB(255, 255, 0), 1, CV_AA, 0);
							cvCircle(matResult, *(defectArray[i].depth_point), 5, CV_RGB(0, 0, 255), 2, 8, 0);
							cvCircle(matResult, *(defectArray[i].start), 5, CV_RGB(0, 255, 0), 2, 8, 0);
							cvLine(matResult, *(defectArray[i].depth_point), *(defectArray[i].end), CV_RGB(0, 255, 255), 1, CV_AA, 0);
							cvDrawContours(matResult, defects, CV_RGB(0, 0, 0), CV_RGB(255, 0, 0), -1, CV_FILLED, 8);
						}
					}
					//std::cout<<con<<"\n";
					char txt[40] = "";
					if (con == 1)
					{
						if (max2 > max)
						{
							img_size.height = C->height;
							img_size.width = C->width;
							Init_ImageProcess_template_cam(img_size);
							cvCopy(C, img_matching_cam);

							char txt1[] = "this is Lyan";
							strcat(txt, txt1);

						}
						else if (max > max2)
						{
							img_size.height = B->height;
							img_size.width = B->width;
							Init_ImageProcess_template_cam(img_size);
							cvCopy(B, img_matching_cam);

							char txt1[] = "This is SunBlock";
							strcat(txt, txt1);


						}

					}
					else if (con == 2)
					{
						char txt1[] = "3 Musketeers";
						strcat(txt, txt1);
					}
					else if (con == 3)
					{
						char txt1[] = "Fanatastic 4";
						strcat(txt, txt1);
					}
					else if (con == 4)
					{
						char txt1[] = "It's 5";
						strcat(txt, txt1);
					}
					else
					{
						char txt1[] = "Jarvis is busy :P"; //Jarvis can't recognise you
						strcat(txt, txt1);
					}
					//cvNamedWindow("contour", 1); cvShowImage("contour", img_Process);
					cvResetImageROI(matResult);
					CvFont font;
					cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 1.5, 1.5, 0, 5, CV_AA);
					cvPutText(matResult, txt, cvPoint(50, 50), &font, cvScalar(0, 0, 255, 0));
					//	j++;

					// Free memory.
					free(defectArray);
				}
				cvReleaseMemStorage(&storage1);
				cvReleaseMemStorage(&storage2);
			}
		}
		cvReleaseMemStorage(&storage);




		/////////////////////////////////////////////매칭되는 이미지를 출력한다.
		//cvShowImage("T9-sample", B); // SunBlock(B) 보기
		// cvShowImage("T9-sample2", C); // 카드(C) 보기
		///////////////////////////////////
		//cvShowImage("T9-sample2", img_matching_cam); // 매칭되는 이미지 보기
		/////////////////////////////기존 입력 이미지 해제
		cvReleaseImage(&B);
		cvReleaseImage(&C);
		//////////////////////상관계수 이미지 해제
		cvReleaseImage(&D);
		cvReleaseImage(&E);
		///////////////////////출력 이미지 해제
		cvReleaseImage(&img_matching_cam);
		//cvReleaseCapture(&capture);

}
}
