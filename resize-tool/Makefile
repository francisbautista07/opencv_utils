CXX = clang
CXXFLAGS =	-O2 -g -Wall -fmessage-length=0
SOURCES = resize-tool.cpp
OBJS =	  resize-tool.o
INCLUDE = -I/usr/local/include -I/usr/local/include/opencv2
LIBSDIR	= -L/usr/local/lib
LIBS = -lcvx -lopencv_calib3d \
			-lopencv_contrib \
			-lopencv_core \
			-lopencv_features2d \
			-lopencv_flann \
			-lopencv_highgui \
			-lopencv_imgproc \
			-lopencv_legacy \
			-lopencv_ml \
			-lopencv_objdetect \
			-lopencv_photo \
			-lopencv_stitching \
			-lopencv_ts \
			-lopencv_video \
			-lopencv_videostab \
			-lopencv_nonfree \
			-lboost_iostreams \
			-lboost_regex

LIBSFLAGS = $(LIBSDIR) $(LIBS)

TARGET =	resize-tool

all:
	$(CXX) $(CXXFLAGS) -o $(TARGET) *.cpp $(LIBSFLAGS) $(INCLUDE)

clean:
	rm -f $(OBJS) $(TARGET)
