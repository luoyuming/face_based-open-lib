UNAME := $(shell uname)

FACE_LIB=-L./home/lym/baidu/test-face-api/facelib -lFaceSDK
BASE_SDK = -L./home/lym/baidu/test-face-api/facelib -lBaiduFaceApi-ubuntu16
LIB3=/home/lym/baidu/test-face-api/lib3
OPENBLAS=-L$(LIB3)/openblas-linux/lib -lopenblas
OPENCV=-L$(LIB3)/opencv-linux/lib -lopencv_imgcodecs -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_video -lopencv_videoio 
CURL=-L$(LIB3)/curl-linux/lib-ubuntu 
LIB64=-L$(LIB3)/lib64 -ljpeg -lpng12 -ltiff -ljasper -lImath -lIlmImf -lIex -lHalf -lIlmThread -lgtk-x11-2.0 -lgdk-x11-2.0 -latk-1.0 -lcairo -ldc1394 -lfontconfig -lfreetype -lgdk_pixbuf-2.0 -lgio-2.0 -lglib-2.0 -lgmodule-2.0 -lgobject-2.0 -lgstapp-0.10 -lgstbase-0.10 -lgstpbutils-0.10 -lgstreamer-0.10 -lgstriff-0.10 -lgstvideo-0.10 -lgthread-2.0 -lpango-1.0 -lpangocairo-1.0 -lpangoft2-1.0 -lv4l1 -lv4l2 -lbz2 -lX11 -lXfixes -lXext -lXrender -lXinerama -lXi -lXcomposite -lXdamage -lpixman-1 -lusb-1.0 -lselinux -lXrandr -lXcursor -lraw1394 -lgsttag-0.10 -lxcb -lv4lconvert -lgstaudio-0.10 -lXau -lgstinterfaces-0.10
FFMPEG=-L$(LIB3)/ffmpeg-linux/lib -lavformat -lavcodec -lavutil -lswscale -lswresample
JSON_LIB=-L/home/lym/baidu/test-face-api/lib3/json/lib-ubuntu16 -ljson
LIBS=$(LIB64) $(OPENBLAS) $(OPENCV) $(FACE_LIB) $(CURL)  $(FFMPEG)



INCLUDE = -I./baidu_include  ./

GCCV = -linux

CC = g++

CXXFLAGS =  -std=c++11 -shared -fPIC

#SOURCE_CPP = $(wildcard cpp/*.h cpp/*.cpp)
SOURCE_CPP = ct.cpp

OUT = ct.so 

all: main 

main:	
	$(CC) $(CXXFLAGS) -o $(OUT)  $(SOURCE_CPP) $(INCLUDE) $(LIB64) $(FFMPEG) $(OPENCV) $(OPENBLAS) $(FACE_LIB) $(JSON_LIB) $(BASE_SDK) 
clean:
	rm -f *.o $(OUT) 

