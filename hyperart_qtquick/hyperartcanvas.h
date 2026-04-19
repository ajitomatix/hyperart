#ifndef HYPERARTCANVAS_H
#define HYPERARTCANVAS_H

#include <QQuickPaintedItem>
#include <QPainter>
#include <QPainterPath>
#include "appcontroller.h"
#include "diagram.h"
#include "element.h"

class HyperArtCanvas : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(AppController* controller READ controller WRITE setController NOTIFY controllerChanged)

public:
    explicit HyperArtCanvas(QQuickItem *parent = nullptr);

    AppController* controller() const { return m_controller; }
    void setController(AppController* controller);
    
    Q_INVOKABLE void saveAs(const QString& filePath);

    void paint(QPainter *painter) override;

signals:
    void controllerChanged();

private slots:
    void onDiagramChanged();

private:
    double diameter() const;
    QPointF origin() const;
    QPointF mapPoint(const Point& mp) const;

    void drawDiagram(QPainter *painter);
    void drawLayer(QPainter *painter, const int layerId);
    void drawPattern(QPainter *painter, const Pattern& pat);
    void drawElement(QPainter *painter, const ElementPtr e);

    AppController* m_controller;
};

#endif // HYPERARTCANVAS_H
