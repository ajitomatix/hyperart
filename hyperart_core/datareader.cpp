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

#include <QFile>
#include <QColor>
#include <QTextStream>
#include "datareader.h"
#include "diagram.h"

//TODO cleaner code and better error handling

QString DataReader::xmlText;

/*!
    \fn DataReader::collectXML(const QString &fileName)
    Read all the XML file into string xmlText
 */
bool DataReader::collectXML(const QString &fileName)
{
    QFile f( fileName );
    if ( !f.open( QIODevice::ReadOnly ) )
    return false;

    QString line = "";
    QTextStream stream(&f);
    DataReader::xmlText = "";
    while( !stream.atEnd() ) {
        line = stream.readLine();
        DataReader::xmlText += line + "\n";
    }

    f.close();
    return true;
}

Diagram* DataReader::createDiagram(const QString& fileName)
{
    bool readOk = DataReader::collectXML(fileName);

    if(! readOk) {
        qWarning("DataReader::createDiagram : Could not collect XML from input file");
        return 0;
    }
    QDomDocument doc;
    doc.setContent(xmlText);

    //determine document type from the root node
    //and delegate Diagram object creation to appropriate reader
    DiagramType type = DataReader::dgramType(doc);
    if(REGULAR_PGON == type) {
        DataReaderRegularPgon reader;
        RegularPgon* rp = dynamic_cast<RegularPgon*>(reader.readXML(doc));
        if(rp) {
            rp->init();
            return rp;
        }
    }
    else if(IRREGULAR_PGON == type) {
        DataReaderIrregularPgon reader;
        IrregularPgon* irp = dynamic_cast<IrregularPgon*>(reader.readXML(doc));
        if(irp) {
            irp->init();
            return irp;
        }
    }

    return 0;
}

DiagramType DataReader::dgramType(const QDomDocument& doc)
{
    QDomElement root = doc.documentElement();
    QString designType = root.attributeNode("type").value();
    if("REGULAR_PGON" == designType) {
        return REGULAR_PGON;
    }
    else if("IRREGULAR_PGON" == designType) {
        return IRREGULAR_PGON;
    }
    else {
        qWarning("DataReader::dgramType : Not a valid diagram type");
        return DIAGRAM;
    }
}

DataReader::DataReader()
{
    //m = diag;
    
    reflSymMap["REFL_NONE"] = REFL_NONE;
    reflSymMap["REFL_EDGE_BISECTOR"] = REFL_EDGE_BISECTOR;
    reflSymMap["REFL_PGON_RADIUS"] = REFL_PGON_RADIUS;

    orienMap["ROTATION"] = ROTATION;
    orienMap["REFLECTION"] = REFLECTION;
    
    elemTypeMap["EUCLID_POLYLINE"] = EUCLID_POLYLINE;
    elemTypeMap["EUCLID_POLY"] = EUCLID_POLY;
    elemTypeMap["CIRCLE"] = CIRCLE;
    elemTypeMap["HYPER_POLYLINE"] = HYPER_POLYLINE;
    elemTypeMap["HYPER_POLY"] = HYPER_POLY;
}

DataReader::~DataReader()
{
}

bool DataReader::readMetadata(Diagram* m, QDomNode& root)
{
    bool readOk = false;
    QDomNode node = root.firstChild();
    while( !node.isNull() ) {
        if(node.isElement()) {
            if(node.nodeName() == "colors") {
                readOk = false;
                readOk = readColors(m,node);
            }
        }
        node = node.nextSibling();
    }
    return readOk;
}

bool DataReader::readColors(Diagram* m, QDomNode& root) {
    bool readOk = false;
    QDomAttr colorCount = root.toElement().attributeNode("count");
    m->setNumColors(colorCount.value().toInt());
    QDomNode node = root.firstChild();
    for(int i=0; i<m->numColors(); i++) {
        if(node.isElement()) {
            if(node.nodeName() == "color") {
                readOk = readColor(m,node);
            }
        }
        node = node.nextSibling();
    }
    return readOk;
}

bool DataReader::readColor(Diagram* m, QDomNode& root)
{
    bool readOk = false;
    unsigned int cid;
    QColor c("#000000");
    QDomNode node = root.firstChild();
    while( !node.isNull() ) {
        if(node.isElement()) {
            readOk = false;
            if(node.nodeName() == "cid") {
                QDomText text = node.firstChild().toText();
                if( !text.isNull() ) {
                    cid = text.nodeValue().toInt();
                    readOk = true;
                }
            }
            if(node.nodeName() == "hex") {
                QDomText text = node.firstChild().toText();
                if( !text.isNull() ) {
                    c = "#" + text.nodeValue().trimmed();
                    readOk = true; 
                }
            }
        }
        m->setColorMapVal(cid,c);
        node = node.nextSibling();
    }
    return readOk;
}

bool DataReader::readPerm(Diagram* m, QDomNode& root, Permutation& perm) {
    bool readOk = false;
    QDomNode node = root.firstChild();
    for(int i=0; i<m->numColors(); i++) {
        if(node.isElement()) {
            readOk = false;
            if(node.nodeName() == "perm") {
                QDomText text = node.firstChild().toText();
                if( !text.isNull() ) {
                    perm[i] = text.nodeValue().toInt(); 
                    readOk = true;
                }
            }
        }
        node = node.nextSibling();
    }
    return readOk;
}


bool DataReader::readAdjacency(Diagram* m, QDomNode& root) {
    bool readOk = false;
    QDomNode node = root.firstChild();
    for(int i=0; i<m->p(); i++) {
        if(node.isElement()) {
            readOk = false;
            if(node.nodeName() == "entry") {
                readOk = readEntry(m,node);
                }
        }
        node = node.nextSibling();
    }
    return readOk;
}

bool DataReader::readEntry(Diagram* m, QDomNode& root) {
    QDomAttr eAttr = root.toElement().attributeNode("e");
    int e = eAttr.value().toInt();

    bool readOk = false;
    QDomNode node = root.firstChild();
    while( !node.isNull() ) {
        if(node.isElement()) {
            readOk = false;
            if(node.nodeName() == "orientation") {
                QDomText text = node.firstChild().toText();
                readOk = false;
                if( !text.isNull() ) {
                    m->edges[e].setOrientation(orienMap[text.nodeValue().trimmed()]);
                    readOk = true;
                }
            }
            if(node.nodeName() == "edge") {
                QDomText text = node.firstChild().toText();
                readOk = false;
                if( !text.isNull() ) {
                    m->edges[e].setAdjEdgeId(abs(text.nodeValue().toInt()));  //as a saftey take abs value of adjacent edge
                    readOk = true;
                }
            }
            if(node.nodeName() == "color_perm") {
                readOk = readPerm(m, node, m->edges[e].colorPerm());
            }
        }
        node = node.nextSibling();
    }
    return readOk;
}

bool DataReader::readElements(Diagram* m, QDomNode& root) {
    bool readOk = false;
    QDomNode node = root.firstChild();
    while( !node.isNull() ) {
        if(node.isElement()) {
            readOk = false;
            if(node.nodeName() == "elem") {
                readOk = readElem(m,node);
                }
        }
        node = node.nextSibling();
    }
    return readOk;
}

bool DataReader::readElem(Diagram* m, QDomNode& root) {
    bool readOk = false;
    static int zorder = 1; //start with a low zorder
    ElementPtr e=0;
    QDomAttr typeAttr = root.toElement().attributeNode("type");
    if( !elemTypeMap.contains(typeAttr.value()) ) return readOk=false;
    e = createElementObject( elemTypeMap[typeAttr.value()] );
    if(!e) { return readOk=false; }

    QDomNode node = root.firstChild();
    while( !node.isNull() ) {
        if(node.isElement()) {
            readOk = false;
            if(node.nodeName() == "fill") {
                QDomText text = node.firstChild().toText();
                readOk = false;
                if( !text.isNull() ) {
                    if(text.nodeValue().trimmed() == "true") {
                        e->setFilled(true);
                    }
                    else {
                        e->setFilled(false);
                    }
                    readOk = true;
                }
            }
            if(node.nodeName() == "cid") {
                QDomText text = node.firstChild().toText();
                readOk = false;
                if( !text.isNull() ) {
                    e->setCid(text.nodeValue().toInt());
                    readOk = true;
                }
            }
            if(node.nodeName() == "points") {
                readOk = readPoints(node, e);
            }
        }
        node = node.nextSibling();
    }
    e->setZOrder(zorder);
    zorder++; //put next element to the front of the previous
    m->fundPat().addElement(e);
    delete e; //safe to delete e now that it is added
    return readOk;
}

bool DataReader::readPoints(QDomNode& root, ElementPtr e) {
    bool readOk = false;
    QDomNode node = root.firstChild();
    while( !node.isNull() ) {
        if(node.isElement()) {
            readOk = false;
            if(node.nodeName() == "pt") {
                double x = 0.0, y=0.0;
                readOk = readPt(node, x, y);
                Point pt(x,y);
                pt.poincareToWeierstrass(); //convert to Weierstrass as we read
                e->addPoint(pt);
            }
        }
        node = node.nextSibling();
    }
    return readOk;
}

bool DataReader::readPt(QDomNode& root, double& x, double& y) {
    bool readOk = false;
    QDomNode node = root.firstChild();
    while( !node.isNull() ) {
        if(node.isElement()) {
            readOk = false;
            if(node.nodeName() == "x") {
                QDomText text = node.firstChild().toText();
                readOk = false;
                if( !text.isNull() ) {
                    x = text.nodeValue().toDouble();
                    readOk = true;
                }
            }
            if(node.nodeName() == "y") {
                QDomText text = node.firstChild().toText();
                readOk = false;
                if( !text.isNull() ) {
                    y = text.nodeValue().toDouble();
                    readOk = true;
                }
            }
        }
        node = node.nextSibling();
    }
    return readOk;
}

ElementPtr DataReader::createElementObject(ElemType type) {
    ElementPtr e = 0;
    switch(type) {
        case EUCLID_POLYLINE:
            e = new EuclidPolyLine();
            break;
        case EUCLID_POLY:
            e = new EuclidPoly();
            break;
        case CIRCLE:
            e = new Circle();
            break;
        case HYPER_POLYLINE:
            e = new HyperPolyLine();
            break;
        case HYPER_POLY:
            e = new HyperPoly();
            break;
        case ELEMENT:
            throw "Unexpected ElemType";
    }
    return e;
}

//============================================================================

Diagram* DataReaderRegularPgon::readXML(QDomDocument& doc)
{
    RegularPgon* m = new RegularPgon();
    
    QDomElement root = doc.documentElement();
    if(root.tagName() != "design") {
        qWarning("Not a valid motif xml document");
        return 0;
    }

    Diagram* d = dynamic_cast<Diagram*>(m);
    QDomNode node;
    node = root.firstChild();
    bool readOk = true;
    while( !node.isNull() && readOk) {  
        if(node.isElement()) {
            readOk = false;
            if(node.nodeName() == "metadata") {
                readOk = readMetadata(m,node);
            }
            if(node.nodeName() == "p") {
                QDomText text = node.firstChild().toText();
                if( !text.isNull() ) {
                    m->setP(text.nodeValue().toInt()); 
                }
                readOk = true;
            }
            if(node.nodeName() == "q") {
                QDomText text = node.firstChild().toText();
                if( !text.isNull() ) {
                    m->setQ(text.nodeValue().toInt()); 
                }
                readOk = true;
            }
            if(node.nodeName() == "fund_reg_edges") {
                QDomText text = node.firstChild().toText();
                if( !text.isNull() ) {
                    m->setFundRegEdges(text.nodeValue().toInt()); 
                }
                readOk = true;
            }
            
            if(node.nodeName() == "refl_sym_type") {
                QDomText text = node.firstChild().toText();
                if( !text.isNull() ) {
                    m->setReflSym(reflSymMap[text.nodeValue().trimmed()]);
                }
                readOk = true;
            }
            
            if(node.nodeName() == "color_perm_rotn") {
                readOk = readPerm(d, node, m->rotnColorPerm());
            }
            if(node.nodeName() == "color_perm_refl") {
                readOk = readPerm(d, node, m->reflColorPerm());
            }
            if(node.nodeName() == "adjacency") {
                readOk = readAdjacency(d,node);
            }
            if(node.nodeName() == "elements") {
                readOk = readElements(d,node);
            }
        }
        node = node.nextSibling();
    }
    if(readOk) {
        return m;
    }
    return 0;
}

//============================================================================

bool DataReaderIrregularPgon::readQlist(IrregularPgon* m, QDomNode& root)
{
    bool readOk = true;
    QDomNode node = root.firstChild();
    for(int i=0; readOk && i<m->p(); i++) {
        if(node.isElement()) {
            if(node.nodeName() == "q") {
                int qval;
                readOk = readQ(node, qval);
                if(readOk) {
                    m->setQ(i, qval);
                }
                else {
                    qWarning("Failed to read q value");
                }
            }
        }
        node = node.nextSibling();
    }
    return readOk;
}

bool DataReaderIrregularPgon::readQ(QDomNode& root, int& qval)
{
    QDomText text = root.firstChild().toText();
    if( !text.isNull() ) {
        qval = text.nodeValue().toInt(); 
        return true;
    }
    return false;
}

Diagram* DataReaderIrregularPgon::readXML(QDomDocument& doc)
{
    IrregularPgon* m = new IrregularPgon();
    
    QDomElement root = doc.documentElement();
    if(root.tagName() != "design") {
        qWarning("Not a valid 'had' document");
        return 0;
    }

    Diagram* d = dynamic_cast<Diagram*>(m);
    QDomNode node;
    node = root.firstChild();
    bool readOk = true;
    while( !node.isNull() && readOk) {  
        if(node.isElement()) {
            readOk = false;
            if(node.nodeName() == "metadata") {
                readOk = readMetadata(d, node);
            }
            if(node.nodeName() == "p") {
                QDomText text = node.firstChild().toText();
                if( !text.isNull() ) {
                    m->setP(text.nodeValue().toInt()); 
                }
                readOk = true;
            }
            if(node.nodeName() == "qlist") {
                readOk = readQlist(m, node);
            }
            if(node.nodeName() == "adjacency") {
                readOk = readAdjacency(d, node);
            }
            if(node.nodeName() == "elements") {
                readOk = readElements(d, node);
            }
        }
        node = node.nextSibling();
    }
    if(readOk) {
        return m;
    }
    return 0;
}
