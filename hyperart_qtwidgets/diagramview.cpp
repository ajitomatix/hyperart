//
// C++ Implementation: diagramview
//
// Description: 
//
//
// Author: Ajit Datar <ajitdatar@gmail.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "diagramview.h"

DiagramView::DiagramView()
{
    docViewer = new QDocViewer();
    //Derived class should connect
    //docViewer :: SIGNAL(onDocumentChange(unsigned int, unsigned long)
    //to DerivedClass :: SLOT(onDocumentChange())
    //
    //and
    //docViewer :: SIGNAL(onDocumentFirstChange(unsigned int, unsigned long)
    //to DerivedClass :: SLOT(onDocumentChange())
}

DiagramView::~DiagramView()
{
    if(docViewer) {
        delete docViewer;
        docViewer = 0;
    }
}

void DiagramView::setDocument(QDocument* doc)
{
    if(docViewer) {
        docViewer->setDocument(doc);
    }
    else {
        qWarning("Doc viewer does not exist");
    }
}

