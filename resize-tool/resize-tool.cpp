#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;
using namespace cv;

/*
 * A simple program that resizes all jpg image files in a given directory
 */
/*
TODO: Configure input and output directory
*/
int main(int argc, char **argv)
{
	if(argc != 4) {
		cout << "Syntax: resize-tool [input-directory] [output-directory] [length] [width]" << endl;
		exit(-1);
	}

  int length = atoi(argv[2]);
  int width = atoi(argv[3]);

	ifstream fin;

	// The directory we want tot open
	string dir = argv[1];

	// Stores the file path of a read image sample
	string filepath;

	// A num counter so we can label each resized image according to the number
	int num = 0;

	// Directory pointer
	DIR *dp;

	struct dirent *dirp;
	struct stat filestat;

	cout << "Directory: " << dir << endl;

	dp = opendir(dir.c_str());

	if(dp == NULL) {
		cout << "Error in opening directory " << dir.c_str() << endl;
		exit(-1);
	}

	// Read all files inside the given directory
	while((dirp = readdir(dp))) {
		filepath = dir + "/" + dirp->d_name;

		if(stat(filepath.c_str(), &filestat)) continue;
		if(S_ISDIR(filestat.st_mode)) continue;

		fin.open(filepath.c_str());

		num++;
		cout << "File found: " << filepath << endl;

    /*
    TODO: Check if file is jpg before reading
    */

		// The original image read from the given filepath
		Mat img = imread(filepath);

		// A matrix to store the resized image
		Mat resized_image;

		// Resize the image based on given LENGTH and WIDTH
		resize(img, resized_image, Size(length, width));

		// Create a new filename for saving
		ostringstream o;
		o << num;
		string file = o.str();
		string file_type = ".jpg";
		string new_file = file + file_type;
		cout << "New file: " << new_file << endl;

		// Save the resized image
		imwrite(new_file, resized_image);

		fin.close();
	}

	cout << "Total number of files: " << num << endl;

	closedir(dp);

	return 0;
}
