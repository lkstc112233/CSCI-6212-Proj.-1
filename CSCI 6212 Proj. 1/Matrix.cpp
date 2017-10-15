//
//  Matrix.cpp
//  CSCI 6212 Proj. 1
//
//  Created by 煦 柯 on 2017/10/14.
//  Copyright © 2017年 Must Studio. All rights reserved.
//

#include "Matrix.hpp"
#include <cstdlib>
#include <algorithm>

Matrix::Matrix(int s)
: size(s)
{
    data = new double[size * size];
}

Matrix::Matrix(const Matrix& m)
: size(m.size)
{
    data = new double[size * size];
    memcpy(data, m.data, size * size * sizeof(double));
}

Matrix& Matrix::operator=(const Matrix& m)
{
    size = m.size;
    data = new double[size * size];
    memcpy(data, m.data, size * size * sizeof(double));
    return *this;
}

Matrix::~Matrix()
{
    delete[] data;
}

// Naive way
void Matrix::Multiply1(const Matrix &m2, Matrix& output)
{
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j)
        {
            double sum = 0;
            for (int k = 0; k < size; ++k)
                sum += get(i, k) * get(k, j);
            output.set(i, j, sum);
        }
}
// Strassen’s algorithm
void Matrix::Multiply2(const Matrix &m2, Matrix& output)
{
    if (size == 1)
    {
        output.set(0, 0, get(0, 0) * m2.get(0, 0));
        return;
    }
    
    int subSize = size / 2;
    Matrix A11(subSize);
    Matrix A12(subSize);
    Matrix A21(subSize);
    Matrix A22(subSize);
    Matrix B11(subSize);
    Matrix B12(subSize);
    Matrix B21(subSize);
    Matrix B22(subSize);
    
    split(A11, A12, A21, A22);
    m2.split(B11, B12, B21, B22);
    
    Matrix M1(subSize);
    Matrix M2(subSize);
    Matrix M3(subSize);
    Matrix M4(subSize);
    Matrix M5(subSize);
    Matrix M6(subSize);
    Matrix M7(subSize);
    
    Matrix T1(A11);
    T1 += A22;
    Matrix T2(B11);
    T2 += B22;
    T1.Multiply2(T2, M1); // M1 = (A11 + A22)(B11 + B22)
    T1 = A21;
    T1 += A22;
    T1.Multiply2(B11, M2); // M2 = (A21 + A22)B11
    T1 = B12;
    T1 -= B22;
    A11.Multiply2(T1, M3); // M3 = A11(B12 - B22)
    T1 = B21;
    T1 -= B11;
    A22.Multiply2(T1, M4); // M4 = A22(B21 - B11)
    T1 = A11;
    T1 += A12;
    T1.Multiply2(B22, M5); // M5 = (A11 + A12)B22
    T1 = A21;
    T1 -= A11;
    T2 = B11;
    T2 += B12;
    T1.Multiply2(T2, M6); // M6 = (A21 - A11)(B11 + B12)
    T1 = A12;
    T1 -= A22;
    T2 = B21;
    T2 += B22;
    T1.Multiply2(T2, M7); // M7 = (A12 - A22)(B21 + B22)
    
    Matrix C11(subSize);
    Matrix C12(subSize);
    Matrix C21(subSize);
    Matrix C22(subSize);
    
    C11 = M1;
    C11 += M4;
    C11 -= M5;
    C11 += M7;
    C12 = M3;
    C12 += M5;
    C21 = M2;
    C21 += M4;
    C22 = M1;
    C22 -= M2;
    C22 += M3;
    C22 += M6;
    
    output.merge(C11, C12, C21, C22);
}

Matrix& Matrix::operator+=(const Matrix &m)
{
    int size2 = size * size;
    for (int i = 0; i < size2; ++i)
        data[i] += m.data[i];
    return *this;
}

Matrix& Matrix::operator-=(const Matrix &m)
{
    int size2 = size * size;
    for (int i = 0; i < size2; ++i)
        data[i] -= m.data[i];
    return *this;
}

void Matrix::split(Matrix &_11, Matrix &_12, Matrix &_21, Matrix &_22) const
{
    int smallerSize = _11.size;
    for (int i = 0; i < smallerSize; ++i)
        for (int j = 0; j < smallerSize; ++j)
        {
            _11.set(i, j, get(i, j));
            _12.set(i, j, get(i, j + smallerSize));
            _21.set(i, j, get(i + smallerSize, j));
            _22.set(i, j, get(i + smallerSize, j + smallerSize));
        }
}

void Matrix::merge(const Matrix &_11, const Matrix &_12, const Matrix &_21, const Matrix &_22)
{
    int smallerSize = _11.size;
    for (int i = 0; i < smallerSize; ++i)
        for (int j = 0; j < smallerSize; ++j)
        {
            set(i, j, _11.get(i, j));
            set(i, j + smallerSize, _12.get(i, j));
            set(i + smallerSize, j, _21.get(i, j));
            set(i + smallerSize, j + smallerSize, _22.get(i, j));
        }
}

// Parallel and distributed algorithms
void Matrix::Multiply3(const Matrix &m3, Matrix& output)
{
    
}