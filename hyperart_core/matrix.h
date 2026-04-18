/***************************************************************************
 *   HyperArt Copyright (C) 2005-2023 by Ajit Datar (ajitdatar@gmail.com)  *
 *                                                                         *
 *   Hyperbolic geometry drawing algorithms developed by                   *
 *   and credited to Dr. Douglas Dunham (ddunham@d.umn.edu)                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   Please see LICENSE file for more details.                             *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
using namespace std;

/**
A matrix of double-precision numbers
Always stored row-major
eg A 3x4 matrix has 3 rows and 4 columns

Reinterpretation of matrix size to different row X col value
is not supported (you can do that by deriving your own class from this)
@author Ajit Datar
*/
class Matrix {
public:
    /**
    Creates a matrix of size r X c and initializes all elements to 0.
    Default is to create a 3x3 matrix.
    */
    Matrix(int r=3, int c=3);
    int rows() const { return rows_; }
    int cols() const { return cols_; }
    /**
    Makes this matrix an identity matrix
    */
    void initIdentity();

    /**
    resizes the matrix to r x c
    Old matrix data is lost
    */
    void reinit(int r=3, int c=3);
    /**
    get a reference to the row vector
    
    Even though this is against the OO practice of data hiding,
    this is provided so that we can use a natural notation like matrix[i][j]
    to directly get/set elements.
    Susceptible to overflow errors.
    */
    vector<double>& operator[] (int r) { return storage_[r]; }
    const vector<double>& operator[] (int r) const { return storage_[r]; }

    Matrix& operator+= (const Matrix& m);

    ~Matrix();

protected:
    int rows_;
    int cols_;
    vector< vector<double> > storage_; //internal storage for matrix elements
};

/**
Matrix multiplication m1 * m2

*/
const Matrix operator* (const Matrix& m1, const Matrix& m2);

ostream& operator<< (ostream& o, const Matrix& m);

#endif
