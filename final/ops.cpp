#include <opencv2/opencv.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/core.hpp>
//#include <opencv2/imgproc.hpp> // have to use cpp here b/c cvSaveImage() fails
extern "C"
{
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/imgcodecs/imgcodecs_c.h>
#include <opencv2/objdetect/objdetect_c.h>
#include <math.h>
#include <stdio.h>
#include "ops.h"
}
#define PI 3.14159265

void rotatedBBox(int width, int height, int angle, int* outWidth, int* outHeight);

void display(const CvMat* img)
{
    printf("Displaying image... Press any key to close.\n");
    cvNamedWindow("Loaded image", CV_WINDOW_AUTOSIZE);
    cvShowImage("Loaded image", img);
    cvWaitKey(0);
}

CvMat* rotate(const CvMat* img, float angle, int resize)
{
    CvPoint2D32f center = cvPoint2D32f(img->cols/2, img->rows/2);
    CvMat* rotmat = cvCreateMat(2, 3, CV_32F);
    cv2DRotationMatrix(center, angle, 1.f, rotmat);
    int dstcols = img->cols;
    int dstrows = img->rows;
    if (resize)
    {
        rotatedBBox(img->cols, img->rows, angle, &dstcols, &dstrows);
        CV_MAT_ELEM(*rotmat, float, 0, 2) += (dstcols - img->cols) / 2.f;
        CV_MAT_ELEM(*rotmat, float, 1, 2) += (dstrows - img->rows) / 2.f;
    } 
    CvMat* dst = cvCreateMat(dstrows, dstcols, img->type);
    cvWarpAffine(img, dst, rotmat, CV_INTER_LINEAR | CV_WARP_FILL_OUTLIERS, cvScalarAll(0));
    cvReleaseMat(&rotmat);
    return dst;
}

int writeImage(const CvMat* img, const char* fname)
{
    cv::imwrite(fname, cv::cvarrToMat(img));
    return 0;
}

void rotatedBBox(int width, int height, int angle, int* outWidth, int* outHeight)
{
    float a = angle * PI / 180;
    float sina = sin(a);
    sina = sina < 0 ? -sina : sina;
    float cosa = cos(a);
    cosa = cosa < 0 ? -cosa : cosa;
    float outWidthF = height * sina + width * cosa;
    float outHeightF = height * cosa + width * sina;
    *outWidth = (int) ceil(outWidthF);
    *outHeight = (int) ceil(outHeightF);
}

int cascadeFind(const CvMat* img, const char* cascadePath, CvMemStorage** storage, CvSeq** out)
{
    cvLoad(cascadePath, NULL, NULL, NULL);
    CvHaarClassifierCascade* cascade = (CvHaarClassifierCascade *) cvLoad(cascadePath, 0, 0, 0); 
    if (cascade == NULL)
    {
        fprintf(stderr, "Cannot load cascade file '%s'. Abort.\n", cascadePath);
        return -1;
    }
    printf("Creating storage...\n");
    *storage = cvCreateMemStorage(0);
    *out = cvHaarDetectObjects(img, cascade, *storage, 1.1, 2, CV_HAAR_DO_CANNY_PRUNING, cvSize(40, 40));
    cvReleaseHaarClassifierCascade(&cascade);
    return 0;
}

int findFaces(const CvMat* img, CvMemStorage** storage, CvSeq** out) 
{
    const char* cascadePath = "haarcascade_frontalface_alt.xml";
    return cascadeFind(img, cascadePath, storage, out);
}


int cropAndSave(CvMat* img, CvMemStorage** storage, const char* folderPath)
{
    CvSeq* faces = NULL;
    int errcode = findFaces(img, storage, &faces); 
    char save_file[256];
    cv::Mat imgsrc = cv::cvarrToMat(img);
    if (faces && faces->total)
    {
        if (errcode) return errcode;
        int i = 0;
        for (i = 0; i < faces->total; i++)
        {
            CvRect* r = (CvRect*) cvGetSeqElem(faces, i);
            cv::Mat cropped = imgsrc(*r);
            sprintf(save_file, "%s/face_%d.png", folderPath, i);
            cv::imwrite(save_file, cropped);
            printf("Cropped face image written to '%s'.\n", save_file);
        }
    }
    return 0;
}

