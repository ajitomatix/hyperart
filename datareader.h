/***************************************************************************
 *   Copyright (C) 2005 by Dr. Douglas Dunham, Ajit Datar                  *
 *   ddunham@d.umn.edu , data0003@d.umn.edu                                *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                                                         *
 ***************************************************************************/
#ifndef DATAREADER_H
#define DATAREADER_H

#include <QObject>
#include <QDomDocument>
#include <QMap>
#include "defs.h"
#include "permutation.h"

class QFile;
#include "diagram.h"
#include "regularpgon.h"
#include "irregularpgon.h"

/**
Reads data from xml file.

@author Ajit Datar
*/
class DataReader
{
public: //static methods
    /**
    Read a Diagram design specification from a file,
    and create a Diagram of appropriate type.
    Return 0 if Digram cannot be created.
    */
    static Diagram* createDiagram(const QString& fileName);
protected:
    static bool collectXML(const QString &fileName);
    /**
    Identify the type of design the XML tree specifies
    */
    static DiagramType dgramType(const QDomDocument& doc);
    static QString xmlText;

public:
    DataReader();

    virtual ~DataReader();
    virtual Diagram* readXML(QDomDocument& doc) = 0;
protected: //methods
    virtual bool readMetadata(Diagram* m, QDomNode& node);
    virtual bool readColors(Diagram* m, QDomNode& root);
    virtual bool readColor(Diagram* m, QDomNode& root);
    virtual bool readPerm(Diagram* m, QDomNode& root, Permutation& perm);
    virtual bool readAdjacency(Diagram* m, QDomNode& root);
    virtual bool readEntry(Diagram* m, QDomNode& root);
    virtual bool readElements(Diagram* m, QDomNode& node);
    virtual bool readElem(Diagram* m, QDomNode& node);
    virtual bool readPoints(QDomNode& root, ElementPtr e);
    virtual bool readPt(QDomNode& root, double& x, double& y);
    virtual ElementPtr createElementObject(ElemType type);
protected: //data
    /**
        Map of strings from XML to enumeration values
    */
    QMap<QString,ReflSymType> reflSymMap;
    /**
        Map of strings from XML to enumeration values
    */
    QMap<QString, Orientation> orienMap;
    QMap<QString, ElemType> elemTypeMap;
};

class DataReaderRegularPgon : public DataReader {
public:
    virtual Diagram* readXML(QDomDocument& doc);
};

class DataReaderIrregularPgon : public DataReader {
public:
    virtual Diagram* readXML(QDomDocument& doc);
protected:
    virtual bool readQlist(IrregularPgon* m, QDomNode& root);
    virtual bool readQ(QDomNode& root, int& qval);
};

#endif
