/***************************************************************************
 *   Copyright (C) 2005 by Dr. Douglas Dunham, Ajit Datar                  *
 *   ddunham@d.umn.edu , data0003@d.umn.edu                                *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
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
