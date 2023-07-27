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

#ifndef PATTERN_H
#define PATTERN_H

#include <QMap>
#include <QList>
#include "defs.h"
#include "element.h"
#include "transformation.h"

typedef QList<ElementPtr> ElemList;
typedef QList<ElementPtr>::iterator ElemListIter;
/**
Pattern is a set of Elements.
It can be used to represent fundametal region or a pgon or layers, basically any collection of Elements.

@author Ajit Datar
*/
class Pattern {
public:
    Pattern() : patId_(IdFactory::getUid()) { }
    Pattern(const Pattern& other);
    Pattern& operator=(const Pattern& other);
    
    ~Pattern();
    
    UId id() const { return patId_; }
    //void setId(UId id) { patId_ = id; }
    
    /**
    Adds a copy of element to the pattern. Returns its id.
    Unless cloneId is false, a new element Id will be used for the copy.
    If frame is true, then the element is considered part of pattern frame.
    */
    UId addElement(const ElementPtr m, bool cloneId=true, bool frame=false);
    /**
    Adds a transformed copy of the element.
    Unless cloneId is false, a new element Id will be used for the copy.
    If frame is true, then the element is considered part of pattern frame.
    */
    UId addElement(const ElementPtr m, const Transformation& tran, bool cloneId=true, bool frame=false);
    /** 
    Removes the element identified by elemId.
    */
    void removeElement(UId elemd, bool frame=false);
    
    /**
    Use this function to pick an element by elemId.
    If your intention is to process all the elements see elems() method, 
    which returns a list of Element pointers.
    */
    ElementPtr element(UId elemId);
    const ElementPtr element(UId elemId) const;
    ElementPtr frameElement(UId elemId) { return frame_[elemId]; }
    const ElementPtr frameElement(UId elemId) const { return frame_[elemId]; }
    /**
    Returns a list of Element pointers. Use ElemListIter to iterate
    through it.
    */
    const ElemList elems() const { return elems_.values(); }
    const ElemList frame() const { return frame_.values(); }
    //removes all elements from the pattern
    void clear();
    PatternPtr clone();
    /**
    Make a clone of this pattern and apply the given transformation to all elements in the clone.
    Returns the transformed clone.
    */
    PatternPtr cloneAndTransform(const Transformation& trans);
    /**
    Applies the transformation to this pattern, pattern is modified.
    */
    void transform(const Transformation& trans);
private:
    QMap<UId, ElementPtr > elems_;
    QMap<UId, ElementPtr> frame_; //bounding hyperlines
    UId patId_; //patternId
};

#endif
