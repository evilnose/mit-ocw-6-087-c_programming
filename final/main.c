#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <opencv2/imgcodecs/imgcodecs_c.h>
#include <opencv2/core/types_c.h>
#include "argparse.h"
#include "ops.h"

#define VALID_FLAGS "vrfohce"
// flags that require an accompanying argument
#define ARG_FLAGS "roe"
#define ORDER_FLAGS "roce"

void displayHelp();
int checkdir(const char* dirname);

int main(int argc, char* argv[])
{
    int fset = 0;
    aqueue* argq = aqueue_create();
    int result = parse_args(argq, &fset, argc, argv,
			    getfset(VALID_FLAGS, strlen(VALID_FLAGS)));

    if (result) return result;
    if (isflagset(fset, 'h'))
    {
        displayHelp();
        return 0;
    }
    int resize = isflagset(fset, 'f');

    char* out_fname = NULL;

    int orderfset = getfset(ORDER_FLAGS, strlen(ORDER_FLAGS));
    int argfset = getfset(ARG_FLAGS, strlen(ARG_FLAGS));
    CvMat* tmpimg;
    aqueue* flagq = aqueue_create(); // queue for flags
    aqueue* auxq = aqueue_create(); // queue for args for flags
    qnode* cur;
    short nfreeflags = 0;
    short nfreeargs = 1;
    CvMat* img = NULL;
    for (cur = argq->front; cur != NULL; cur = cur->next)
    {
//        printf("qnode val: %s\n", cur->val);
        if (cur->isflag) {
            if (!isflagset(orderfset, cur->val[0]))
                continue;
            enqueue(flagq, cur->val, 1); 
            if (isflagset(argfset, cur->val[0]))
                nfreeflags++;
        } else {
            if (nfreeflags > 0)
            {
                enqueue(auxq, cur->val, 0);
                nfreeflags--;
            }
            else if (nfreeargs == 1)
            {
                img = cvLoadImageM(cur->val, CV_LOAD_IMAGE_UNCHANGED);
                if (img == NULL)
                {
                    fprintf(stderr, "Failed to load image '%s'. Abort.\n", cur->val);
                    return -1;
                }
                printf("Loaded image with width %d and height %d\n", img->cols, img->rows);
                nfreeargs--;
            }
            else
            {
                fprintf(stderr, "Too many arguments. Abort.\n");
                return -1;
            }
        }
    }
    if (nfreeflags != 0)
    {
        fprintf(stderr, "Not enough arguments for flags. Abort.\n");
        return -1;
    }
    if (nfreeargs != 0)
    {
        fprintf(stderr, "Image file path not given. Abort.\n");
        return -1;
    }
    qnode* curflag;
    qnode* curaux;
    char* extract_folder;
    for (curflag = flagq->front, curaux = auxq->front; curflag != NULL; curflag = curflag->next)
    {
        switch (curflag->val[0])
        {
            case 'r':
                // rotation
                tmpimg = rotate(img, atof(curaux->val), resize);
                cvReleaseData(img);
                img = tmpimg;
                break;
            case 'o':
                out_fname = curaux->val;
                break;
            case 'c': ;
                CvSeq* faces = NULL;
                CvMemStorage* storage = NULL;
                int errcode = findFaces(img, &storage, &faces); 
                if (errcode) return errcode;
                if (!faces || faces->total == 0)
                {
                    printf("No faces found.\n");
                    break;
                }
                else
                {
                    printf("%d faces found.\n", faces->total);
                }
                int i;
                CvPoint p1, p2;
                for (i = 0; i < faces->total; i++)
                {
                    CvRect* r = (CvRect*) cvGetSeqElem(faces, i);
                    p1.x = r->x;
                    p1.y = r->y;
                    p2.x = r->x + r->width;
                    p2.y = r->y + r->height;
                    CvScalar color;
                    color.val[0] = 0;
                    color.val[1] = 0;
                    color.val[2] = 255;
                    cvRectangle(img, p1, p2, color, 3, 8, 0);
                }
                cvReleaseMemStorage(&storage);
                break;
            case 'e':
                extract_folder = curaux->val;
                if (checkdir(extract_folder))
                {
                    fprintf(stderr, "Face output directory '%s' does not exist. Abort.\n", extract_folder);
                    return -1;
                }
                break;
        }
        if (isflagset(argfset, curflag->val[0]))
            curaux = curaux->next;
    } 
    if (isflagset(fset, 'v'))
    {
        display(img);
    }

    if (isflagset(fset, 'e'))
    {
        CvMemStorage* store = NULL;
        int code = cropAndSave(img, &store, extract_folder);
        if (code) return code;
        cvReleaseMemStorage(&store);
    }
    
    if (isflagset(fset, 'o') && out_fname)
    {
        int code = writeImage(img, out_fname);
        if (code)
            fprintf(stderr, "Failed to write to '%s':"
                    "cvSaveImage() returned non-zero status code %d\n", out_fname, code);
    }
    aqueue_delete(argq);
    cvReleaseMat(&img);
    return 0;
}

int checkdir(const char* dirname)
{
    DIR* dir = opendir(dirname);
    if (dir) return 0;
    if (ENOENT == errno) return -1;
    return 1;
}

void displayHelp()
{
    printf("Basic face detection program\n"
           "\nUsage: ./bfdp [image path] [OPTIONS]\n"
           "\nOptions:\n"
           "%-25sDetect faces and draw bounding boxes around them.\n"
           "%-25sExtract faces from the image and output cropped faces the folder.\n"
           "%-25sFit the output image dimensions to the image size (used for rotation).\n"
           "%-25sDisplay this help text.\n"
           "%-25sWrite output image.\n"
           "%-25sRotate image counter-clockwise.\n"
           "%-25sView image in a new window.\n",
           "-c",
           "-e [folder path]",
           "-f",
           "-h",
           "-o [output path]",
           "-r [angle in degrees]",
           "-v");
}
