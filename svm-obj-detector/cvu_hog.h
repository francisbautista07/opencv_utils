#ifndef CVUHOG_H_
#define CVUHOG_H_

#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

#define BIN_COUNT 9

struct HOG_BLOCK {
  Rect rect_block;
  vector<Mat> cell_matrices;
  vector<vector<int> > hogs;
  vector<vector<float> > normalized_hogs;
};

vector<HOG_BLOCK> compute_hog_features(Mat image, int kernel_cell_size, int block_size, int gaussian_blur_kernel);

#endif