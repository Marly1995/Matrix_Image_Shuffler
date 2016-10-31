// ConsoleApplication1.cpp : Defines the entry point for the console application.
//
#pragma once
#include "stdafx.h"
#include "Matrix.h"
#include "Binary.h"
#include "Timer.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <sstream> // stringstream
#include <fstream> // ifstream
#include <istream>

using namespace std;

// Reads .txt file representing an image of R rows and C Columns stored in filename 
// and converts it to a 1D array of doubles of size R*C
// Memory allocation is performed inside readTXT
double* readTXT(char *fileName, int sizeR, int sizeC);

// Converts a 1D array of doubles of size R*C to .pgm image of R rows and C Columns 
// and stores .pgm in filename
// Use Q = 255 for greyscale images and Q=1 for binary images.
void WritePGM(char *filename, double *data, int sizeR, int sizeC, int Q);

// The SSD search used
double ssd(Matrix comparison, Matrix data);
// The NC search used
double NC(Matrix comparison, Matrix data);


int main()
{
	// stopwatch that times speed of program
	// Used for determining Time efficiency of NC and SSD
	stopwatch timer;
	// This part will show you how to use the two functions.

	//M and N represent the number of rows and columns in the image,
	//e.g. for task 1: logo_with_noise and logo_shuffled, M = 512, N = 512


	int M = 512; int N = 512;
	// input_data is a pointer to a 1D array of M*N doubles stored in heap. Memory allocation is performed 
	// inside readTXT. readTXT will read an image (in .pgm format) of size MxN and will  store the result in input_data.
	// once you're done with data DO NOT forget to delete the memory as in the end of this main() function
	double* input_data = 0;
	double* comparison_data = 0;

	cout << endl;
	cout << "Data from text file -------------------------------------------" << endl;

	// .pgm image is stored in inputFileName, change the path in your program appropriately
	char* inputFileName = "shuffled_logo.txt";
	input_data = readTXT(inputFileName, M, N);
	// .pgm noisy image stored n comparisonfilename
	char * comparisonFileName = "unshuffled_logo_noisy.txt";
	comparison_data = readTXT(comparisonFileName, M, N);

	/*CODE TO PROCESS*/
	//create matrix to hold shuffled image data
	Matrix Dat = Matrix(512, 512, input_data);
	// create matrix to hold noisy image data
	Matrix Comp = Matrix(512, 512, comparison_data);
	// create matrix to write unshuffled image data to
	Matrix Output = Matrix(512, 512, input_data);

	// str and stc are the starting points for rows and columns in the shuffled matrix
	// used to increment through blocks in order
	int str = 0;
	int stc = 0;

	// same as stc and str but for noisy matrix
	int Cstr = 0;
	int Cstc = 0;

	// initial score to compare SSD result to
	// changed to 0 for NC
	double score = 100000000;
	// initalize a value for the comparison score of each block
	double tempBlockScore = 0;
	// initalize an array to store the data determning whether a block has already been matched
	int checker[256];
	// initilize a value used to keep track of the block that was used last
	int checkTemp = 0;

	// initalize a matrix used to store the best match for each block
	Matrix writer(32, 32, input_data);
	// loop through all the blocks in nosiy matrix
	for (int ppp = 0; ppp < 256; ppp++)
	{
		// initalize a temporary matrix with the data from the current block of the noisy matrix
		Matrix compTemp = Comp.getBlock(Cstr, Cstr + 31, Cstc, Cstc + 31);
		// loop through all the blocks in the shuffled matrix 
		for (int x = 0; x < 256; x++)
		{
			// make sure the block has not already been matched
			// if it has dont search it
			// this should approximatley double speed of search
			if (checker[x] != 1)
			{
				// initalize a temporary matrix to store data from current block in shuffled matrix
				Matrix temp = Dat.getBlock(str, str + 31, stc, stc + 31);

				// run SSD or NC and store outcome in tempblockscore
				tempBlockScore = ssd(compTemp, temp);
				//tempBlockScore = NC(compTemp, temp);
				
				// if the score is the best so far use that block
				if (tempBlockScore < score){
				
					// set best so far score to current
					score = tempBlockScore;
					// set writer matrix to the best matched block 
					writer = temp;
					// set checker value to the best matched block location
					checkTemp = x;
				}
			}
				// this iterates through all of the blocks in the matrix
				str += 32;
				if (str > 481)
				{
					stc += 32;
					str = 0;
				}
		}
		// set checker to the best matched block 
		checker[checkTemp] = 1;
		// reset temporary checker
		checkTemp = 0;
		// set the current block in output to the best matched block
		Output.setBlock(Cstr, Cstc, writer);
		//reseet score
		score = 100000000;

		// iterate through all blocks in matrix
		Cstr += 32;
		if (Cstr > 481)
		{
			Cstc += 32;
			Cstr = 0;
		}
		// reset position to start for shuffled matrix
		stc = 0;
		str = 0;
	}
	
	/*CODE TO PROCESS*/
	// writes data back to .pgm file stored in outputFileName
	char* outputFileName = "logo_restored.pgm";
	// Use Q = 255 for greyscale images and 1 for binary images.
	int Q = 255;
	WritePGM(outputFileName, Output.getData(), M, N, Q);

	delete[] input_data;
	delete[] comparison_data;
	return 0;
}


// Read .txt file with image of size RxC, and convert to an array of doubles
double* readTXT(char *fileName, int sizeR, int sizeC)
{
	double* data = new double[sizeR*sizeC];
	int i = 0;
	ifstream myfile(fileName);
	if (myfile.is_open())
	{

		while (myfile.good())
		{
			if (i>sizeR*sizeC - 1) break;
			myfile >> *(data + i);
			// cout << *(data+i) << ' '; // This line display the converted data on the screen, you may comment it out. 
			i++;
		}
		myfile.close();
	}

	else cout << "Unable to open file";
	//cout << i;

	return data;
}

// convert data from double to .pgm stored in filename
void WritePGM(char *filename, double *data, int sizeR, int sizeC, int Q)
{

	int i;
	unsigned char *image;
	ofstream myfile;

	image = (unsigned char *) new unsigned char[sizeR*sizeC];

	// convert the integer values to unsigned char

	for (i = 0; i<sizeR*sizeC; i++)
		image[i] = (unsigned char)data[i];

	myfile.open(filename, ios::out | ios::binary | ios::trunc);

	if (!myfile) {
		cout << "Can't open file: " << filename << endl;
		exit(1);
	}

	myfile << "P5" << endl;
	myfile << sizeC << " " << sizeR << endl;
	myfile << Q << endl;

	myfile.write(reinterpret_cast<char *>(image), (sizeR*sizeC)*sizeof(unsigned char));

	if (myfile.fail()) {
		cout << "Can't write image " << filename << endl;
		exit(0);
	}

	myfile.close();

	delete[] image;

}

double ssd(Matrix comparison, Matrix data)
{
	double tempBlockScore = 0;

	for (int r = 0; r < 32; r++)
	{
		for (int c = 0; c < 32; c++)
		{
				tempBlockScore += sqrt(pow((comparison.get(r, c) - data.get(r, c)), 2));
				//cout << tempBlockScore << "  ";
		}
	}

	return tempBlockScore;
}

double NC(Matrix comparison, Matrix data)
{
	double normal = 0;
	double compMean = comparison.sum() / 1024;
	double dataMean = data.sum() / 1024;

	for (int r = 0; r < 32; r++)
	{
		for (int c = 0; c < 32; c++)
		{
			comparison.set(r, c, comparison.get(r, c) - compMean);
			data.set(r, c, data.get(r, c) - dataMean);
		}
	}
	
	Matrix top = data * comparison;
	double NCtop = top.sum();
	Matrix NCbotL = comparison * comparison;
	Matrix NCbotR = data * data;
	double NCBL = NCbotL.sum();
	double NCBR = NCbotR.sum();
	double NCBline = NCBL * NCBR;

	normal = (NCtop / sqrt(NCBline));
	return normal;
}