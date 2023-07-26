//
// C++ Interface: irregularpgon
//
// Description: 
//
//
// Author: Ajit Datar <ajitdatar@gmail.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
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
