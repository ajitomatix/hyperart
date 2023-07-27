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
