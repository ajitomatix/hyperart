/***************************************************************************
 *   Copyright (C) 2005 by Dr. Douglas Dunham, Ajit Datar                  *
 *   ddunham@d.umn.edu , data0003@d.umn.edu                                *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/
#include "pgonedge.h"

PgonEdge::PgonEdge()
{
}

/**
    setNumColors has to be called in order for perm to be initilized
*/
void PgonEdge::setNumColors(size_t numColors) {
    perm_.setSize(numColors);
}

PgonEdge::~PgonEdge()
{
}


