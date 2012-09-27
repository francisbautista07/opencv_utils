CXX 			=	clang++
CXXFLAGS 	= -c -Wall
EXEC			= svm-validator
SOURCES 	= main.cpp \
						cvu_hog.cpp
OBJS			=	main.o \
						cvu_hog.o
INCLUDE 	= -I/usr/local/include \
						-I/usr/local/include/opencv2
LIBSDIR		= -L/usr/local/lib
LIBS 			= -lopencv_calib3d \
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

all: $(EXEC) clean

$(EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $(EXEC) $(LIBSFLAGS) 

$(OBJS): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) $(INCLUDE)

clean:
	rm $(OBJS)