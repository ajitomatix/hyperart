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

#ifndef LAYER_H
#define LAYER_H

#include "pattern.h"
#include <QList>

typedef QList<PatternPtr> PatternList;
typedef QList<PatternPtr>::iterator PatternListIter;

/**
Layer is used to group patterns. It is useful to view different layers of the diagram.

@author Ajit Datar
*/
class Layer {
public:
    Layer(int id=0) : layerId_(id) { }
    int id() { return layerId_; }
    void setId(int id) { layerId_ = id; }
    /**
    Returns pattern id after adding a copy of the pattern
    */
    void addPattern(const PatternPtr pat);
    void removePattern(UId patId);
    bool hasPattern(const UId patId) { return patterns_.contains(patId); }
    //TODO check if patId exists
    const PatternPtr pattern(const UId patId) const { return patterns_[patId]; }
    const PatternList patterns() const { return patterns_.values(); }
    void clear();
private:
    QMap<UId, PatternPtr> patterns_;
    int layerId_;
};

#endif
