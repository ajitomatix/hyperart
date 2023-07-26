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
#include "diagram.h"

Diagram::Diagram()
{
}

Diagram::~Diagram()
{
//    clear();
}

QColor Diagram::colorMapVal(std::size_t cid) const
{
    //TODO validation
    return colorMap[cid];
}

void Diagram::setP(int v)
{ 
    p_ = v;

    for(int i=0; i< p_; i++) {
        PgonEdge e;
        e.setNumColors(numColors_);
        edges.push_back(e);
    }
}

void Diagram::setNumColors(int v) 
{
    numColors_ = v;
    colorMap.resize(numColors_);
}

void Diagram::setColorMapVal(size_type cid, QColor color)
{
    //TODO validation
    colorMap[cid] = color;
}

void Diagram::clear()
{
    p_ = 0;
    numColors_ = 0;
    fundPat_.clear();
    QMap<PatId, PatternPtr>::iterator it;
    for(it=diag_.begin(); it != diag_.end(); ++it) {
        delete it.value();
    }
    diag_.clear();
    for(unsigned int i=0; i<layers_.size(); i++) {
        layers_[i].clear();
    }
    animq.clear();
    edges.clear();
}

PatternList Diagram::layerPatterns(int layerid)
{
    //TODO do a bounds check
    return layers_[layerid].patterns();
}

void Diagram::addPattern(int layerid, Pattern& pat)
{
    PatternPtr clone = pat.clone();
    PatId id(layerid, clone->id()); //copy assignment of pattern should keep the same id
    diag_[ id ] = clone;
    layers_[layerid].addPattern(clone);
    animq.push_back(id);
}
