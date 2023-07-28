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

#ifndef DIAGRAM_H
#define DIAGRAM_H

#include <QColor>

#include "defs.h"
#include "pattern.h"
#include "layer.h"
#include "pgonedge.h"

/**
Diagram provides a common interface for different types of diagrams (regular pgon, irregular pgon).
Each type of Diagram should be a derived class of this.

@author Ajit Datar
*/
class Diagram  {
public:
    Diagram();

    virtual ~Diagram();
    
    /**
    Init should be called whenever a new data is fed to the object.
    Eg. The DataReader would do so after successfully reading
    input file.
    */
    virtual void init() = 0;

    /**
    Clears the data stored in the object, making it available for
    feeding new data.
    Eg. The DataReader would call this before reading in new file.
    */
    virtual void clear();
    
    /**
    This is where the diagram generation algorithm goes.
    It should implement the Pattern replication algorithm.
    */
    virtual void make()=0;
    
    /**
    Dervied classes should provide their own type
    */
    virtual DiagramType type() { return DIAGRAM; }
    
    //get methods
    virtual int p() { return p_; }
    virtual QColor colorMapVal(std::size_t cid) const;
    virtual int numLayers() { return numLayers_; }
    virtual Pattern& fundPat() { return fundPat_; }
    int numColors() const { return numColors_; }

    virtual PatternPtr pattern(PatId id) { return diag_[ id ]; }
    /**
    Get all the pattern in the diagram
    */
    virtual PatternList allPatterns() { return diag_.values(); }
    /**
    Get all the patterns in a layer
    */
    virtual PatternList layerPatterns(int layerid); 

    //set methods
    virtual void setP(int v);
    virtual void setNumColors(int v);
    virtual void setColorMapVal( size_type cid, QColor color);
    virtual void setNumLayers(int la) { numLayers_ = la; }
protected: //methods
    /**
    Adds the pattern to diagram. Pattern is added to layer 'layerid'
    */
    void addPattern(int layerid, Pattern& pat);

public: //data
    vector<PgonEdge> edges;
    /** 
    maintains patterns in the order in which they were created.
    used by the step thru animation.
    */
    vector< PatId > animq;
protected: //data
    QString fileName_;
    int p_;
    
    int numColors_;
    Pattern fundPat_; //fundametal pattern
    
    //setNumColors has to be called to initialize these
    ColorMap colorMap;
    
    /**
    Number of Layers to generate
    */
    int numLayers_ = 4;
    /**
    diag_ is collection of all the patterns.
    */
    QMap< PatId, PatternPtr> diag_;
    vector< Layer > layers_; 
};

#endif
