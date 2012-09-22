#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;

/**
 * HOG-EXTRACTOR
 * Tool to extract histogram of oriented gradients
 * of a given image.
 */
int main(int argc, char **argv)
{
  vector<Rect> cells;
  vector<Mat> cell_images;
  vector<Rect> blocks;
  vector<Mat> block_images;

  Mat cell_image;
  Mat block_image;

	/**
	 * If the number of arguments is not equal to 2 then exit.
	 */
	if(argc != 4) {
		cout << "Syntax: hog-extractor [filename] [kernel_cell_size] [block_size]" << endl;
		exit(-1);
	}
 
  // Read file as grayscale
  Mat original_image = imread(argv[1], 0);
  original_image.copyTo(cell_image);
  original_image.copyTo(block_image);

  int height = original_image.rows;
  int width = original_image.cols;

  // Print out dimensions
  cout << "Dimensions of image:" << endl;
  cout << "height: " << height << endl; 
  cout << "width: " << width << endl;
  cout << "-------------------" << endl;

  // Parameters of HOG
  int kernel_cell_size = atoi(argv[2]);
  cout << "kernel cell size: " << kernel_cell_size << endl;

  if(height % kernel_cell_size > 0 || width % kernel_cell_size > 0) {
    cerr << "Invalid kernel cell size." << endl;
    exit(-1);
  }

  int block_size = atoi(argv[3]);
  cout << "block size: " << block_size << endl;
  int num_cells_height = height / kernel_cell_size;
  int num_cells_width = width / kernel_cell_size;
  if(num_cells_height % block_size > 0 || num_cells_width % block_size > 0) {
    cerr << "Invalid block size." << endl;
    exit(-1);
  }

  // Divide according to cell size
  for(int i = 0; i < width; i += kernel_cell_size) {
    for(int j = 0; j < height; j += kernel_cell_size) {
      int upper_left_x = i;
      int upper_left_y = j;

      Rect r(upper_left_x, upper_left_y, kernel_cell_size, kernel_cell_size);
      Mat imageROI = cell_image(r);

      cells.push_back(r);
      cell_images.push_back(imageROI);
    }
  }

  // Draw the division lines and show the image
  Scalar s(255);
  for(u_int i = 0; i < cells.size(); i++) {
    //printf("Rectangle %d: %d, %d, %d, %d\n", i, cells.at(i).x, cells.at(i).y, cells.at(i).width, cells.at(i).height);
    rectangle(cell_image,
              Point(cells.at(i).x, cells.at(i).y),
              Point(cells.at(i).x + cells.at(i).width, cells.at(i).y + cells.at(i).height),
              s,
              1, 8);
  }

  // Divide cells according to block size
  for(int i = 0; i < width - kernel_cell_size; i += kernel_cell_size) {
    for(int j = 0; j < height - kernel_cell_size; j += kernel_cell_size) {
      int upper_left_x = i;
      int upper_left_y = j;
      Rect r(upper_left_x, upper_left_y, kernel_cell_size * block_size, kernel_cell_size * block_size);
      Mat imageROI = original_image(r);

      blocks.push_back(r);
      block_images.push_back(imageROI);
    }
  }

  imshow("ORIGINAL", original_image);
  imshow("GRID", cell_image);
  imshow("BLOCK", block_images.at(0));

  cout << "Number of cells: " << cells.size() << endl;
  cout << "Number of blocks: " << blocks.size() << endl;

  waitKey(0);

	return 0;
}
