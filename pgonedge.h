/***************************************************************************
 *   Copyright (C) 2005 by Dr. Douglas Dunham, Ajit Datar                  *
 *   ddunham@d.umn.edu , data0003@d.umn.edu                                *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/
#ifndef PGONEDGE_H
#define PGONEDGE_H
#include "defs.h"
#include "permutation.h"

/**
A Pgon edge for the central pgon. It has adjacency info.

@author Ajit Datar
*/
class PgonEdge{
public:
    PgonEdge();

    ~PgonEdge();
    void setNumColors(size_t numColors);

    Orientation orientation() { return orientation_; }
    Permutation& colorPerm() { return perm_; }
    int adjEdgeId() { return adjEdgeId_; }
    
    void setAdjEdgeId(int v) { adjEdgeId_ = v; }
    void setOrientation(Orientation o) { orientation_ = o; }
    
private:
    Orientation orientation_;
    Permutation perm_;
    int adjEdgeId_;
};

#endif
