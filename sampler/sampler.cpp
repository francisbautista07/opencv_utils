#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

const char *display = "WINDOW";

Mat input_image;
Mat original_image;
Mat output_image;
string output_dir_path;
int width, height;
int num;

void mouse_callback(int event, int x, int y, int flags, void *param)
{ 
  original_image.copyTo(output_image);
  int x_s = x - (width / 2);
  int y_s = y - (height / 2);

  switch(event) {
    case CV_EVENT_MOUSEMOVE:
     
      rectangle(output_image, Point(x_s, y_s), Point(x_s + width, y_s + height), Scalar(255, 0, 0));
      imshow(display, output_image);

      break;
    case CV_EVENT_LBUTTONDOWN:
      break;
    case CV_EVENT_LBUTTONUP:
      Mat sub_frame = original_image(Rect(Point(x_s, y_s), Point(x_s + width, y_s + height)));

      ostringstream o;
      o << num;
      string file = o.str();
      string file_type = ".jpg";
      string new_file = output_dir_path + "/" + file + file_type;

      cout << "Saving file to " << new_file << "..." << endl;
      imwrite(new_file, sub_frame);

      num++;
      break;
  }
}

int main(int argc, char **argv)
{

  // Check input
	if(argc != 5) {
		cout << "Syntax: negative-extractor [input_image] [output_dir] [width] [height]" << endl;
    cout << "==> input_image: Image to extract regions from" << endl;
    cout << "==> output_dir: Directory to store extracted regions" << endl;
    cout << "==> width: Region width" << endl;
    cout << "==> height: Region height" << endl;

    exit(-1);
	}

  input_image = imread(argv[1]);
  input_image.copyTo(original_image);
  original_image.copyTo(output_image);

  output_dir_path = argv[2];
  width = atoi(argv[3]);
  height = atoi(argv[4]);

  num = 0;

  imshow(display, input_image);

  cvSetMouseCallback(display, mouse_callback, NULL);

  waitKey(0);

	return 0;
}
