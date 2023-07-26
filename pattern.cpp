/***************************************************************************
 *   Copyright (C) 2005 by Ajit Datar                                      *
 *   ajitdatar@gmail.com                                                   *
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
#include "pattern.h"

//copy constructor --> keep pattern-id the same but duplicate elements
Pattern::Pattern(const Pattern& other)
{
    patId_ = other.patId_;
    elems_ = other.elems_;
    QMap<UId, ElementPtr >::Iterator it;
    for(it = elems_.begin(); it != elems_.end(); ++it) {
        ElementPtr e = it.value();
        //this will do the cloning, but use the original's id
        addElement(e, false);
    }
    
    frame_ = other.frame_;
    for(it = frame_.begin(); it != frame_.end(); ++it) {
        ElementPtr e = it.value();
        //this will do the cloning, but use the original's id
        addElement(e, false, true);
    }
}

//copy assignment --> keep pattern-id the same but duplicate elements
Pattern& Pattern::operator=(const Pattern& other)
{
    if(this != &other) {
        patId_ = other.patId_;
        QMap<UId, ElementPtr >::Iterator it;
        for(it = elems_.begin(); it != elems_.end(); ++it) {
            ElementPtr e = it.value();
            delete e;
        }
        elems_.clear(); //remove existing elements
        elems_ = other.elems_;
        for(it = elems_.begin(); it != elems_.end(); ++it) {
            ElementPtr e = it.value();
            //this will do the cloning, but use the original's id
            addElement(e, false);
        }
        
        frame_.clear(); //remove existing elements
        frame_ = other.frame_;
        for(it = frame_.begin(); it != frame_.end(); ++it) {
            ElementPtr e = it.value();
            //this will do the cloning, but use the original's id
            addElement(e, false, true);
        }
    }
    return *this;
}

Pattern::~Pattern()
{
    QMap<UId, ElementPtr >::Iterator it;
    for(it = elems_.begin(); it != elems_.end(); ++it) {
        ElementPtr e = it.value();
        delete e;
    }
    for(it = frame_.begin(); it != frame_.end(); ++it) {
        ElementPtr e = it.value();
        delete e;
    }
}

ElementPtr Pattern::element(const UId elemId)
{
    //TODO check if elemId exist in elems_
    return elems_[elemId];
}

const ElementPtr Pattern::element(const UId elemId) const
{
    //TODO check if elemId exist in elems_
    return elems_[elemId];
}

UId Pattern::addElement(ElementPtr e, bool cloneId, bool frame) {
    ElementPtr clone = e->clone();
    //whether to use clone's id or original's id
    UId elemId = cloneId ? clone->id() : e->id();
    if(! frame) {
        elems_.insert(elemId, clone);
    }
    else {
        frame_.insert(elemId, clone);
    }
    return elemId;
}

UId Pattern::addElement(const ElementPtr e, const Transformation& tran, bool cloneId, bool frame)
{
    ElementPtr clone = e->clone();
    clone->transform(tran); //transform the clone
    //whether to use clone's id or original's id
    UId elemId = cloneId ? clone->id() : e->id(); 
    if(! frame) {
        elems_.insert(elemId, clone);
    }
    else {
        frame_.insert(elemId, clone);
    }
    return elemId;
}

void Pattern::removeElement(UId elemId, bool frame) {
    ElementPtr e;
    if(! frame) {
        e = elems_[elemId];
        elems_.remove(elemId);
    }
    else {
        e = frame_[elemId];
        frame_.remove(elemId);
    }
    if(e) delete e;
}

PatternPtr Pattern::clone()
{
    PatternPtr clonePat = new Pattern();
    
    QMap<UId, ElementPtr>::Iterator it;

    ElementPtr elem;
    //for all existing elements in fundamental pattern
    for(it = elems_.begin(); it != elems_.end(); ++it) {
        elem = it.value();
        clonePat->addElement(elem);
    }
    for(it = frame_.begin(); it != frame_.end(); ++it) {
        elem = it.value();
        clonePat->addElement(elem, true, true);
    }
    return clonePat;
}

PatternPtr Pattern::cloneAndTransform(const Transformation& trans)
{
    PatternPtr clonePat = new Pattern();
    
    QMap<UId, ElementPtr>::Iterator it;

    ElementPtr elem;
    //for all existing elements in fundamental pattern
    for(it = elems_.begin(); it != elems_.end(); ++it) {
        elem = it.value();
        clonePat->addElement(elem, trans);
    }
    for(it = frame_.begin(); it != frame_.end(); ++it) {
        elem = it.value();
        clonePat->addElement(elem, trans, true, true);
    }
    return clonePat;
}

void Pattern::transform(const Transformation& trans)
{
    QMap<UId, ElementPtr>::Iterator it;

    ElementPtr elem;
    //for all existing elements in fundamental pattern
    for(it = elems_.begin(); it != elems_.end(); ++it) {
        elem = it.value();
        elem->transform(trans);
    }
    for(it = frame_.begin(); it != frame_.end(); ++it) {
        elem = it.value();
        elem->transform(trans);
    }
}

void Pattern::clear() { 
    elems_.clear();
    frame_.clear();
}
