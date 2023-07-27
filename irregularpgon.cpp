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

#include "irregularpgon.h"
#include "parameters.h"
#include <cmath>
#include <numeric> //for accumulate
using namespace std;

IrregularPgon::IrregularPgon()
 : Diagram()
{
}

IrregularPgon::~IrregularPgon()
{
}

void IrregularPgon::setP(int v)
{
    Diagram::setP(v);
    q_.resize(v); //we now have a vector of q values
}

void IrregularPgon::clear()
{
    Diagram::clear();
    q_.clear();
    pgonPat_.clear();
    edgeTran.clear();
}

//TODO
//Define F(x) and Fprime(x)
double IrregularPgon::F(double x)
{
    double sum = 0.0;
    for(int i=0; i<p_; i++) {
        sum += asin(x * cosP_i[i]);
    }
    sum -= PI;
    return sum;
}

double IrregularPgon::FPrime(double x)
{
    double sum = 0.0;
    for(int i=0; i<p_; i++) {
        sum += cosP_i[i] / sqrt(1 - pow(x * cosP_i[i], 2));
    }
    return sum;
}

void IrregularPgon::initTrans()
{
    int qSum = accumulate(q_.begin(), q_.end(), 0);
    int avgQ = (qSum/p_) + 1;
    cosP_i.resize(p_);
    for(int i=0; i<p_; i++) {
        cosP_i[i] = cos(PI/q_[i]);
    }
    
    double x = sin(PI/p_)/cos(PI/avgQ); //first approximation -- Newton's method
    double xold = x;
    do {
        xold = x;
        x = x - F(x)/FPrime(x);
    }while(fabs(x-xold) > 0.0000000001); //refine till it converges
    
    for(int i=0; i<2; i++) {
        x = x - F(x)/FPrime(x);
    }
    
    double coshEby2 = 1/x; //cosh(E/2)
    double sinhEby2 = sqrt(coshEby2 * coshEby2 - 1);
    double coshE = 2*coshEby2*coshEby2 - 1;
    double sinhE = 2*coshEby2*sinhEby2;
    
    
    Transformation reflectEdgeBisector(numColors());
    {
        Matrix& mat = reflectEdgeBisector.matrix();
        mat[1][1] = -1.0;
        reflectEdgeBisector.setOrient(REFLECTION);
    }

    Transformation reflectPgonEdge(numColors());
    {
        Matrix& mat = reflectPgonEdge.matrix();
        mat[0][0] = -coshE;
        mat[0][2] = sinhE;
        mat[2][0] = -sinhE;
        mat[2][2] = coshE;
    }
    
    Transformation rot2 = reflectPgonEdge * reflectEdgeBisector;
    
    Transformation moveLeft(numColors()), moveLeftInv(numColors());
    {
        Matrix& mat = moveLeft.matrix();
        mat[0][0] = coshEby2;
        mat[0][2] = -sinhEby2;
        mat[2][0] = -sinhEby2;
        mat[2][2] = coshEby2;
    }
    {
        Matrix& mat = moveLeftInv.matrix();
        mat[0][0] = coshEby2;
        mat[0][2] = sinhEby2;
        mat[2][0] = sinhEby2;
        mat[2][2] = coshEby2;
    }
    
    double centerAngle = asin(x*cosP_i[0]);
    double coshHypot = (cos(centerAngle)/sin(centerAngle)) * (cos(PI/q_[0])/sin(PI/q_[0]));
    double sinhHypot = sqrt(coshHypot*coshHypot - 1);
    Point down;
    down.setX(sinhHypot * cos(centerAngle));
    down.setY(sinhHypot * sin(centerAngle));
    down.setW(coshHypot);
    down.setType("weierstrass");
    down.transform(moveLeft);
    
    
    Transformation moveDown(numColors()), moveDownInv(numColors());
    {
        Matrix& mat = moveDown.matrix();
        mat[1][1] = down.w();
        mat[1][2] = -down.y();
        mat[2][1] = -down.y();
        mat[2][2] = down.w();
    }
    {
        Matrix& mat = moveDownInv.matrix();
        mat[1][1] = down.w();
        mat[1][2] = down.y();
        mat[2][1] = down.y();
        mat[2][2] = down.w();
    }
    
    Transformation moveTran = moveDown * moveLeft;
    Transformation moveTranInv = moveLeftInv * moveDownInv;
    
    vector<Transformation> rotp(p_), rotpInv(p_);
    Permutation identity(numColors());
    double ang = 0;
    for(int i=0; i<p_; i++) {
        {
            rotp[i].initIdentity();
            rotp[i].setColorPerm(identity);
            Matrix& mat = rotp[i].matrix();
            mat[0][0] = cos( ang );
            mat[0][1] = -sin( ang );
            mat[1][0] = sin( ang );
            mat[1][1] = cos( ang );
        }
        
        {
            rotpInv[i].initIdentity();
            rotpInv[i].setColorPerm(identity);
            Matrix& mat = rotpInv[i].matrix();
            mat[0][0] = cos( ang );
            mat[0][1] = sin( ang );
            mat[1][0] = -sin( ang );
            mat[1][1] = cos( ang );
        }
        ang += 2 * asin(x*cosP_i[i]);
    }
    
    
    edgeTran.resize(p_);
    for(int i=0; i<p_; i++) {
        int adjEdge = edges[i].adjEdgeId();
        if(REFLECTION == edges[i].orientation()) {
            edgeTran[i] = moveTran * rotp[i] * reflectPgonEdge * rotpInv[adjEdge] * moveTranInv;
        }
        else if(ROTATION == edges[i].orientation() ) {
            edgeTran[i] = moveTran * rotp[i] * rot2 * rotpInv[adjEdge] * moveTranInv;
        }
        else {
            throw "Error: invalid orientation";
        }
        edgeTran[i].setOrient(edges[adjEdge].orientation());
        edgeTran[i].setpPos(adjEdge);
        edgeTran[i].setColorPerm(edges[i].colorPerm());
    }
    initFrame(x,moveTran);
}

void IrregularPgon::initFrame(double x, Transformation& moveTran)
{
    double baseAng = 0.0;
    HyperPoly *hpol = new HyperPoly();
    
    for(int i=0; i<p_; i++) {
        double ang = asin(x*cosP_i[i]);
        double coshHypot = (cos(ang)/sin(ang)) * (cos(PI/q_[i])/sin(PI/q_[i]));
        double sinhHypot = sqrt(coshHypot*coshHypot - 1);
        double u = sinhHypot * cos(baseAng + ang)/(coshHypot+1);
        double v = sinhHypot * sin(baseAng + ang)/(coshHypot+1);
        Point p(u,v);
        cerr<<u<<","<<v<<endl;
        p.poincareToWeierstrass();
        p.transform(moveTran);
        hpol->addPoint(p);
        baseAng += 2 * ang;
    }

    //hpol->addPoint(hpol->getPoint(0));
    hpol->setZOrder(10000);
    hpol->setCid(0);
    hpol->setFilled(false);
    fundPat_.addElement(hpol, true, true);
}

Transformation IrregularPgon::shiftTran(const Transformation& tran, int shift)
{
    int newEdge = (tran.pPos() + tran.orient() * shift + 2*p_) % p_;
    if(newEdge < 0 || newEdge > (p_-1)) {
        throw "RegularPgon::shiftTran newEdge out of range";
    }
    return tran * edgeTran[newEdge];
}

void IrregularPgon::make()
{
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
    layers_.resize(numLayers_);

    pgonPat_ = *(fundPat().clone()); //copy existing set of elements (this will remove the old set)
    addPattern(0, pgonPat_);
    
    if(numLayers_ > 1) {
        Transformation qtran = edgeTran[0];
        for(int i=0; i< q_[0]; i++) {
            makeHelper(MAXEXPOSURE, 1, qtran);
            if( (-1 % p_) != 0) {
                qtran = shiftTran(qtran, -1);
            }
        }
    }
}

void IrregularPgon::makeHelper(Exposure exposure, int layerId, Transformation& tran)
{
    ParametersIRP param(p_);
    
    PatternPtr pat = pgonPat_.cloneAndTransform(tran);
    addPattern(layerId, *pat);
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
            int vert = (ROTATION == ptran.orient()) ? ((ptran.pPos()-1 + p_) % p_) : ptran.pPos();
            int pgonsTodo = q_[vert] - param.pgonsToSkip(exposure, i);
            for(int j=0; j<pgonsTodo; j++) {
                if( (3 == p_) && ( j == (pgonsTodo-1))) {
                    pat = pgonPat_.cloneAndTransform(qtran);
                    addPattern(layerId+1, *pat);
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
