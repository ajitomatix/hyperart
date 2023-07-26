/***************************************************************************
 *   Copyright (C) 2005 by Ajit Datar, Dr. Douglas Dunham   *
 *   ajitdatar@gmail.com, ddunham@d.umn.edu   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
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
#include "regularpgon.h"
#include "parameters.h"

RegularPgon::RegularPgon()
 : Diagram()
{
}


RegularPgon::~RegularPgon()
{
}

void RegularPgon::setNumColors(int v) 
{
    Diagram::setNumColors(v);
    rotnColorPerm_.setSize(numColors());
    reflColorPerm_.setSize(numColors());
}

void RegularPgon::clear()
{
    Diagram::clear();
    
    q_ = 0;
    pgonPat_.clear();
    
    rotp.clear();
    rotpInv.clear();
    edgeTran.clear();
}

void RegularPgon::init()
{
    initTrans();
}

void RegularPgon::initTrans()
{
    double  sinp, cosp;
    double  cos2p, cosh2q;
    double  sin2p, sinh2q;
    double  sinh2, sinhq;
    double  cosh2, coshq;
    double  rad2;
    
    cosp = cos(PI/p_);
    sinp = sin(PI/p_);
    cos2p = cos(2 * PI/p_);
    sin2p = sin(2 * PI/p_);
    coshq = cos(PI/q_) / sinp;
    sinhq = sqrt(coshq * coshq - 1);
    cosh2q = 2 * coshq * coshq - 1;
    sinh2q = 2 * sinhq * coshq;
    cosh2 = 1 / ((sin(PI/p_) / cos(PI/p_)) * (sin(PI/q_) / cos(PI/q_)));
    sinh2 = sqrt(cosh2 * cosh2 - 1);
    rad2 = sinh2 / (cosh2 + 1);
    x2pt = sinhq / (coshq + 1);
    xqpt = cosp * rad2;
    yqpt = sinp * rad2;
    
    //get ourself an identity permutation for numcolors in the current document
    //This is because before we load document we don't know the number of colors
    Permutation identity(numColors());
    
    {
        reflectHypot.initIdentity();
        reflectHypot.setColorPerm(identity);
        Matrix& mat = reflectHypot.matrix();
        mat[0][0] = cos2p;
        mat[0][1] = sin2p;
        mat[1][0] = sin2p;
        mat[1][1] = -cos2p;
        reflectHypot.setOrient(REFLECTION);
    }
    
    {
        reflectPgonEdge.initIdentity();
        reflectPgonEdge.setColorPerm(reflColorPerm());
        Matrix& mat = reflectPgonEdge.matrix();
        mat[0][0] = -cosh2q;
        mat[0][2] = sinh2q;
        mat[2][0] = -sinh2q;
        mat[2][2] = cosh2q;
        reflectPgonEdge.setOrient(REFLECTION);
    }
    
    {
        reflectEdgeBisector.initIdentity();
        reflectEdgeBisector.setColorPerm(reflColorPerm());
        Matrix& mat = reflectEdgeBisector.matrix();
        mat[1][1] = -1.0;
        reflectEdgeBisector.setOrient(REFLECTION);
    }
    
    rot2 = reflectPgonEdge * reflectEdgeBisector;
    
    //setup rotp, rotpInv
    //rotp and rotpInv define transformations across
    //each of the p edges
    rotp.resize(p_);
    rotpInv.resize(p_);
    for(int i=0; i<p_; i++) {
        {
            rotp[i].initIdentity();
            rotp[i].setColorPerm(rotnColorPerm());
            Matrix& mat = rotp[i].matrix();
            mat[0][0] = cos( 2*i*PI / p_ );
            mat[0][1] = -sin( 2*i*PI / p_ );
            mat[1][0] = sin( 2*i*PI / p_ );
            mat[1][1] = cos( 2*i*PI / p_ );
        }
        
        {
            rotpInv[i].initIdentity();
            rotpInv[i].setColorPerm(rotnColorPerm());
            Matrix& mat = rotpInv[i].matrix();
            mat[0][0] = cos( 2*i*PI / p_ );
            mat[0][1] = sin( 2*i*PI / p_ );
            mat[1][0] = -sin( 2*i*PI / p_ );
            mat[1][1] = cos( 2*i*PI / p_ );
        }
    }
    
    edgeTran.resize(p_);
    for(int i=0; i<p_; i++) {
        int adjEdge = edges[i].adjEdgeId();
        if(REFLECTION == edges[i].orientation()) {
            edgeTran[i] = rotp[i] * reflectPgonEdge * rotpInv[adjEdge];
        }
        else if(ROTATION == edges[i].orientation() ) {
            edgeTran[i] = rotp[i] * rot2 * rotpInv[adjEdge];
        }
        else {
            throw "Error: invalid orientation";
        }
        edgeTran[i].setOrient(edges[adjEdge].orientation());
        edgeTran[i].setpPos(adjEdge);
        edgeTran[i].setColorPerm(edges[i].colorPerm());
    }
}

void RegularPgon::makePgonPat()
{
    //First add a frame for the fundamental pattern
    Point fundFramePt1(xqpt,yqpt),fundFramePt2(xqpt,-yqpt),fundFramePt3(0,0),fundFramePt4(x2pt,0);
    fundFramePt1.poincareToWeierstrass();
    fundFramePt2.poincareToWeierstrass();
    fundFramePt3.poincareToWeierstrass();
    fundFramePt4.poincareToWeierstrass();
    HyperPolyLine* hp1 = new HyperPolyLine();
    hp1->addPoint(fundFramePt1);
    hp1->addPoint(fundFramePt2);
    hp1->setZOrder(10000);
    hp1->setCid(0);
    fundPat_.addElement(hp1,true,true);
    delete hp1;
    
//     //This is internal frame -- commented to give a clearer view of the bounding frame
//     HyperPolyLine* hp2 = new HyperPolyLine();
//     hp2->addPoint(fundFramePt1);
//     hp2->addPoint(fundFramePt3);
//     hp2->addPoint(fundFramePt2);
//     hp2->setZOrder(10000);
//     hp2->setCid(0);
//     hp2->setLineStyle(DOTS);
//     fundPat_.addElement(hp2,true,true);
//     delete hp2;

//     HyperPolyLine* hp3 = new HyperPolyLine();
//     hp3->addPoint(fundFramePt3);
//     hp3->addPoint(fundFramePt4);
//     hp3->setZOrder(10000);
//     hp3->setCid(0);
//     hp3->setLineStyle(DOTS);
//     fundPat_.addElement(hp3,true,true);
//     delete hp3;
    
    //TODO write makePgonPat using Pattern::cloneAndTransform
    addPattern(0, fundPat());
    pgonPat_ = *(fundPat().clone()); //copy existing set of elements (this will remove the old set)
    
    //This function does what genpgonpat used to do in old code
    //ie. internal replication of fundamental pattern
    if(REFL_PGON_RADIUS == reflSym() || REFL_EDGE_BISECTOR == reflSym()) {
        Transformation reflect;
        if(REFL_PGON_RADIUS == reflSym())
            reflect = reflectHypot;
        else
            reflect = reflectEdgeBisector;
        //NOTE reflectEdgeBisector and reflectPgonEdge already have
        //reflColorPerm_ as their color permutation
        
        //now apply reflection transformation to each existing element
        //and put the new element in pgon_
        ElementPtr elem;
        ElemList elems = fundPat().elems();
        //for all existing elements in fundamental pattern
        for(ElemListIter it = elems.begin(); it != elems.end(); ++it) {
            elem = *it;
            pgonPat_.addElement(elem, reflect); //and add it to our pattern
        }
        ElemList frame = fundPat().frame();
        //for all existing elements in the frame of fundamental pattern
        for(ElemListIter it = frame.begin(); it != frame.end(); ++it) {
            elem = *it;
            pgonPat_.addElement(elem, reflect, true, true); //and add it to our pattern
        }
        addPattern(0, pgonPat_);
        //DEBUG
        //cerr<<"Reflected!"<<endl;
    }
    //we are done with reflection now rotate all elements in pgonPat_ to generate
    //the complete pattern
    //we use rotp to change rotateCenter in the loop
    //NOTE rotp and rotpInv already have
    //rotnColorPerm_ as their color permutation
    
    ElemList elems = pgonPat_.elems();
    ElemList frame = pgonPat_.frame();
    
    ElementPtr elem;
    int numRotations = p_ / fundRegEdges_;
    Transformation rotateCenter(numColors()); //start with an identity transformation
    for(int r=1; r<numRotations; ++r) { //if more than 1 rotation
        rotateCenter *= rotp[fundRegEdges_]; //increment the transform by rotp
        //for all existing elements in fundamental pattern
        for(ElemListIter it = elems.begin(); it != elems.end(); ++it) {
            elem = *it;
            pgonPat_.addElement(elem, rotateCenter);
        }
        //for all existing elements in the frame of fundamental pattern
        for(ElemListIter it = frame.begin(); it != frame.end(); ++it) {
            elem = *it;
            pgonPat_.addElement(elem, rotateCenter, true, true); //and add it to our pattern
        }
        addPattern(0, pgonPat_);
    }
}

Transformation RegularPgon::shiftTran(const Transformation& tran, int shift)
{
    int newEdge = (tran.pPos() + tran.orient() * shift + 2*p_) % p_;
    if(newEdge < 0 || newEdge > (p_-1)) {
        throw "RegularPgon::shiftTran newEdge out of range";
    }
    return tran * edgeTran[newEdge];
}

void RegularPgon::make()
{
    //clear all the patterns
    pgonPat_.clear();
    
    QMap<PatId, PatternPtr>::iterator it;
    for(it=diag_.begin(); it != diag_.end(); ++it) {
        delete it.value();
    }
    diag_.clear();
    for(unsigned int i=0; i<layers_.size(); i++) {
        layers_[i].clear();
    }
    animq.clear();

    //and start generating new
    layers_.resize(numLayers_);
    Parameters param(p_,q_);
    makePgonPat();
    //addPattern(0, pgonPat_); //0th layer always has pgonPat as the only pattern (in regular case)
    
    if(numLayers_ > 1) {
        for(int i=0; i < p_; i++) {
            Transformation qtran = edgeTran[i];
            for(int j=0; j < (q_ -2); j++) {
                if( (3 == p_) && ((q_-3 ) == j)) {
                    PatternPtr pat = pgonPat_.cloneAndTransform(qtran);
                    addPattern(1, *pat);
                }
                else {
                    makeHelper(param.exposure(0, i, j), 1, qtran);
                }
                if( (-1 % p_) != 0) {
                    qtran = shiftTran(qtran, -1);
                }
            }
        }
    }
}

void RegularPgon::makeHelper(Exposure exposure, int layerId, Transformation& tran)
{
    Parameters param(p_,q_);
    PatternPtr pat = pgonPat_.cloneAndTransform(tran);
    addPattern(layerId, *pat);
    //delete pat;
    if(layerId < numLayers_ -1) {
        int pskip = param.pSkip(exposure);
        int verticesTodo = param.verticesTodo(exposure);
        for(int i=0; i< verticesTodo; i++) {
            //setptran
            Transformation ptran = shiftTran(tran, pskip);
            Transformation qtran(numColors());
            //set qtran based on ptran
            int qskip = param.qSkip(exposure,i);
            if( (qskip % p_) != 0) {
                    qtran = shiftTran(ptran, qskip);
            }
            else {
                qtran = ptran;
            }
            int pgonsTodo = param.pgonsTodo(exposure, i);
            for(int j=0; j<pgonsTodo; j++) {
                if( (3 == p_) && ( j == (pgonsTodo-1))) {
                    pat = pgonPat_.cloneAndTransform(qtran);
                    addPattern(layerId, *pat);
                }
                else {
                    makeHelper(param.exposure(layerId, i, j), layerId+1, qtran);
                }
                if( (-1 % p_) != 0) {
                    qtran = shiftTran(qtran, -1);
                } //else don't change qtran
            }
            pskip = (pskip +1)% p_;
        }
    }
}
