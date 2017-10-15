//
//  Matrix.hpp
//  CSCI 6212 Proj. 1
//
//  Created by 煦 柯 on 2017/10/14.
//  Copyright © 2017年 Must Studio. All rights reserved.
//

#ifndef Matrix_hpp
#define Matrix_hpp

class Matrix
{
public:
    Matrix(int size);
    Matrix(const Matrix&);
    Matrix& operator=(const Matrix&);
    ~Matrix();
    // Naive way
    void Multiply1(const Matrix&, Matrix& output);
    // Strassen’s algorithm
    void Multiply2(const Matrix&, Matrix& output);
    // Parallel and distributed algorithms
    void Multiply3(const Matrix&, Matrix& output);
    inline double get(int x, int y) const {return data[x * size + y];}
    inline void set(int x, int y, double value){data[x * size + y] = value;}
private:
    void split(Matrix&,Matrix&,Matrix&,Matrix&) const;
    void merge(const Matrix&, const Matrix&, const Matrix&, const Matrix&);
    Matrix& operator+=(const Matrix&);
    Matrix& operator-=(const Matrix&);
    double* data;
    int size;
};

#endif /* Matrix_hpp */
