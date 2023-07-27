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

#ifndef POINCAREVIEW_H
#define POINCAREVIEW_H

#include <QGraphicsView>
#include <QTimer>
#include "canvaselements.h"
#include "diagram.h"
#include "element.h"
class Pattern;

/**
Poincare disk view of the diagram.
All the drawing is done using a QGraphicsScene
@author Ajit Datar
*/
class PoincareView : public QGraphicsView
{
Q_OBJECT
public:
    PoincareView(QWidget* parent=0, Qt::WindowFlags f=Qt::Widget);

    ~PoincareView();
    void documentChanged(Diagram *diagram);
    float diameter(); //poincare disk Diamater
    QPoint origin();

protected:
    void resizeEvent(QResizeEvent *event) override;

private: //methods
    void init();
    /**
    draws the poincare disk. 
    TODO Right now the background color is set to an arbitrary
    shade. Maybe this color should be specified in the input XML file.
    */
    void drawBoundingCircle(bool visible=true, bool init = false);
    void drawFrame(bool visible=true, bool init=false);
    void restoreDiagramState();
    /**
    draws the whole diagram showing all generated layers
    */
    void drawDiagram(bool visible=true, bool init = false);
    /**
    draw only the given layer
    */
    void drawLayer(const int layerId, bool visible=true, bool init = false);
    /**
    Draws a pattern using the drawing framework.
    A pattern contains Elements. Elements are mapped to
    appropriate screen objects.
    calls drawElement for each element in the pattern.
    */
    void drawPattern(const Pattern& pat, bool visible=true, bool init = false);
    void drawPatternFrame(const Pattern& pat, bool visible=true, bool init = false);
    /**
    All the points in the Element that is passed should be already transformed
    to their intended coords in the weierstrass/poincare model (this is the job of other
    funtions in the Diagram class). drawPattern will map it to the scene coordinates and draw the elements.
    */
    void drawElement(const ElementPtr e, bool visible=true, bool init = false);
public:
    void saveAs(QString fileName);
    void toggleLayer(int layer, bool state);
    void toggleFrame(bool state);
    void animPlay();
    void animPause();
    void animStop();
    void animNext();
    void animPrev();
    void animateTimerDone();
    void pan(PanType ptype);
private: //utility functions not part of the general view interface
    /**
    Make a QPoint (screen point) from a Point
    Utility function. Used as a point on the QCanvas.
    */
    QPointF makeQPointF(const Point& mp);
    CanvasHyperLine* makeCanvasHyperLine(const HyperLine& mhl);
private: //data
    int diameter_;
    QPoint origin_;
    QGraphicsScene* scene_;
    QGraphicsEllipseItem* disk; //the poincare disk
    ViewMode viewMode_;
    QTransform defaultView;
    bool showFrame_;
    
    QMap<int, bool> isLayerVisible;

    typedef QMap<UId, QGraphicsItem*> ItemStore;
    typedef QMap<UId, QGraphicsItem*>::Iterator ItemStoreIter;
    ItemStore items_;

    QTimer *animateTimer;
    int animateNext_;
    bool paused_;
    int animDelay_; //(in msec)

    Diagram* dgram;
};

#endif
