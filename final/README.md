# Basic Face Detection Program with OpenCV

As basic as it can be. Uses Haar Cascade classifier with pre-trained weights from OpenCV repo. Final project for MIT OpenCourseWare 6.087 Practical programming in C.

## Build

* First install OpenCV 3.4.5 and then run `make`. This is written in both C and C++ because some C functions do not work (as OpenCV C API is deprecated).

* Download Haar Cascade weights "haarcascade\_frontalface\_default.xml" from [opencv-2.4.10](https://github.com/opencv/opencv/tree/2.4.10/data/haarcascades). NOTE: older versions of weights do not work.

## Usage
`./bfdp [path to image] [OPTIONS]`
Use the `-h` option to see semi-detailed help text.

