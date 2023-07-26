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

#include "canvaselements.h"
#include <QPolygon>

#include <iostream>
#include <cmath>
using namespace std;

void CanvasPolyLine::paint(QPainter &painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0) {
    painter.setPen( pen() );
    painter.setBrush( Qt::NoBrush );
    painter.drawPolyline(this->polygon());
}

//============================================================================
CanvasPoly::CanvasPoly(bool fill)
    : filled_(fill)
{
}

void CanvasPoly::paint(QPainter &painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0)
{
    painter.setPen( pen() );
    if(filled_) {
       painter.setBrush( brush());
       painter.drawPolygon(this->polygon());
    }
    else {
       painter.setBrush( Qt::NoBrush );
       painter.drawPolygon(this->polygon());
       //painter.drawPolyline(this->polygon());
       //painter.drawLine() (QLineF(this->polygon().last(), this->polygon().first()));
    }
}

//============================================================================

void CanvasEllipse::paint(QPainter &painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0)
{
    painter.setPen( pen() );
    if(filled_) {
        painter.setBrush( brush() );
        painter.drawEllipse(rect());
        //painter.drawEllipse(x(), y(), width(), height());
    }
    else {
        painter.setBrush( Qt::NoBrush );
        //painter.drawArc(x(), y(), width(), height(), 0, 360*16);
        painter.drawArc(rect(), 0, 360*16);
    }
}
//============================================================================

CanvasHyperLine::CanvasHyperLine(const QPointF& topleft, const QSize& size, int startAngle, int alen)
{
    rect_.setTopLeft(topleft);
    rect_.setWidth(size.width());
    rect_.setHeight(size.height());
    angle1_ = startAngle;
    angle2_ = alen;
    approximate_ = false;

    //find out the bounding rectangle using the points in the arc
    //QPolygon points;
    QPainterPath path;
    path.arcTo(topleft.x(), topleft.y(), size.width(), size.height(), angle1_*16, angle2_*16);
    boundingRect_ = path.boundingRect();
}

CanvasHyperLine::CanvasHyperLine(const QPointF& a, const QPointF& b)
{
    a_ = a;
    b_ = b;
    approximate_ = true;
    QPolygon pa(2);
    pa.setPoint(0, a_.toPoint());
    pa.setPoint(1, b_.toPoint());
    boundingRect_ = pa.boundingRect();
}

void CanvasHyperLine::paint(QPainter &painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0)
{
    if(approximate_) {
        painter.drawLine(a_,b_);
    }
    else {
        painter.drawArc(rect_, angle1_*16, angle2_*16);
    }
}

//============================================================================

CanvasHyperPolyLine::~CanvasHyperPolyLine()
{
    lines_.clear();
}

void CanvasHyperPolyLine::addLine(CanvasHyperLine* line) 
{
    lines_.push_back(line);
    if(lines_.size() > 1) {
        boundingRect_ = boundingRect_.united(line->boundingRect());
    }
    else { //first time setting
        boundingRect_ = line->boundingRect();
    }
}

void CanvasHyperPolyLine::paint(QPainter &painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0)
{
    painter.setPen(pen());
    painter.setBrush(Qt::NoBrush);
    vector<CanvasHyperLine*>::iterator iter;
    for(iter = lines_.begin(); iter != lines_.end(); ++iter) {
        (*iter)->paint(painter, option, widget);
    }
}

QPolygonF CanvasHyperPolyLine::areaPoints() const
{
    QPolygonF points(4);
    points << boundingRect_.topLeft()
            << boundingRect_.topRight()
            << boundingRect_.bottomRight()
            << boundingRect_.bottomLeft();
    return points;
}

//============================================================================
void CanvasHyperPoly::addLine(CanvasHyperLine* l) 
{
    CanvasHyperPolyLine::addLine(l);
    //add points of arc/straight line to approximate us with a euclid polygon
    if(l->isApproximated()) {
        points_.putPoints(int(points_.count()), 2, l->a().x(), l->a().y(), l->b().x(), l->b().y());
    }
    else {
        QPolygon pNew;
        QPointF topleft = l->topLeft();
        QSizeF size = l->size();
        int angle1 = l->startAngle(), angle2 = l->lenAngle();
        //QPolygon pOrig;
        QPainterPath path;
        path.arcTo(topleft.x(), topleft.y(), size.width(), size.height(), angle1*16, angle2*16);
        //pOrig.makeArc(topleft.x(), topleft.y(), size.width(), size.height(), angle1*16, angle2*16);
        QPolygon pOrig = path.toFillPolygon().toPolygon();
        unsigned int pointsInSegment = pOrig.count()/8;
        if(pointsInSegment == 0) pointsInSegment=1;
        for(int i=0,j=0; i<pOrig.count(); i += pOrig.count()/pointsInSegment, j++) {
            pNew.putPoints(j, 1, pOrig.point(i).x(), pOrig.point(i).y());
        }
        points_.putPoints(int(points_.count()), pNew.count(), pNew);
    }
}

void CanvasHyperPoly::paint(QPainter &painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0)
{
    painter.setPen(pen());
    painter.setBrush(brush());
    if(filled_)
        painter.drawPolygon(points_);
    else
        painter.drawPolyline(points_);
}
