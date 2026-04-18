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

#include "element.h"
#include "transformation.h"

Point::Point()
    : x_(0), y_(0), w_(0), type_("poincare")
{
}

Point::Point(double x, double y)
    : x_(x), y_(y), w_(0), type_("poincare")
{
}

void Point::poincareToWeierstrass()
{
    if("weierstrass" == type_) {
        //qWarning("Point::poincareToWeierstrass() : point is already weierstrass");
        return;
    }
    else if("poincare" != type_) {
        //qWarning("Point::poincareToWeierstrass() : point is not in poincare mode");
        return;
    }
    double s = x_ * x_ + y_ * y_;
    if(1.0 == s) {
        throw "Point::poincareToWeierstrass() : possible divide by zero error";
    }
    x_ = 2.0*x_ / (1.0 - s);
    y_ = 2.0*y_ / (1.0 - s);
    w_ = (1.0 + s) / (1.0 - s);
    type_ = "weierstrass";
}

void Point::weierstrassToPoincare()
{
    if("poincare" == type_) {
        //qWarning("Point::weierstrassToPoincare() : point is already poincare");
        return;
    }
    else if("weierstrass" != type_) {
        //qWarning("Point::weierstrassToPoincare() : point not in weierstrass mode");
        return;
    }
    double denom = 1.0 + w_;
    x_ = x_ / denom;
    y_ = y_ / denom;
    w_ = 0.0;
    type_ = "poincare";
}

void Point::transform(const Transformation& trans)
{
    //TODO trans compatibilty checks
    const Matrix& mat = trans.matrix();
    double x = x_, y = y_, w = w_; //make copy of coordinates
    if ("poincare" == type_ ) {
        x_ = x * mat[0][0] + y * mat[0][1];
        y_ = x * mat[1][0] + y * mat[1][1];
    }
    else if("weierstrass" == type_ ) {
        x_ = x * mat[0][0] + y * mat[0][1] + w * mat[0][2];
        y_ = x * mat[1][0] + y * mat[1][1] + w * mat[1][2];
        w_ = x * mat[2][0] + y * mat[2][1] + w * mat[2][2];
    }
}

Point weierstrassCrossProduct(const Point& p1, const Point p2)
{
    Point r;
    if("weierstrass" != p1.type() || "weierstrass" != p2.type()) {
        qWarning("weierstrassDotProduct : both points should be weierstrass points");
        return r;
    }
    r.setX(p1.y() * p2.w() - p1.w() * p2.y());
    r.setY(p1.w() * p2.x() - p1.x() * p2.w());
    r.setW(-p1.x()* p2.y() + p1.y() * p2.x());
    
    double norm = sqrt(r.x()*r.x() + r.y()*r.y() - r.w()*r.w());
    if(norm == 0.0) {
        throw "weierstrassCrossProduct : possible divide by zero error";
    }
    r.setX(r.x()/norm);
    r.setY(r.y()/norm);
    r.setW(r.w()/norm);
    return r;
}

//============================================================================

Element::Element()
{
    filled_ = false; 
    cid_ = 0;
    open_ = true;
    zorder_ = 1;
    lineStyle_ = SOLID;
    id_ = IdFactory::getUid();
}

/**
    sets an identifier for this element
*/
// void Element::setId(int id) {
//     id_ = id;
// }

Element::~Element()
{
}

void Element::transform(const Transformation& tran)
{
    //first apply transformation to each of the points
    for(size_type i=0; i<numPoints(); i++) {
        points_[i].transform(tran);
    }
    //change elements's color to one suggested by
    //the color permutation
    setCid(tran.colorPerm()[cid()]);
}

EuclidPolyLine::EuclidPolyLine()
    : Element()
{
    open_ = true;
}


EuclidPoly::EuclidPoly() 
    : Element()
{
    open_ = false;
}


Circle::Circle()
    : Element()
{
    open_ = false;
}

HyperLine::HyperLine()
    : startAngle_(0.0), endAngle_(0.0)
{
}

void HyperLine::setPoints(const Point& p1, const Point& p2)
{
    Point s = p1, e = p2;
    s_ = p1; 
    e_ = p2;
    
    s_.weierstrassToPoincare(); //make sure s_ and e_ are in poincare
    e_.weierstrassToPoincare();
    
    //DEBUG
    //cerr<<"HyperLine: ("<<s_.x()<<","<<s.y()<<") - ("<<e_.x()<<","<<e_.y()<<")"<<endl;
    //make sure we are in weierstrass mode
    s.poincareToWeierstrass();
    e.poincareToWeierstrass();
    Point a = weierstrassCrossProduct(s, e);
    
    if(fabs(a.w()) < 1e-6 ) {
        //if the curve is too flat, it's better to approximate with a straight line
        shouldDrawArc_ = false;
        return;
    }
    
    shouldDrawArc_ = true;
    double xc,yc; //center of the arc to be drawn
    xc = a.x() / a.w();
    yc = a.y() / a.w();
    
    //vector to end points of the arc from the center
    double u1,v1,u2,v2;
    u1 = s_.x() - xc;
    v1 = s_.y() - yc;
    u2 = e_.x() - xc;
    v2 = e_.y() - yc;
    double r = sqrt ( (u1 * u1) + (v1 * v1) );
    
    //this makes sure we are always drawing an arc in anticlockwise fashion
    double theta = atan2 ( (v2*u1 - v1*u2), (u1*u2 + v1*v2) );
    
    ////-------uncomment below------ To enable postscript order of arc drawing
    ////----------------------------------------------------------------------
//     double u,v;
//     if ( theta > 0.0 )
//     {
//         u = u1;
//         v = v1; 
//     }
//     else
//     {
//         u = u2;
//         v = v2;
//         theta *= -1;
//     }
    ////-------uncomment above------ To enable postscript order of arc drawing
    ////----------------------------------------------------------------------
    
    startAngle_ = ( atan2 ( v1, u1 ) * 180 / PI );
    endAngle_ = ( theta * 180 / PI );
    ////-------uncomment below------ To enable postscript order of arc drawing
    ////----------------------------------------------------------------------
//     if ( startAngle_ < 0.0 )
//         startAngle_ += 360;
    ////-------uncomment above------ To enable postscript order of arc drawing
    ////----------------------------------------------------------------------
    
    //paramters of the bounding rectangle
    topLeft_.setX(xc-r);
    topLeft_.setY(yc+r);
    width_ = 2*r;
    height_ = 2*r;
    
    //DEBUG
    //cerr<<"HyperLine: "<<xc-r<<","<<yc+r<<","<<r<<","<<startAngle_<<","<<endAngle_<<endl;
}

HyperPolyLine::HyperPolyLine()
    : Element()
{
    open_ = true;
}

void HyperPolyLine::addPoint(Point pt)
{
    Element::addPoint(pt);
    size_type count = numPoints();
    if(count >= 2) {
        HyperLine hline;
        hline.setPoints(getPoint(count-2), getPoint(count-1)); //create a hyperline from last point to current pt
        lines_.push_back(hline); //add it to the lines store.
    }
}

void HyperPolyLine::transform(const Transformation& tran)
{
    Element::transform(tran);
    //rebuild hyperlines
    lines_.clear(); //remove old
    if(numPoints() < 2) {
        throw "HyperPolyLine::transform : Cannot transform an incomplete Hyper polyline";
    }
    for(unsigned int i=1; i<numPoints(); i++) {
        HyperLine hline;
        hline.setPoints(getPoint(i-1), getPoint(i));
        lines_.push_back(hline);
    }
}

HyperPoly::HyperPoly()
    : HyperPolyLine()
{
    open_ = false;
}

void HyperPoly::addPoint(Point pt)
{
    Element::addPoint(pt); //add the new point
    size_type count = numPoints();
    if(count > 3) { //4 or more points exist (including the new point)
        lines_.pop_back(); //remove last added hyperline
        HyperLine hline;
        hline.setPoints(getPoint(count-2), getPoint(count-1)); //hline from last point to new point
        lines_.push_back(hline); //add it to the lines store.
        hline.setPoints(getPoint(count-1), getPoint(0)); //hline from new point to first point
        lines_.push_back(hline); //add it to the lines store.
    }
    else if(count == 3) {
        HyperLine hline;
        hline.setPoints(getPoint(count-2), getPoint(count-1)); //hline from last point to new point
        lines_.push_back(hline); //add it to the lines store.
        hline.setPoints(getPoint(count-1), getPoint(0)); //hline from new point to first point
        lines_.push_back(hline); //add it to the lines store.
    }
    else if(count == 2) { //only two points so far, just add a hyperline
        HyperLine hline;
        hline.setPoints(getPoint(count-2), getPoint(count-1)); //create a hyperline from last point to current pt
        lines_.push_back(hline); //add it to the lines store.
    }
}

void HyperPoly::transform(const Transformation& tran)
{
    HyperPolyLine::transform(tran);
    //join last point to first
    HyperLine hline;
    hline.setPoints(getPoint(numPoints()-1), getPoint(0));
    lines_.push_back(hline);
}
