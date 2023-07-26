/***************************************************************************
 *   Copyright (C) 2005 by Dr. Douglas Dunham, Ajit Datar                  *
 *   ddunham@d.umn.edu , data0003@d.umn.edu                                *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
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
