#pragma once
#include "stdafx.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <sstream> // stringstream
#include <fstream> // ifstream
#include <istream>


using namespace std;

class Matrix
{
protected:
	int _C, _R;
	double* _data;

	
public:
	//constructor
	Matrix();
	Matrix(int sizeR, int sizeC, double* input_data);

	Matrix(const Matrix& m);

	//destructor
	virtual ~Matrix();

	double get(int i, int j);

	void set(int i, int j, double val);
	void binarize();
	void setBlock(int Rpos, int Cpos, Matrix copy);

	double* getData();

	Matrix getBlock(int start_row, int end_row, int start_column, int end_column);

	int getN();

	int getM();

	double sum();

	Matrix add(const Matrix& other);

	Matrix operator+(const Matrix& other);

	Matrix operator=(const Matrix& other);

	Matrix operator*(const Matrix& other);
};

Matrix::Matrix()
{
	_R = 1;
	_C = 1;

	_data = new double[_R*_C];

	for (int x = 0; x < _R*_C; x++)
	{
		_data[x] = 0;
	}

}

Matrix::Matrix(int sizeR, int sizeC, double* input_data)
{
	_R = sizeR;
	_C = sizeC;

	_data = new double[_R*_C];

	for (int x = 0; x < _R*_C; x++)
	{
		_data[x] = input_data[x];
	}
}
//copy constructor
Matrix::Matrix(const Matrix& m)
{
	_R = m._R;
	_C = m._C;

	_data = new double[_R*_C];

	for (int i = 0; i < _R*_C; i++)
	{
		_data[i] = m._data[i];
	}
}

//operator overloads
Matrix Matrix::operator+(const Matrix& other)
{
	Matrix temp;
	temp._R = other._R;
	temp._C = other._C;

	temp._data = new double[temp._R*temp._C];

	for (int x = 0; x < (temp._R*temp._C); x++)
	{
		temp._data[x] = this->_data[x] + other._data[x];
	}

	return temp;
}

Matrix Matrix::operator=(const Matrix& other)
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

Matrix Matrix::operator*(const Matrix& other)
{
	Matrix temp;
	temp._R = other._R;
	temp._C = other._C;

	temp._data = new double[temp._R*temp._C];

	for (int x = 0; x < (temp._R*temp._C); x++)
	{
		temp._data[x] = this->_data[x] * other._data[x];
	}

	return temp;
}


//functions
int Matrix::getM()
{
	return _R;
}

int Matrix::getN()
{
	return _C;
}

double Matrix::get(int i, int j)
{
	return _data[(i*_C) + j];
}

Matrix Matrix::getBlock(int start_row, int end_row, int start_column, int end_column)
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
	Matrix temp(rows, columns, tempData);
	//delete the heap memory reserved for the extracted data
	delete[] tempData;

	//return the temporary matrix
	return temp;
}
void Matrix::binarize()
{
	for (int i = 0; i < 262144; i++) {
		if (_data[i] > 200)
		{
			_data[i] = 1;
		}
		else
		{
			_data[i] = 0;
		}
		cout << _data[i];
	}
}
void Matrix::setBlock(int Rpos, int Cpos, Matrix copy)
{
	for (int i = Rpos; i < Rpos + 32; i++)
	{
		for (int x = Cpos; x < Cpos + 32; x++)
		{
			_data[(i * _C) + x] = copy.get(i - Rpos, x - Cpos);
		}
	}
}

Matrix Matrix::add(const Matrix& other)
{
	//create temporary array of row*colum size
	double* data = new double[other._R*other._C];

	//fill the array with other._data
	for (int x = 0; x < (other._R*other._C); x++)
	{
		data[x] = this->_data[x] + other._data[x];
	}

	//create a temporary Matrix object with the row/column/data info
	Matrix temp(other._R, other._C, data);
	//delete the data array (which we can do as the array is 'deep copied' when 'temp' is created
	delete[] data;

	return temp;
}

double* Matrix::getData()
{
	return _data;
}

Matrix::~Matrix()
{
		delete[] _data;
}
void Matrix::set(int i, int j, double val)
{
	int x = (i * _C) + j;
	_data[x] = val;
}

double Matrix::sum()
{
	double Sum = 0;
	int count = 0;
	for (int i = 0; i < _R; i++)
	{
		for (int x = 0; x < _C; x++)
		{
			Sum += _data[count];
			count++;
		}
	}
	return Sum;
}