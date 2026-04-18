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

#ifndef DEFS_H
#define DEFS_H

//This file defines various enumerations and constants

#include <map>
#include <vector>
#include <cmath>
using namespace std;

enum ReflSymType {
    REFL_NONE = 0,
    REFL_EDGE_BISECTOR = 1,
    REFL_PGON_RADIUS = 2
};

//in the old code -ve values meant reflection and +ve meant rotation
enum Orientation {
    REFLECTION = -1,
    ROTATION = 1
};

enum Exposure {
    MINEXPOSURE,
    MAXEXPOSURE
};

//used for runtime type identification
enum ElemType {
    ELEMENT = 0,
    EUCLID_POLYLINE,
    EUCLID_POLY,
    CIRCLE,
    HYPER_POLYLINE,
    HYPER_POLY
};

enum ZoomType {
    IN = -1,
    OUT = 1,
    DEFAULT = 2
};

enum PanType {
    PAN_LEFT,
    PAN_RIGHT,
    PAN_UP,
    PAN_DOWN
};

enum ViewMode {
    NORMAL, //no animation, no editing
    ANIMATE,
    EDIT
};

enum DiagramType {
    DIAGRAM = 0, //abstract diagram 
    REGULAR_PGON,
    IRREGULAR_PGON
    //etc
};

enum LineStyle {
    SOLID = 0,
    DOTS
};

const double PI = 3.14159265359;

typedef unsigned int UId; //app level unique id

//app level unique ids
class IdFactory {
public:
    //get the next unique integer id
    static UId getUid();
private:
    static UId currentUid_;
};

class Element;
typedef Element* ElementPtr;

class Pattern;
typedef Pattern* PatternPtr;

typedef pair<int, UId> PatId; //ientifies a pattern by it's layer number and pattern UId
#endif
