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

#include "cvu_hog.h"

/**
 * HOG-EXTRACTOR
 * Tool to extract histogram of oriented gradients
 * of a given image.
 */
int main(int argc, char **argv)
{

  // Check input
	if(argc != 7) {
		cout << "Syntax: hog-extractor [image_dir] [output_file] [kernel_cell_size] [block_size] [gaussian_blur_kernel] [label]" << endl;
		cout << "==> image_dir: directory containing sample images" << endl;
    cout << "==> output_file: file containing features extracted from images" << endl;
    cout << "==> kernel_cell_size: size dimension of cell (hog)" << endl;
    cout << "==> block_size: size of block (hog)" << endl;
    cout << "==> gaussian_blur_kernel: gaussian kernel dimension" << endl;
    cout << "==> label: numerical label for sampled images" << endl;

    exit(-1);
	}

  // The directory we want tot open
  string dir = argv[1];

  // Stores the file path of a read image sample
  string filepath;

  // Directory pointer
  DIR *dp;

  struct dirent *dirp;
  struct stat filestat;

  // Output file
  string output_file = argv[2];

  // parameters for compute_hog_features
  int kernel_cell_size = atoi(argv[3]);
  int block_size = atoi(argv[4]);
  int gaussian_blur_kernel = atoi(argv[5]);
  int label = atoi(argv[6]);

  cout << "Directory: " << dir << endl;

  dp = opendir(dir.c_str());

  if(dp == NULL) {
    cout << "Error in opening directory " << dir.c_str() << endl;
    exit(-1);
  }
  
  ifstream fin;

  // Open file for writing
  ofstream data_file;
  data_file.open(output_file.c_str());

  // Read all files inside the given directory
  while((dirp = readdir(dp))) {

    filepath = dir + "/" + dirp->d_name;

    if(stat(filepath.c_str(), &filestat)) continue;
    if(S_ISDIR(filestat.st_mode)) continue;

    fin.open(filepath.c_str());

    cout << "File found: " << filepath << endl;

    // Compute for the hog features of read image
    Mat img = imread(filepath, 0);
 
    vector<HOG_BLOCK> hog_blocks = compute_hog_features(img, kernel_cell_size, block_size, gaussian_blur_kernel);
    if(hog_blocks.size() == 0) {
      cerr << "Error" << endl;
      exit(-1);
    }

    // Write data to file
    cout << "Writing features for " << filepath << endl;
    for(int i = 0; i < hog_blocks.size(); i++) {
      for(int j = 0; j < hog_blocks.at(i).normalized_hogs.size(); j++) {
        for(int b = 0; b < BIN_COUNT; b++) {
          data_file << hog_blocks.at(i).normalized_hogs.at(j).at(b) << " ";
        }
      }
    }

    data_file << label << "\n";

    fin.close();
  }

  data_file.close();

	return 0;
}
