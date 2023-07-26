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

#ifndef CANVASELEMENTS_H
#define CANVASELEMENTS_H

#include <QGraphicsScene>
#include <QPainter>
#include <QPoint>
#include <QSize>
#include <QPolygon>
#include <QGraphicsPolygonItem>
#include "defs.h"

/**
Declares various scene elements used by PoincareView.
These are essentially mappings from corresponding Element to its
screen representation on a QCanvas
*/

/**
A CanvasPolyLine is a Euclid polyline.
Derived from QGraphicsPolygonItem.
Use setPoints to set polyline points. setPen to set the pen.
Brush is not used.
*/
class CanvasPolyLine : public QGraphicsPolygonItem
{
public:
    virtual int rtti() const { return EUCLID_POLYLINE; }
    virtual void paint(QPainter &painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
};

//===========================================================================

class CanvasPoly : public QGraphicsPolygonItem
{
public:
    CanvasPoly(bool fill=true);
    virtual int rtti() const { return EUCLID_POLY; }
    virtual bool isFilled() { return filled_; }
    virtual void setFilled(bool f) { filled_ = f; }
    virtual void paint(QPainter &painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
protected:
    bool filled_;
};

//===========================================================================

class CanvasEllipse : public QGraphicsEllipseItem
{
public:
    CanvasEllipse(bool fill=true) : filled_(fill) { }
    CanvasEllipse(QRectF rect) : QGraphicsEllipseItem(rect), filled_(true) { }
    virtual int rtti() const { return CIRCLE; }
    virtual bool isFilled() { return filled_; }
    virtual void setFilled(bool f) { filled_ = f; }
    virtual void paint(QPainter &painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
protected:
    bool filled_;
};
//===========================================================================

/**
A Hyperbolic Line. 
Makes sense only on a poincare or other hyperbolic models.
*/
class CanvasHyperLine
{
public:
    CanvasHyperLine(const QPointF& topleft, const QSize& size, int startAngle, int alen);
    CanvasHyperLine(const QPointF& a, const QPointF& b);
    virtual QRectF boundingRect() const { return boundingRect_; }
    bool isApproximated() { return approximate_; }
    //make sense if isApproximated()
    QPointF a() { return a_; }
    QPointF b() { return b_; }
    //make sense if not isApproximated()
    QPointF topLeft() { return rect_.topLeft(); }
    QSizeF size() { return rect_.size(); }
    int startAngle() { return angle1_; }
    int lenAngle() { return angle2_; }
    virtual void paint(QPainter &painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
protected:
    QRectF rect_; //the ellipse for drawing this arc
    int angle1_, angle2_;
    QPointF a_, b_; //if approximated using a straight line
    bool approximate_;
    QRectF boundingRect_;
};

//===========================================================================

/**
It is like a Euclid Polyline but individual lines are
hyperbolic lines in the poincare disk.
*/
class CanvasHyperPolyLine : public QGraphicsPolygonItem
{
public:
    virtual ~CanvasHyperPolyLine();
    void addLine(CanvasHyperLine* line);
    virtual int rtti() const { return HYPER_POLYLINE; }
    virtual QPolygonF areaPoints() const;
    /**
    Doesn't make sense to use this without calling setLines() method first
    */
    virtual QRectF boundingRect() const { return boundingRect_; }
    virtual void paint(QPainter &painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
 protected:
    vector<CanvasHyperLine*> lines_;
    QRectF boundingRect_;
};

//===========================================================================

/**
It is like a Euclid Poly but polygon edges are
hyperbolic lines in the poincare disk
*/
class CanvasHyperPoly : public CanvasHyperPolyLine
{
public:
    CanvasHyperPoly(bool fill=true) : filled_(fill) { }
    virtual ~CanvasHyperPoly() { }
    virtual int rtti() const { return HYPER_POLY; }
    bool isFilled() { return filled_; }
    void addLine(CanvasHyperLine* line);
    void setFilled(bool f) { filled_ = f; }
    virtual void paint(QPainter &painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
protected:
    QPolygon points_;
    bool filled_;
};
//===========================================================================

#endif
