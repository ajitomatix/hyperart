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

#ifndef PARAMETERS_H
#define PARAMETERS_H

#include "defs.h"

/**
Get different drawing parameters based on values in Diagram.

@author Ajit Datar
*/
class Parameters{
public:
    Parameters(int p, int q);
    
    /**
    Returns exposure (min or max) based on the 3 parameters.
    layer : 0 or more
    vertexNum : 0 to p-1
    pgonNum : 0 to q-1
    
    layer == 0 is special case where value of vertexNum is irrelevant
    */
    virtual Exposure exposure(int layer, int vertexNum, int pgonNum);
    
    /**
    Returns how many vertices to skip.
    Used by drawing algo.
    */
    virtual int pSkip(Exposure e);
    
    /**
    Returns how many pgons to skip.
    Used by drawing algo.
    */
    virtual int qSkip(Exposure e, int vertexNum);
    
    /**
    Returns how many vertices to process for given exposure.
    Used by drawing algo.
    */
    virtual int verticesTodo(Exposure e);
    
    /**
    Returns how many pgons to draw for given exposure and vertex.
    Used by the drawing algo.
    */
    virtual int pgonsTodo(Exposure e, int vertexNum);
    
    //set functions
    void setP(int val) { p_ = val; }
    void setQ(int val) { q_ = val; }
    virtual ~Parameters();
private:
    int p_, q_;
};

/**
Parameters for 'IrregularPGon with vertex at center' algorithms
*/
class ParametersIRP : protected Parameters
{
public:
    /**
    We set the value of q_ in the base class such that parameter values for general case are returned.
    Here it is arbitrarily set to 5 to avoid special cases like p ==3 or q == 3 
    */
    ParametersIRP(int p) : Parameters(5,5) { p_ = p; }
    virtual ~ParametersIRP() {}
    
    virtual Exposure exposure(int layer, int vertexNum, int pgonNum) { return Parameters::exposure(layer, vertexNum, pgonNum); }
    virtual int pSkip(Exposure e) { return Parameters::pSkip(e); }
    virtual int qSkip(Exposure e, int vertexNum) { return Parameters::qSkip(e,vertexNum); }
    virtual int verticesTodo(Exposure e);
    virtual int pgonsToSkip(Exposure e, int vertexNum);
private:
    int p_;
};

#endif
