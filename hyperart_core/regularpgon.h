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

#ifndef REGULARPGON_H
#define REGULARPGON_H

#include "diagram.h"

/**
A diagram which is based on regular pgons. This constitute the original Escher circle limit I-IV patterns.

@author Ajit Datar
*/
class RegularPgon : public Diagram {
public:
    RegularPgon();

    virtual ~RegularPgon();
    virtual void init();
    virtual void clear();
    virtual void make();
    //get methods
    virtual DiagramType type() { return REGULAR_PGON; }
    virtual ReflSymType reflSym() { return reflSym_; }
    virtual Permutation& reflColorPerm() { return reflColorPerm_; }
    virtual Permutation& rotnColorPerm() { return rotnColorPerm_; }
    virtual int fundRegEdges() const { return fundRegEdges_; }
    //set methods
    virtual void setQ(int v) { q_ = v; }
    virtual void setReflSym(ReflSymType v) { reflSym_ = v; }
    virtual void setNumColors(int v);
    void setFundRegEdges(int v) { fundRegEdges_ = v; }
protected: //methods
    /**
    Makes the central pgon.
    In regular pgon case fundamental pattern is reflected and rotated to
    generate a pgonPat_ (which is in the central pgon)
    */
    virtual void makePgonPat();
    void makeHelper(Exposure exposure, int layerId, Transformation& tran);
    Transformation shiftTran(const Transformation& tran, int shift);
    /**
    Initialize transformations required for this diagram.
    edgeTran transformations are used by drawing algo.
    */
    void initTrans();
protected: //data
    int q_;
    Pattern pgonPat_; //pgon pattern
    int fundRegEdges_;
    ReflSymType reflSym_;
    Permutation reflColorPerm_; 
    Permutation rotnColorPerm_;
    
    double x2pt, xqpt, yqpt; 
    
    Transformation reflectHypot, reflectPgonEdge, reflectEdgeBisector;
    Transformation rot2;
    vector<Transformation> rotp, rotpInv;
    vector<Transformation> edgeTran; //transformation associated with each pgon edge
};

#endif
