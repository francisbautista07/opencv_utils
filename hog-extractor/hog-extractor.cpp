#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

#include "cvu_hog.h"

/**
 * HOG-EXTRACTOR
 * Tool to extract histogram of oriented gradients
 * of a given image.
 */
int main(int argc, char **argv)
{

  // Check input
	if(argc != 5) {
		cout << "Syntax: hog-extractor [filename] [kernel_cell_size] [block_size] [gaussian_blur_kernel]" << endl;
		exit(-1);
	}

  // Read file as grayscale
  Mat original_image = imread(argv[1], 0);
  int kernel_cell_size = atoi(argv[2]);
  int block_size = atoi(argv[3]);
  int gaussian_blur_kernel = atoi(argv[4]);
 
  vector<HOG_BLOCK> hog_blocks = compute_hog_features(original_image, kernel_cell_size, block_size, gaussian_blur_kernel);
  if(hog_blocks.size() == 0) {
    cerr << "Error" << endl;
    exit(-1);
  }

  // Display block contents
  for(int i = 0; i < hog_blocks.size(); i++) {
    cout << "BLOCK " << i << endl;
    cout << "  Cell size: " << hog_blocks.at(i).cell_matrices.size() << endl;
    cout << "  HoG size: " << hog_blocks.at(i).hogs.size() << endl;
    cout << "-----------------------" << endl;
    for(int j = 0; j < hog_blocks.at(i).hogs.size(); j++) {
      cout << "    Cell " << j << ":" << endl;
      for(int b = 0; b < 9; b++) {
        cout << "      Bin " << b << ": " << hog_blocks.at(i).hogs.at(j).at(b);
        cout << " <--> Normalized Bin " << b << ": " <<hog_blocks.at(i).normalized_hogs.at(j).at(b) << endl;
      }
    }
  }

  cout << "----------------" << endl;
  cout << "Number of blocks: " << hog_blocks.size() << endl;
  cout << "Cells per block: " << hog_blocks.at(0).cell_matrices.size() << endl;
  cout << "Cell size: " << kernel_cell_size << endl;
  cout << "Block size: " << block_size << endl;
  cout << "Dimensions: " << hog_blocks.size() * hog_blocks.at(0).cell_matrices.size() * 9 << endl;

	return 0;
}
