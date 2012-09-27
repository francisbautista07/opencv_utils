#include "cvu_hog.h"

vector<HOG_BLOCK> compute_hog_features(Mat image, int kernel_cell_size, int block_size, int gaussian_blur_kernel) 
{
  int height = image.rows;
  int width = image.cols;

  vector<Mat> block_matrix;

  Mat gradient_image;

  Mat grad_x, grad_y;

  vector<HOG_BLOCK> hog_blocks;

  // Check for proper kernel size
  if(height % kernel_cell_size > 0 || width % kernel_cell_size > 0) {
    cerr << "Invalid kernel cell size." << endl;
    
    return hog_blocks;
  }

  // Check for proper block size
  int num_cells_height = height / kernel_cell_size;
  int num_cells_width = width / kernel_cell_size;
  if(num_cells_height % block_size > 0 || num_cells_width % block_size > 0) {
    cerr << "Invalid block size." << endl;
    
    return hog_blocks;
  }

  image.copyTo(gradient_image);

  // Equalize histogram
  equalizeHist(gradient_image, gradient_image);

  // Gaussian blur kernel size
  GaussianBlur(gradient_image, gradient_image, Size(gaussian_blur_kernel, gaussian_blur_kernel), 0, 0);

  // Compute for gradient x-wise using Sobel operator
  Sobel(gradient_image, grad_x, CV_32F, 1, 0, 3);

  // Compute for graident y-wise using Sobel operator
  Sobel(gradient_image, grad_y, CV_32F, 0, 1, 3);

  // Compute for gradient orientation.
  // Store in new gradient matrix.
  Mat orientation_matrix(gradient_image.rows, gradient_image.cols, DataType<float>::type);
  for(int i = 0; i < gradient_image.rows; i++) {
    for(int j = 0; j < gradient_image.cols; j++) {
      float gx = grad_x.at<float>(i,j);
      float gy = grad_y.at<float>(i,j);
      float orientation = atan(gy / (gx + 0.00001)) * (180 / 3.14) + 90;
      orientation_matrix.at<float>(i, j) = orientation;
    }
  }

  // Divide cells according to block size
  for(int i = 0; i < width - (kernel_cell_size * block_size) + 1; i += (kernel_cell_size * block_size) / 2) {
    for(int j = 0; j < height - (kernel_cell_size * block_size) + 1; j += (kernel_cell_size * block_size) / 2) {
      
      Rect r(i, j, kernel_cell_size * block_size, kernel_cell_size * block_size);
      Mat b_matrix = orientation_matrix(r);

      block_matrix.push_back(b_matrix);

      HOG_BLOCK hg;
      hg.rect_block = r;
      for(int row = 0; row < (kernel_cell_size * block_size); row += kernel_cell_size) {
        for(int col = 0; col < (kernel_cell_size * block_size); col += kernel_cell_size) {
          Rect r2(row, col, kernel_cell_size, kernel_cell_size);
          Mat c_matrix = b_matrix(r2);

          hg.cell_matrices.push_back(c_matrix);
        }
      }

      hog_blocks.push_back(hg);
    }
  }

  // Store histogram of oriented gradients per block
  // Loop through each hog_block
  //    Loop through each cell of hog_block
  //      Get orientation of each pixel position in cell and increment corresponding bin
  for(int i = 0; i < hog_blocks.size(); i++) {
    for(int j = 0; j < hog_blocks.at(i).cell_matrices.size(); j++) {
      vector<int> histogram_of_oriented_gradient;
      histogram_of_oriented_gradient.resize(BIN_COUNT);
      for(int row = 0; row < hog_blocks.at(i).cell_matrices.at(j).rows; row++) {
        for(int col = 0; col < hog_blocks.at(i).cell_matrices.at(j).cols; col++) {
          float o = hog_blocks.at(i).cell_matrices.at(j).at<float>(row, col);
          if(o <= 20) {
            histogram_of_oriented_gradient.at(0)++;
          } else if(o > 20 && o <= 40) {
            histogram_of_oriented_gradient.at(1)++;
          } else if(o > 40 && o <= 60) {
            histogram_of_oriented_gradient.at(2)++;
          } else if(o > 60 && o <= 80) {
            histogram_of_oriented_gradient.at(3)++;
          } else if(o > 80 && o <= 100) {
            histogram_of_oriented_gradient.at(4)++;
          } else if(o > 100 && o <= 120) {
            histogram_of_oriented_gradient.at(5)++;
          } else if(o > 120 && o <= 140) {
            histogram_of_oriented_gradient.at(6)++;
          } else if(o > 140 && o <= 160) {
            histogram_of_oriented_gradient.at(7)++;
          } else if(o > 160 && o <= 180) {
            histogram_of_oriented_gradient.at(8)++;
          }
        }
      }

      hog_blocks.at(i).hogs.push_back(histogram_of_oriented_gradient);
    }
  }

  // Normalize the hogs
  for(int i = 0; i < hog_blocks.size(); i++) {
    for(int j = 0; j < hog_blocks.at(i).hogs.size(); j++) {
      vector<float> normalized_hogs;
      normalized_hogs.resize(BIN_COUNT);
      float l2norm = 0;
      
      for(int b = 0; b < BIN_COUNT; b++) {
        l2norm += pow(hog_blocks.at(i).hogs.at(j).at(b), 2);
      }
      
      l2norm = sqrt(l2norm);

      for(int b = 0; b < BIN_COUNT; b++) {
        normalized_hogs.at(b) = hog_blocks.at(i).hogs.at(j).at(b) / l2norm;
      }

      hog_blocks.at(i).normalized_hogs.push_back(normalized_hogs);
    }
  }

  return hog_blocks;
}