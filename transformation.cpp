/***************************************************************************
 *   Copyright (C) 2005 by Dr. Douglas Dunham, Ajit Datar                  *
 *   ddunham@d.umn.edu , data0003@d.umn.edu                                *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/
#include "transformation.h"

/**
Creates an identity transformation by default

It has an identity matrix, identity permutation for ncolors,
pPos set to 0 and orientation set to ROTATION
*/
Transformation::Transformation(int ncolors)
 : pPos_(0), orient_(ROTATION), colorPerm_(Permutation(ncolors))
{
    mat_.initIdentity();
}

void Transformation::initIdentity()
{
    pPos_ = 0;
    orient_ = ROTATION;
    colorPerm_.initIdentity();
    mat_.initIdentity();
}

Transformation::~Transformation()
{
}

Transformation& Transformation::operator*= (const Transformation& t)
{
    mat_ = mat_ * t.mat_;
    colorPerm_ += t.colorPerm_;
    pPos_ = t.pPos_;
    if((int(orient_) * int(t.orient_)) < 0 ) {
        orient_ = REFLECTION;
    }
    else {
        orient_ = ROTATION;
    }
    return *this;
}

const Transformation operator* (const Transformation& t1, const Transformation& t2)
{
    Transformation result(t1);
    result *= t2;
    return result;
}
