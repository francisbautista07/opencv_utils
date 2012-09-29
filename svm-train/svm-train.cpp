#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
	if(argc != 3) {
		cout << "Syntax: svm-train [data] [model]" << endl;
		cout << "==> data: file containing training samples" << endl;
		cout << "==> model: output file where model will be saved (xml)" << endl;

		exit(-1);
	}

	string data = argv[1];
	string model = argv[2];

	int num_lines = 0;
	vector<float> labels;
	vector<float> possible_labels;

	// Open data file
	ifstream data_file;

	// Get possible labels and count lines
	cout << "Extracting labels..." << endl;
	data_file.open(data.c_str());
	int dimension = 0;
	while(!data_file.eof())
	{
		string data_line;
		getline(data_file, data_line);

		string string_value;
		stringstream stream(data_line);

		// Parse the line and store it as a string in string_value
		float val = 0;
		dimension = 0;
		while(getline(stream, string_value, ' ')) {
			// Convert the value to a float
			val = ::atof(string_value.c_str());
			dimension++;
		}

		num_lines++;
		labels.push_back(val);
	}

	dimension = dimension - 1;

	data_file.close();

	cout << "Getting unique labels..." << endl;
	for(int i = 0; i < labels.size(); i++) {
		bool unique = true;
		if(possible_labels.size() == 0) {
			possible_labels.push_back(labels.at(i));
			unique = false;
		} else {
			for(int j = 0; j < possible_labels.size(); j++) {
				if(possible_labels.at(j) == labels.at(i)) {
					unique = false;
				}
			}
		}

		if(unique) {
			possible_labels.push_back(labels.at(i));
		}
	}

	cout << "Possible labels:" << endl;
	for(int i = 0; i < possible_labels.size(); i++) {
		cout << "==> " << possible_labels.at(i) << endl;
	}

	cout << "Generating labels..." << endl;
	float label_array[labels.size()];
	std::copy(labels.begin(), labels.end(), label_array);
	Mat label_mat(labels.size(), 1, DataType<float>::type, label_array);

	cout << "Number of dimensions: " << dimension << endl;
	cout << "Extracting features..." << endl;
	Mat training_data_mat(labels.size(), dimension, DataType<float>::type);
	data_file.open(data.c_str());
	int index = 0;
	while(!data_file.eof())
	{
		string data_line;
		getline(data_file, data_line);

		string string_value;
		stringstream stream(data_line);

		int counter = 0;
		while(getline(stream, string_value, ' ')) {
			float val = ::atof(string_value.c_str());
			if(counter != dimension) {
				training_data_mat.at<float>(index, counter) = val;
			}
			counter++;
		}
		index++;
	}

	data_file.close();

  // Train the SVM
  // Set up SVM's parameters
  CvSVMParams params;
  params.svm_type = CvSVM::C_SVC;
  params.C = 0.01;
  params.kernel_type = CvSVM::LINEAR;
  params.term_crit = cvTermCriteria(CV_TERMCRIT_ITER, (int)1e7, 1e-6);

	cout << "Number of samples: " << num_lines << endl;
  cout << "Training data..." << endl;
  CvSVM SVM;
  SVM.train(training_data_mat, label_mat, Mat(), Mat(), params);
  cout << "Done training!" << endl;

  cout << "Saving to model " << model << "..." << endl;
  SVM.save(model.c_str());

	return 0;
}