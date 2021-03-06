#==============================================================================
#  Date    : 2013/09/09 (ver 1.0)
#	     2013/10/16 (ver 2.0)
#            2013/11/02 (ver 2.5Beta)
#            2013/11/15 (ver 3.0)
#            2013/11/17 (ver 3.1)
#  Version : 3.1
#  Author  : Ryodo Tanaka
#==============================================================================

#マクロ定義
CC 		= g++
#LRF_INC 	= `c_urg-config --cflags`
#LRF_LIB 	= `c_urg-config --libs`
# OPENCV_INC	= -I/usr/local/include/opencv2 -I/usr/local/include/opencv -I/usr/local/include/
# OPENCV_LIB	= -L/usr/local/lib -lopencv_highgui -lopencv_calib3d -lopencv_contrib -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_gpu -lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_nonfree -lopencv_objdetect -lopencv_photo -lopencv_stitching -lopencv_superres -lopencv_ts -lopencv_video -lopencv_videostab
OPENCV_INC = `pkg-config --cflags opencv`
OPENCV_LIB = `pkg-config --libs opencv`
CFLAGS		= $(OPENCV_INC) $(LRF_INC)
CPPFLAGS	= $(CFLAGS)
LDLIBS		= $(OPENCV_LIB) $(LRF_LIB) -lm
.PHONY : clean

#===============================================================================
#  何かソースを更新したら...
#
#  1. VPATH にそのソースの置いてあるディレクトリの相対パスを追加する。
#     追加するときは : をつけて追加すること。
#
#  2. SUB_SRC に追加したソースの.cファイルを追加する。
#     追加するときはスペースを開けて追加すること。
#  
#  以上が、ソースを追加した時の使い方である。
#  デバッグをしたい時は DEBUG のコメントアウトをとる。
#  オプションを付けたい時は OPTION のコメントアウトをとる。
#  どちらも何か追加したければ、スペースを開けて追加すること。
#
#  オブジェクトプログラムを置いておくフォルダを
#===============================================================================
MAIN		= main

OPTION		= -Wall
DEBUG		= -g -O0 

OBJDIR 		= ./bin/

VPATH		= $(OBJDIR):./TCP_IP:./THF:./LRF_Show

MAIN_SRC	= $(MAIN).c
SUB_SRC		= client.c find.c lrf_show.c

#===============================================================================
# Do not touch from here !!
#===============================================================================
SRCS		= $(MAIN_SRC) $(SUB_SRC)  
OBJS		= $(SRCS:%.c=%.o)
HEADS		= $(SRCS:%.c=%.h) $(SUB_SRC:%.c=_%.h)
TAGS		= TAGS

all: $(OBJDIR) $(%.o) $(MAIN).o $(MAIN) $(TAGS) 

$(MAIN).o : $(MAIN).c $(MAIN).h 
	$(CC) -c $(OPTION) $(DEBUG) $< $(CFLAGS) $(LDLIBS) -o $(OBJDIR)$@

%.o : %.c %.h _%.h 
	$(CC) -c $(OPTION) $(DEBUG) $< $(CFLAGS) $(LDLIBS) -o $(OBJDIR)$@

$(MAIN) : $(OBJS) $(HEADS)
	$(CC) -o $@ $(OPTION) $(DEBUG) $(OBJS:%.o=$(OBJDIR)%.o) $(CFLAGS) $(LDLIBS)

$(OBJDIR) :
	mkdir $(OBJDIR)

$(TAGS) : $(OBJS) $(HEADS)
	etags `find ./ -name "*.[ch]"`

clean :
	rm $(OBJS:%.o=$(OBJDIR)%.o) $(MAIN) $(TAGS)
