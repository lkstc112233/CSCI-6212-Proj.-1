//
//  Matrix.hpp
//  CSCI 6212 Proj. 1
//
//  Created by 煦 柯 on 2017/10/14.
//  Copyright © 2017年 Must Studio. All rights reserved.
//

#ifndef Matrix_hpp
#define Matrix_hpp

extern int mappings[30000];

class Matrix
{
public:
    Matrix(int size);
	Matrix(const Matrix&);
	Matrix(const Matrix&, int position);
    Matrix& operator=(const Matrix&);
    virtual ~Matrix();
    // Naive way
    void Multiply1(const Matrix&, Matrix& output);
    // Strassen’s algorithm
    void Multiply2(const Matrix&, Matrix& output);
    // Parallel and distributed algorithms
    void Multiply3(const Matrix&, Matrix& output);
    inline double get(int x, int y) const 
	{
		if (size & (size - 1))
			return data[x * size + y];
		else
			return data[mappings[x] | (mappings[y] << 1)];
	}
    inline void set(int x, int y, double value)
	{
		if (size & (size - 1))
			data[x * size + y] = value;
		else
			data[mappings[x] | (mappings[y] << 1)] = value;
	}
protected:
    Matrix(bool b, const Matrix &m, int position) : size(m.size / 2), data(m.data + (m.size / 2) * (m.size / 2) * position){}
    void split(Matrix&,Matrix&,Matrix&,Matrix&) const;
    void merge(const Matrix&, const Matrix&, const Matrix&, const Matrix&);
    Matrix& operator+=(const Matrix&);
    Matrix& operator-=(const Matrix&);
	Matrix& plus(const Matrix&, int position);
	Matrix& plus(const Matrix&, int position, const Matrix&, int position2);
	Matrix& plus(int position, const Matrix&);
	Matrix& minus(const Matrix&, int position);
	Matrix& minus(const Matrix&, int position, const Matrix&, int position2);
	Matrix& minus(int position, const Matrix&);
	void set(const Matrix&, int position);
	void set(int position, const Matrix&);
    double* data;
    int size;
};

class TemperateMatrixStatic : public Matrix
{
public:
    TemperateMatrixStatic(const Matrix&, int position);
    ~TemperateMatrixStatic();
};

#endif /* Matrix_hpp */
