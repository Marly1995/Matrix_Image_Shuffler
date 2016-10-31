#pragma once
#include "stdafx.h"
#include "Matrix.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <sstream> // stringstream
#include <fstream> // ifstream
#include <istream>

class Binary : public Matrix
{
private:
	double* _data;
public:
	Binary();
	Binary(int sizeR, int sizeC, double* input_data);
	Binary(const Binary& m);

	~Binary();

	void binarize();

	Binary getBlock(int start_row, int end_row, int start_column, int end_column);
	Binary operator=(const Binary& other);
};

Binary::Binary() : Matrix()
{
	_R = 1;
	_C = 1;

	_data = new double[_R*_C];

	for (int x = 0; x < _R*_C; x++)
	{
		_data[x] = 0;
	}

}

Binary::Binary(int sizeR, int sizeC, double* input_data) : Matrix::Matrix(sizeR, sizeC, input_data)
{
	_R = sizeR;
	_C = sizeC;

	_data = new double[_R*_C];

	for (int x = 0; x < _R*_C; x++)
	{
		_data[x] = input_data[x];
	}
}

Binary::Binary(const Binary& m) : Matrix::Matrix(m)
{
	_R = m._R;
	_C = m._C;

	_data = new double[_R*_C];

	for (int i = 0; i < _R*_C; i++)
	{
		_data[i] = m._data[i];
	}
}

Binary::~Binary()
{
	delete[] _data;
}

void Binary::binarize()
{
	for (int i = 0; i < 262144; i++) {
		if (_data[i] < 50)
		{
			_data[i] = 1;
		}
		else
		{
			_data[i] = 0;
		}
	}
}
Binary Binary::getBlock(int start_row, int end_row, int start_column, int end_column)
{
	//set number of rows and columns in the subimage
	int rows = (end_row - start_row) + 1;
	int columns = (end_column - start_column) + 1;
	//reserve the memory for data extracted from the main image
	double* tempData = new double[rows*columns];

	//variable to set the new array index
	int count = 0;

	//iterate through the image data, extracting the values according to the rows/columns
	for (int x = start_row; x < (rows + start_row); x++)
	{
		for (int y = start_column; y < (columns + start_column); y++)
		{
			tempData[count] = get(x, y);
			count++;
		}
	}

	//create a temporary matrix based on the row/column/data extracted
	Binary temp(rows, columns, tempData);
	//delete the heap memory reserved for the extracted data
	delete[] tempData;

	//return the temporary matrix
	return temp;
}

Binary Binary::operator=(const Binary& other)
{
	//delete existing _data information - as we are going to replace it with 'other._data'
	delete[] _data;
	_R = other._R;
	_C = other._C;

	//reserve memory for new array
	_data = new double[_R*_C];

	//'this' pointer refers to the current object
	for (int x = 0; x < (_R*_C); x++)
	{
		this->_data[x] = other._data[x];
	}

	return *this;
}