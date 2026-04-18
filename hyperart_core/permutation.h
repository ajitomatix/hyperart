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

#ifndef PERMUTATION_H
#define PERMUTATION_H

#include <stdio.h>
#include <vector>
#include <iostream>
using namespace std;

/**
Permutation is an arrangement represented by sequence of integers. The objects which are permuted themselves should be mapped using this sequence by the user of this class. By default created Permutation is an identity permutation which is just an ordered sequence [0 , size)

@author Ajit Datar
*/

typedef vector<size_t> PermIndexVec_;

class Permutation{
public:
    Permutation(size_t sz=0);

    ~Permutation();
    void setSize(size_t size);
    void initIdentity();
    
    Permutation(const Permutation&);    //copy constructor
    Permutation& operator=(const Permutation&); //copy assignment
    size_t size() const { return size_; }
    size_t& operator[](const size_t i);
    const size_t& operator[](const size_t i) const;
    Permutation& operator+=(const Permutation& perm);
    friend ostream& operator<< (ostream& o, const Permutation& perm);
private:
    size_t size_;
    PermIndexVec_ vec_;
};

const Permutation operator+(const Permutation& p1, const Permutation& p2);
#endif
