/***************************************************************************
 *   Copyright (C) 2005 by Dr. Douglas Dunham, Ajit Datar                  *
 *   ddunham@d.umn.edu , data0003@d.umn.edu                                *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/
#ifndef ELEMENT_H
#define ELEMENT_H

#include "defs.h"
#include <string>
#include <QVector>
#include <QColor>
class Transformation;

using namespace std;

typedef vector<QColor> ColorMap;
typedef vector<QColor>::size_type size_type;

/**
Point can be used either as a poincaré point (x,y)
 or as a weierstrass point (x,y,w), or anything other.
*/
class Point {
public:
    Point();
    Point(double x, double y);
    const double x() const { return x_; }
    const double y() const { return y_; }
    const double w() const { return w_; }
    const string type() const { return type_; }
    void setX(const double x) { x_ = x; }
    void setY(const double y) { y_ = y; }
    void setW(const double w) { w_ = w; }
    /** type can be used as an identifier of 
    what this point is being used as. 
    eg poincare/weierstrass or any other use
    */
    void setType(const string& s) { type_ = s; }
    /** 
    convert if it is in poincare mode.
    Do nothing otherwise.
    */
    void poincareToWeierstrass();
    /**
    convert if in weierstrass mode.
    Do nothing otherwise.
    */
    void weierstrassToPoincare();
    /**
    Applies the transformation to current point.
    Transformation should be consistent with the type of point.
    Coordinates of the point are changed according to the tran.
    */
    void transform(const Transformation& trans);

private:
    double x_,y_,w_;
    string type_;
};

/**
Returns a dot product in weierstrass space.
If both p1 and p2 aren't in weierstrass, then a point (0,0,0) is returned.
*/
Point weierstrassCrossProduct(const Point& p1, const Point p2);

/**
An Element is a set of Points which define some geometry. One or more Elements together make up a pattern.  Element is not designed to be used on it's own. Use on of the derived classes instead.

@author Ajit Datar
*/
class Element {
public:
    virtual ~Element();
    /**
        returns if the Element is filled or empty
        An open figure can't be filled, so filled() returns false for open figures
    */
    virtual bool filled() const { return (!open() && filled_); }
    /**
        whether its a closed or an open figure
    */
    virtual bool open() const { return open_; }
    virtual size_type cid() const { return cid_; }
    virtual size_type numPoints() const { return points_.size(); }
    virtual Point getPoint(size_type i) { return points_.at(i); }
    virtual const Point getPoint(size_type i) const { return points_.at(i); }
    virtual ElemType type() const = 0;
    virtual UId id() const { return id_; }
    virtual int zorder() const { return zorder_; }
    virtual int lineStyle() { return lineStyle_; }
    /**
    Applies the transformation to the element, changing its state.
    Transformation should be consistent with the type of points
    in the element.
    */
    virtual void transform(const Transformation& tran);
    
    //set color id
    virtual void setCid(size_type c) { cid_ = c; }
    virtual void addPoint(Point pt) { points_.push_back(pt); }
    virtual void setFilled(bool f) { filled_ = f; }
    //virtual void setId(UId id);
    virtual void setZOrder(int z) { zorder_ = z; }
    virtual void setLineStyle(LineStyle ls) { lineStyle_ = ls; }
    /**
    create a copy of self on heap and return
    */
    virtual ElementPtr clone() = 0;
protected:
    Element();
    QVector<Point> points_;
    size_type cid_;
    bool filled_;
    bool open_;
    UId id_;
    //determines which element obscures which other elements
    //a higher zorder obscures a lower
    int zorder_;
    LineStyle lineStyle_; 
};

class EuclidPolyLine : public Element {
public:
    EuclidPolyLine();
    ElemType type() const { return EUCLID_POLYLINE; }
    virtual ElementPtr clone() { 
        EuclidPolyLine* e = new EuclidPolyLine(*this); 
        e->id_ = IdFactory::getUid();
        return ElementPtr(e);
    }
    virtual ~EuclidPolyLine() { };
};

class EuclidPoly : public Element {
public:
    EuclidPoly();
    ElemType type() const { return EUCLID_POLY; }
    virtual ElementPtr clone() { 
        EuclidPoly* e = new EuclidPoly(*this); 
        e->id_ = IdFactory::getUid();
        return ElementPtr(e);
    }
    virtual ~EuclidPoly() { }
};

class Circle : public Element {
public:
    Circle();
    ElemType type() const { return CIRCLE; }
    virtual ElementPtr clone() { 
        Circle* e = new Circle(*this); 
        e->id_ = IdFactory::getUid();
        return ElementPtr(e);
    }
    virtual ~Circle() { }
};

/**
Helper class for Hyperbolic figures. 
Almost all hyperbolic figures like hyperpolyline and hyperpoly need hyperlines to
construct themselves.

Note: This class is NOT derived from Element. So to actually construct a hyperline
use HyperPolyLine with 2 points.
*/
class HyperLine {
public:
    HyperLine();
    void setPoints(const Point& p1, const Point& p2);
    const Point& topLeft() const { return topLeft_; }
    const double width() const { return width_; }
    const double height() const { return height_; }
    const double startAngle() const { return startAngle_; }
    const double endAngle() const { return endAngle_; }
    const Point& startPoint() const { return s_; }
    const Point& endPoint() const { return e_; }
    /**
    After calling setPoints(), use other get methods only if this method returns true.
    If shouldDrawArc is false, then hyperline should be approximated using a straight line instead of
    an arc.
    */
    const bool shouldDrawArc() const { return shouldDrawArc_; }
private:
    Point s_, e_; //start and end points
    //top-left of the bounding rect for the arc that represents the hyperline
    Point topLeft_;
    //size of the bounding rect for the arc that represents the hyperline
    double width_, height_;
    double startAngle_, endAngle_;
    bool shouldDrawArc_;
};

/**
After adding all the points, use hyperLines() to get the parameters
for drawing hyperlines.
*/
class HyperPolyLine : public Element {
public:
    HyperPolyLine();
    virtual ElemType type() const { return HYPER_POLYLINE; }
    virtual ElementPtr clone() { 
        HyperPolyLine* e = new HyperPolyLine(*this); 
        e->id_ = IdFactory::getUid();
        return ElementPtr(e);
    }
    virtual void addPoint(Point pt);
    virtual vector<HyperLine>& hyperLines() { return lines_; }
    virtual void transform(const Transformation& tran);
    virtual ~HyperPolyLine() { }
protected:
    vector<HyperLine> lines_;
};

class HyperPoly : public HyperPolyLine {
public:
    HyperPoly();
    virtual ElemType type() const { return HYPER_POLY; }
    virtual ElementPtr clone() { 
        HyperPoly* e = new HyperPoly(*this); 
        e->id_ = IdFactory::getUid();
        return ElementPtr(e);
    }
    virtual void addPoint(Point pt);
    virtual void transform(const Transformation& tran);
    ~HyperPoly() { }
};

#endif
