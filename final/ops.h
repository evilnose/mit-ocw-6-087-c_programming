#ifndef OPS_H
#define OPS_H

#include <opencv2/imgproc/imgproc_c.h>

void display(const CvMat* img);
CvMat* rotate(const CvMat* img, float angle, int resize);
int writeImage(const CvMat* img, const char* out_fname);
int findFaces(const CvMat* img, CvMemStorage** storage, CvSeq** out);
int cropAndSave(CvMat* img, CvMemStorage** storage, const char* folderPath);

#endif
