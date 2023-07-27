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

#ifndef PGONEDGE_H
#define PGONEDGE_H
#include "defs.h"
#include "permutation.h"

/**
A Pgon edge for the central pgon. It has adjacency info.

@author Ajit Datar
*/
class PgonEdge{
public:
    PgonEdge();

    ~PgonEdge();
    void setNumColors(size_t numColors);

    Orientation orientation() { return orientation_; }
    Permutation& colorPerm() { return perm_; }
    int adjEdgeId() { return adjEdgeId_; }
    
    void setAdjEdgeId(int v) { adjEdgeId_ = v; }
    void setOrientation(Orientation o) { orientation_ = o; }
    
private:
    Orientation orientation_;
    Permutation perm_;
    int adjEdgeId_;
};

#endif
