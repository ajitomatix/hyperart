//
    // C++ Interface: diagramview
//
// Description: 
//
//
// Author: Ajit Datar <ajitdatar@gmail.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DIAGRAMVIEW_H
#define DIAGRAMVIEW_H
#include "diagram.h"
class QPainter;

/**
Generic interface for a view on a Diagram. Use this as the second base class in your own view implementation.

@author Ajit Datar
*/
class DiagramView {
public:
    DiagramView();

    virtual ~DiagramView();
    virtual void print(QPainter& p)=0;
    virtual void saveAs(QString fileName)=0;
protected:
    Diagram* dgram; //current diagram pointer
};

#endif
