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

#include "poincareview.h"
#include <QFileInfo>
#include <QPaintDevice>
#include <QPixmap>
#include <QPolygon>
#include "QtGui/qevent.h"
#include "diagram.h"

PoincareView::PoincareView(QWidget* parent, Qt::WindowFlags f)
 : QGraphicsView(parent), viewMode_(NORMAL), showFrame_ (true)
{
    scene_ = new QGraphicsScene(parent->rect(), this);
    qDebug() << "Scene Rect: " << scene_->sceneRect();
    this->setScene(scene_);

    // TODO: Set up timer when animation is implemented
    //    animateTimer = new QTimer(this);
    //    connect(animateTimer, SIGNAL(timeout()), this, SLOT(animateTimerDone()));
}

PoincareView::~PoincareView()
{
    delete scene_;
    delete animateTimer;
}

void PoincareView::resizeEvent(QResizeEvent *event)
{
    qDebug() << "Old size: " << event->oldSize();
    qDebug() << "New size: " << event->size();
    qDebug() << "View size: " << size();


    diameter_ = height() < width() ? height() : width();
    origin_ = QPoint(width()/2, height() / 2);
    setSceneRect(0, 0, width(), height());
    fitInView(sceneRect());

    scene()->clear();
    drawBoundingCircle(true,true);

    if(dgram) {
        drawDiagram(true, true);
//        drawFrame(showFrame_,true);
    }

    QGraphicsView::resizeEvent(event);
}

void PoincareView::documentChanged(Diagram* diagram)
{
    dgram = diagram;
    dgram->make();

    init();
    drawBoundingCircle(true,true);
    drawDiagram(true,true);
//    drawFrame(showFrame_,true);
}

void PoincareView::init()
{
    animDelay_ = 200;
    if(animateTimer) {
        animateTimer->stop();
        animateNext_ = 0;
        paused_ = false;
    }

    items_.clear(); //clear our item list

    scene()->clear();

    viewMode_ = NORMAL;
}

void PoincareView::drawBoundingCircle(bool visible, bool init)
{
    if(init) {
        qDebug() << "Poincare Bounding Circle\n diameter: " << diameter() << "; origin: " << origin();
        auto rect = QRectF(QPointF(0,0), QSizeF(diameter(), diameter()));
        rect.moveCenter(origin());
        disk = new QGraphicsEllipseItem(rect);
        qDebug() << "Disk bounding rect: " << disk->rect();
        disk->setBrush(QColor(224, 224, 224));
        disk->setZValue(-100);
    }
    disk->setVisible(visible);
    scene()->addItem(disk);
}


float PoincareView::diameter()
{
    return diameter_;
}

QPoint PoincareView::origin()
{
    return origin_;
}

void PoincareView::drawDiagram(bool visible, bool init)
{
    if(dgram == nullptr)
        return;

    for(int i=0; i<dgram->numLayers(); i++) {
        drawLayer(i, visible, init);
    }
}

void PoincareView::drawLayer(const int layerId, bool visible, bool init)
{
    PatternList patterns = dgram->layerPatterns(layerId);
    qDebug() << "Rendering" << patterns.count() << "patterns in layer" << layerId;
    PatternListIter it;
    for(it = patterns.begin(); it != patterns.end(); ++it) {
        drawPattern(**it, visible, init);
    }
    isLayerVisible[layerId] = visible;
}

void PoincareView::drawPattern(const Pattern& pat, bool visible, bool init)
{
    ElemList elems = pat.elems();
    //DEBUG
    qDebug() << "Rendering" << elems.count() << "elements in pattern" << pat.id();
    for(ElemListIter it = elems.begin(); it != elems.end(); ++it) {
        drawElement(*it, visible, init);
    }
}

void PoincareView::drawPatternFrame(const Pattern& pat, bool visible, bool init)
{
    ElemList frame = pat.frame();
    for(ElemListIter it = frame.begin(); it != frame.end(); ++it) {
        drawElement(*it, visible, init);
    }
}

void PoincareView::drawElement(const ElementPtr e, bool visible, bool init)
{
    if(! init) { //only show/hide existing
        if(items_.contains(e->id())) {
            items_[e->id()]->setVisible(visible);
        }
        return;
    }

    QPen pen(dgram->colorMapVal(e->cid()));
    if(e->lineStyle() == DOTS) {
        pen.setStyle(Qt::DotLine);
    }

    //Initialization
    //TODO handle filled/not-filled, (assumes filled for now)
    if(CIRCLE == e->type()) {
        //first point is the center and second is a point on the circumference
        //calculate radius and draw
        Point center = e->getPoint(0);
        Point circum = e->getPoint(1);
        QPointF p1 = makeQPointF(center);
        QPointF p2 = makeQPointF(circum);
        double radius = sqrt(pow(double(p1.x() - p2.x()), 2) + pow(double(p1.y() - p2.y()), 2));
        CanvasEllipse* circle = new CanvasEllipse(QRectF(p1.x() - radius, p1.y() - radius, 2*radius, 2*radius));
        circle->setZValue(e->zorder());
        circle->setPen(pen);
        circle->setBrush(dgram->colorMapVal(e->cid()));
        circle->setFilled(e->filled());
        circle->setVisible(visible);
        items_.insert(e->id(), circle);
        scene()->addItem(circle);
    }
    else if(EUCLID_POLY == e->type()) {
        //There are n points, last point should be joined to first
        CanvasPoly* poly = new CanvasPoly();
        QPolygon pa(e->numPoints());
        QPointF pt;
        int i=0;
        for(i=0; i<e->numPoints(); ++i) {
            pt = makeQPointF(e->getPoint(i));
            pa.setPoint(i, pt.x(), pt.y());
        }
        poly->setPolygon(pa);
        poly->setZValue(e->zorder());
        poly->setPen(pen);
        poly->setBrush(dgram->colorMapVal(e->cid()));
        poly->setFilled(e->filled());
        poly->setVisible(visible);
        items_.insert(e->id(), poly);
        scene()->addItem(poly);
    }
    else if(EUCLID_POLYLINE == e->type()) {
        CanvasPolyLine* polyline = new CanvasPolyLine();
        QVector<QPointF> points(e->numPoints());
        int i=0;
        for(i=0; i<e->numPoints(); ++i) {
            QPointF pt = makeQPointF(e->getPoint(i));
            // We have initialized the points vector with numPoints points each with value (0,0)
            // Now replace point at each index with the actual coordinates read from the design
            points.replace(i, pt);
        }
        QPolygonF pa(points);
        polyline->setPolygon(pa);
        polyline->setZValue(e->zorder());
        polyline->setPen(pen);
        polyline->setVisible(visible);
        items_.insert(e->id(), polyline);
        scene()->addItem(polyline);
    }
    else if(HYPER_POLYLINE == e->type()) {
        HyperPolyLine* hpl = (HyperPolyLine*)e;
        vector<HyperLine>& mhlines = hpl->hyperLines();
        CanvasHyperPolyLine* hpolyline = new CanvasHyperPolyLine();

        vector<HyperLine>::iterator it;
        for(it = mhlines.begin(); it != mhlines.end(); ++it) {
            CanvasHyperLine* chl = makeCanvasHyperLine(*it);
            hpolyline->addLine(chl);
        }
        hpolyline->setPen(pen);
        hpolyline->setZValue(e->zorder());
        hpolyline->setVisible(visible);
        items_.insert(e->id(), hpolyline);
        scene()->addItem(hpolyline);
    }
    else if(HYPER_POLY == e->type()) {
        HyperPoly* hp = (HyperPoly*)e;
        vector<HyperLine>& mhlines = hp->hyperLines();
        CanvasHyperPoly* hpoly = new CanvasHyperPoly();

        vector<HyperLine>::iterator it;
        for(it = mhlines.begin(); it != mhlines.end(); ++it) {
            CanvasHyperLine* chl = makeCanvasHyperLine(*it);
            hpoly->addLine(chl);
        }
        hpoly->setPen(pen);
        hpoly->setBrush(dgram->colorMapVal(e->cid()));
        hpoly->setZValue(e->zorder());
        hpoly->setVisible(visible);
        hpoly->setFilled(hp->filled());
        items_.insert(e->id(), hpoly);
        scene()->addItem(hpoly);
    }
    else {
        throw "PoincareView::drawElement: Unknown element type!";
    }
}

QPointF PoincareView::makeQPointF(const Point& mp)
{
    QPointF pt;
    Point tmp(mp);
    tmp.weierstrassToPoincare();

    double x,y;
    //map it to scene coordinates
    x = (tmp.x() * diameter()/2 + origin().x());
    y = diameter() - (tmp.y() * diameter()/2 + origin().y());
    pt.setX(x);
    pt.setY(y);
    return pt;
}

CanvasHyperLine* PoincareView::makeCanvasHyperLine(const HyperLine& mhl)
{
    CanvasHyperLine* chl = 0;
    if(mhl.shouldDrawArc()) {
        QPointF tl = makeQPointF(mhl.topLeft());
        QSize sz(mhl.width() * diameter()/2, mhl.height() * diameter()/2);
        chl = new CanvasHyperLine(tl, sz, mhl.startAngle(), mhl.endAngle());
    }
    else {
        chl = new CanvasHyperLine(makeQPointF(mhl.startPoint()), makeQPointF(mhl.endPoint()));
    }
    return chl;
}

void PoincareView::saveAs(QString fileName)
{
    QPixmap pix = grab(sceneRect().toRect());
    pix.save(fileName);
}

//FIXME: Unused legacy umtested method to be connected to a user action slot
void PoincareView::pan ( PanType ptype )
{
    //TODO fix this keyboad scrolling is not working in Linux atleast.
    const int stepSize = 50;
    int hScroll=0, vScroll=0;
    if(PAN_LEFT == ptype)
        hScroll = sceneRect().width()/stepSize;
    else if(PAN_RIGHT == ptype)
        hScroll = - sceneRect().width()/stepSize;
    else if(PAN_UP == ptype)
        vScroll = sceneRect().height()/stepSize;
    else if(PAN_DOWN == ptype)
        vScroll = -sceneRect().height()/stepSize;
    if(hScroll || vScroll) {
        QTransform wm = transform();
        wm.translate(hScroll,vScroll);
        setTransform(wm);
        update();
        //canvas()->update();
    }
}

//FIXME: Unused legacy umtested method to be connected to a user action slot
void PoincareView::toggleLayer(int layerId, bool state)
{
    if( NORMAL != viewMode_)
        return;
    layerId -= 1;
    if(layerId >= 0 && layerId < dgram->numLayers()) {
        isLayerVisible[layerId] = state;
        drawLayer(layerId, isLayerVisible[layerId]);
        scene()->update();
    }
}

//FIXME: Unused legacy umtested method to be connected to a user action slot
void PoincareView::toggleFrame(bool state)
{
    showFrame_ = state;
    drawFrame(showFrame_);
    scene()->update();
}

//FIXME: Unused legacy umtested method to be connected to a user action slot
void PoincareView::animPlay()
{
    viewMode_ = ANIMATE;
    if(paused_) {
        animateTimer->start(animDelay_);
    }
    else { //for first time play
        drawBoundingCircle();
        drawDiagram(false); //hide the whole diagram
        animateNext_ = 0;
        animateTimer->start(animDelay_);
        scene()->update();
    }
    paused_ = false;
}

//FIXME: Unused legacy umtested method to be connected to a user action slot
void PoincareView::animPause() {
    animateTimer->stop();
    paused_ = true;
}

void PoincareView::animStop() {
    viewMode_ = NORMAL;
    animateTimer->stop();
    paused_ = false;
    drawBoundingCircle();
    restoreDiagramState(); //return to view state before animation started
    drawDiagram(true); //show the whole diagram
    scene()->update();
    
    
    //TODO eg only one layer might be visible before animation started
}

//FIXME: Unused legacy umtested method to be connected to a user action slot
void PoincareView::animNext() {
    if(paused_) {
        if(dgram->animq.size() == animateNext_) {
            drawBoundingCircle();
            drawDiagram(false); //hide the whole diagram
            animateNext_ = 0;
            scene()->update();
        }
        PatternPtr p = dgram->pattern(dgram->animq.at(animateNext_));
        drawPattern(*p); //NOTE is ++ atomic?
        ++animateNext_;
        scene()->update();
    }
}

//FIXME: Unused legacy umtested method to be connected to a user action slot
void PoincareView::animPrev() {
    if(paused_) {
        if(animateNext_ < 0) {
            drawBoundingCircle();
            drawDiagram(true); //show the whole diagram
            animateNext_ = dgram->animq.size()-1;
            scene()->update();
        }
        PatternPtr p = dgram->pattern(dgram->animq.at(animateNext_));
        drawPattern(*p, false); //NOTE is -- atomic?
        --animateNext_;
        scene()->update();
    }
}

//FIXME: Unused legacy umtested method to be connected to a user action slot
void PoincareView::animateTimerDone()
{
    if(dgram->animq.size() == animateNext_) {
        //animateTimer->stop();
        //animateNext_ = 0;
        paused_ = false;
        animPlay(); //TODO anim is in loop mode. ask user if loop mode or play once mode.
    }
    else {
        PatternPtr p = dgram->pattern(dgram->animq.at(animateNext_++));
        drawPattern(*p); //NOTE is ++ atomic?
        scene()->update();
    }
}

//FIXME: Unused legacy umtested method to be connected to a user action slot
void PoincareView::drawFrame(bool visible, bool init)
{
       PatternList patterns = dgram->allPatterns();
       for(PatternListIter it = patterns.begin(); it != patterns.end(); ++it) {
            drawPatternFrame(**it, visible, init);
       }
}

//FIXME: Unused legacy umtested method to be connected to a user action slot
void PoincareView::restoreDiagramState()
{
    for(int i=0; i<dgram->numLayers(); i++) {
        drawLayer(i,isLayerVisible[i]);
    }
}
