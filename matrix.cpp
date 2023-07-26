/***************************************************************************
 *   Copyright (C) 2005 by Dr. Douglas Dunham, Ajit Datar                  *
 *   ddunham@d.umn.edu , data0003@d.umn.edu                                *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/
#include "matrix.h"

#include <iostream>
using namespace std;

Matrix::Matrix(int r, int c)
{
    //TODO r and c value checks
    rows_ = r;
    cols_ = c;
    storage_.resize( rows_ );
    for(int i=0; i< rows_; i++) {
        storage_[i].resize(cols_, 0.0);
    }
}

void Matrix::reinit(int r, int c)
{
    rows_ = r;
    cols_ = c;
    storage_.resize( rows_ );
    for(int i=0; i< rows_; i++) {
        storage_[i].resize(cols_, 0.0);
    }
}

void Matrix::initIdentity()
{
    for(int i=0; i<rows_; i++) {
        for(int j=0; j<cols_; j++) {
            if(i == j) storage_[i][j] = 1.0;
            else storage_[i][j] = 0.0;
        }
    }
}

Matrix::~Matrix()
{
}

Matrix& Matrix::operator+= (const Matrix& m)
{
    if( (this->rows() != m.rows() ) && (this->cols() != m.cols() ) ) {
        cerr<<"WARNING: Invalid matrix addition requested"<<endl;
        return *this;
    }

    for(int i=0; i< this->rows(); i++) {
        for(int j=0; j< this->cols(); j++) {
            storage_[i][j] += m[i][j];
        }
    }
    return *this;
}

const Matrix operator* (const Matrix &m1, const Matrix &m2)
{
    Matrix result(m1.rows(), m2.cols());

    if(m1.cols() != m2.rows()) {
        //cerr<<"WARNING: Invalid matrix multiplication requested"<<endl;
        throw "WARNING: Invalid matrix multiplication requested";
        return result;
    }

    //Result_ij = m1_ik * m2_kj
    for(int i=0; i< m1.rows(); i++) {
        for(int j=0; j< m2.cols(); j++) {
            result[i][j] = 0.0;
            for(int k=0; k< m2.rows(); k++) {
                result[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
    return result;
}

ostream& operator<< (ostream& o, const Matrix& m)
{
    for(int i=0; i<m.rows(); i++) {
        for(int j=0; j<m.cols(); j++) {
            o<< m[i][j] <<" ";
        }
        o<<endl;
    }
    return o;
}

//     //test matrix
//     Matrix m1(4,3);
//     m1[0][0] = 1;   m1[0][1] = 3;  m1[0][2] = 5;
//     m1[1][0] = 13;   m1[1][1] = 2;  m1[1][2] = 5;
//     m1[2][0] = 0;   m1[2][1] = 2;  m1[2][2] = 10;
//     m1[3][0] = 6;   m1[3][1] = 7;  m1[3][2] = 0;
//     cout<<"m1:\n"<<m1;
// 
//     Matrix m2(3,2);
//     m2[0][0] = 1;   m2[0][1] = 2;
//     m2[1][0] = 5;   m2[1][1] = 3;
//     m2[2][0] = 4;   m2[2][1] = 9;
//     cout<<"m2:\n"<<m2;
// 
//     Matrix m3 = m1 * m2;
//     cout<<"m3:\n"<<m3;
//     Matrix m4 = m2;
//     m4 += m2;
//     cout<<"m4:\n"<<m4;
