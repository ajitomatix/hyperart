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

#ifndef IRREGULARPGON_H
#define IRREGULARPGON_H

#include "diagram.h"

/**
A diagram using irregular pgons. This class implements an algorithm with pgon vertex at the center.

@author Ajit Datar
*/
class IrregularPgon : public Diagram
{
public:
    IrregularPgon();

    virtual ~IrregularPgon();
    virtual void init() { initTrans(); }
    virtual void clear();
    virtual void make();
    //get methods
    virtual DiagramType type() { return IRREGULAR_PGON; }
    //set methods
    virtual void setP(int v);
    virtual void setQ(int vertex, int val) { q_[vertex] = val; }
protected: //methods
    void makeHelper(Exposure exposure, int layerId, Transformation& tran);
    Transformation shiftTran(const Transformation& tran, int shift);
    /**
    Initialize transformations required for this diagram.
    edgeTran transformations are used by drawing algo.
    */
    void initTrans();
    /**initializes the frame for the fundamental pattern */
    void initFrame(double x, Transformation& moveTran);
protected: //data
    vector<int> q_;
    vector<Transformation> edgeTran;
    
    vector<double> cosP_i; //cosines of angles at a vertex
    Pattern pgonPat_;
private:
    double F(double x); //F(x) for newton's method
    double FPrime(double x); //F'(x) for newton's method
};

#endif
