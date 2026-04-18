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
