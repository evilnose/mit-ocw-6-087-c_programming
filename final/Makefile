CC=gcc
CPPC=g++
PKG_INC=`pkg-config --cflags opencv`
#PKG_LIBS=`pkg-config --libs opencv`
PKG_LIBS=-L/opt/opencv-3.4.5/build/lib -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_objdetect -lopencv_imgcodecs
DEBUG_CFLAGS=-Wall -O0 -g 
PROD_CFLAGS=-Wall -O2
OUT=bfdp
OBJ=main.o argparse.o

$(OUT): $(OBJ) ops.o
	$(CPPC) -o $(OUT) $(DEBUG_CFLAGS) $(OBJ) ops.o -lm $(PKG_LIBS)

%.o : %.c
	$(CC) $(DEBUG_CFLAGS) -c -o $@ $^ $(PKG_INC)

ops.o : ops.cpp
	$(CPPC) $(DEBUG_CFLAGS) -c -o $@ $^ $(PKG_INC) -std=c++11

