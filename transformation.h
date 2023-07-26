/***************************************************************************
 *   Copyright (C) 2005 by Dr. Douglas Dunham, Ajit Datar                  *
 *   ddunham@d.umn.edu , data0003@d.umn.edu                                *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/
#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include "defs.h"
#include "matrix.h"
#include "permutation.h"
/**
A transformation consists of matrix, color permutations etc

@author Ajit Datar
*/
class Transformation{
public:
    Transformation(int ncolors=3);

    Matrix& matrix() { return mat_; }
    const Matrix& matrix() const { return mat_; }
    Permutation& colorPerm() { return colorPerm_; }
    const Permutation& colorPerm() const { return colorPerm_; }
    int pPos() const { return pPos_; }
    const Orientation orient() const { return orient_; }
    void initIdentity();
    
    void setMatrix(Matrix m) { mat_ = m; }
    void setColorPerm(Permutation p) { colorPerm_ = p; }
    void setpPos(int val) { pPos_ = val; }
    void setOrient(Orientation o) { orient_ = o; }
    Transformation& operator*= (const Transformation& t);

    ~Transformation();

private:
    Matrix mat_;
    Permutation colorPerm_;
    int pPos_; //TODO does it matter for anything else but edgeTran?
    Orientation orient_;
};

const Transformation operator*( const Transformation& t1, const Transformation& t2);

#endif
