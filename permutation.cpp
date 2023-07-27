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

#include "permutation.h"

Permutation::Permutation(size_t sz) 
    : size_(sz)
{
    if(size_) {
        setSize(size_);
    }
}

void Permutation::setSize(size_t size) {
    size_ = size;
    vec_.resize(size_);
    //make it an identity permutation
    initIdentity();
}

void Permutation::initIdentity()
{
    for(size_t i=0; i<size_; i++) {
        vec_[i] = i;
    }
}

Permutation::~Permutation()
{
}

Permutation::Permutation(const Permutation& perm) {
    size_ = perm.size();
    vec_.resize(size_);
    for(size_t i=0; i < size_; i++) {
        vec_[i] = perm.vec_[i];
    }
}

Permutation& Permutation::operator= (const Permutation& perm) {
    if(this != &perm) {
        size_ = perm.size();
        vec_.resize(size_);
        for(size_t i=0; i < size_; i++) {
            vec_[i] = perm.vec_[i];
        }
    }
    return *this;
}

size_t& Permutation::operator[] ( const size_t i )
{
    //we just map this subscript operator to internal vector's operator
    if( i< size_) return vec_[i];
    else return vec_[size_-1];
}

const size_t& Permutation::operator[] ( const size_t i ) const
{
    //we just map this subscript operator to internal vector's operator
    if( i< size_) return vec_[i];
    else return vec_[size_-1];
}

/**
    defines an "addition" or "followed by" operation
    stores the result of 'this' permutation followed by 'perm'
    result[i] = perm[this[i]]
    If the sizes of the permutations don't match then the current permutation
    is not modified.
*/
Permutation& Permutation::operator+= (const Permutation& perm) {
    if( size_ != perm.size() ) {
        return *this;
    }

    for(size_t i=0; i < size_; i++) {
        vec_[i] = perm.vec_[ vec_[i] ];
    }
    return *this;
}

ostream& operator<< (ostream& o, const Permutation& perm) {
    o<<"[ ";
    for(size_t i=0; i < perm.size(); i++) {
        o<< perm.vec_[i] <<" ";
    }
    o<<" ]";
    return o;
}

const Permutation operator+ (const Permutation& p1, const Permutation& p2)
{
    Permutation result( p1 );
    result += p2;
    return result;
}

/**
    //some test code for Permutation class

    Permutation p1, p2(3);
    cout<<p1<<endl<<p2<<endl;
    p1.setSize(3);
    p1[0] = 1;
    p1[1] = 0;
    p1[2] = 2;

    p2[0] = 2;
    p2[1] = 1;
    p2[2] = 0;
    cout<<p1<<endl<<p2<<endl;
    p1 += p2;
    cout<<p1<<endl<<p2<<endl;
    p2 = p1;
    cout<<p1<<endl<<p2<<endl;
*/
