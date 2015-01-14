CC		= gcc
CXX		= g++	
OPENCV_INC	= -I/usr/local/include/opencv2 -I/usr/local/include/opencv -I/usr/local/include/
OPENCV_LIB	= -L/usr/local/lib -lopencv_highgui -lopencv_calib3d -lopencv_contrib -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_gpu -lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_nonfree -lopencv_objdetect -lopencv_photo -lopencv_stitching -lopencv_superres -lopencv_ts -lopencv_video -lopencv_videostab

OPTION		= -Wall
#DEBUG 		= -g -O0		

CFLAGS		= $(OPTION) $(DEBUG) $(OPENCV_INC)
CPPFLAGS	= $(CFLAGS)
LDLIBS		= $(OPENCV_LIB) -lm



all:
