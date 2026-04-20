#include "hyperartcanvas.h"
#include <QtMath>
#include <QDebug>

HyperArtCanvas::HyperArtCanvas(QQuickItem *parent)
    : QQuickPaintedItem(parent), m_controller(nullptr)
{
    setAntialiasing(true);
    setOpaquePainting(false);
    setRenderTarget(QQuickPaintedItem::FramebufferObject); // Extremely important hardware caching for resize performance
}

void HyperArtCanvas::setController(AppController* controller)
{
    if (m_controller == controller) return;

    if (m_controller) {
        m_controller->disconnect(this);
    }

    m_controller = controller;

    if (m_controller) {
        connect(m_controller, &AppController::diagramChanged, this, &HyperArtCanvas::onDiagramChanged);
    }
    emit controllerChanged();
    update();
}

void HyperArtCanvas::onDiagramChanged()
{
    update();
}

QColor HyperArtCanvas::backgroundColor() const
{
    return m_backgroundColor;
}

void HyperArtCanvas::setBackgroundColor(const QColor& color)
{
    if (m_backgroundColor != color) {
        m_backgroundColor = color;
        emit backgroundColorChanged();
        update();
    }
}

void HyperArtCanvas::saveAs(const QString& filePath)
{
    if (width() <= 0 || height() <= 0) return;
    
    QImage img(width(), height(), QImage::Format_ARGB32_Premultiplied);
    img.fill(m_backgroundColor); // match QML backdrop dynamically
    
    QPainter p(&img);
    paint(&p);
    p.end();
    
    QUrl url(filePath);
    QString localPath = url.isLocalFile() ? url.toLocalFile() : filePath;
    img.save(localPath);
}

double HyperArtCanvas::diameter() const
{
    return qMin(width(), height());
}

QPointF HyperArtCanvas::origin() const
{
    return QPointF(width() / 2.0, height() / 2.0);
}

QPointF HyperArtCanvas::mapPoint(const Point& mp) const
{
    double px = mp.x();
    double py = mp.y();
    // Fast path: check w to avoid expensive string-based weierstrassToPoincare() conversion
    if (mp.w() != 0.0) {
        double denom = 1.0 + mp.w();
        px /= denom;
        py /= denom;
    }

    return QPointF(px * m_frameHalfDiameter + m_frameOrigin.x(),
                   m_frameDiameter - (py * m_frameHalfDiameter + m_frameOrigin.y()));
}

void HyperArtCanvas::paint(QPainter *painter)
{
    if (!m_controller || !m_controller->hasDiagram() || width() <= 0 || height() <= 0) {
        return;
    }

    painter->setRenderHint(QPainter::Antialiasing, true);

    // Cache viewport constants once per frame for high-performance math
    m_frameDiameter = diameter();
    m_frameHalfDiameter = m_frameDiameter / 2.0;
    m_frameOrigin = origin();

    // Bounding disk
    QRectF rect(0, 0, m_frameDiameter, m_frameDiameter);
    rect.moveCenter(m_frameOrigin);
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(224, 224, 224));
    painter->drawEllipse(rect);

    drawDiagram(painter);
}

void HyperArtCanvas::drawDiagram(QPainter *painter)
{
    Diagram* dgram = m_controller->getDiagram();
    int animStep = m_controller->animationStep();
    
    if (animStep >= 0 && animStep <= dgram->animq.size()) {
        // Draw sequential animation
        for (int i = 0; i < animStep; ++i) {
            PatternPtr p = dgram->pattern(dgram->animq.at(i));
            if (p) drawPattern(painter, *p);
        }
    } else {
        // Draw normally based on layer toggles
        for(int i=0; i<dgram->numLayers(); i++) {
            drawLayer(painter, i);
        }
    }
}

void HyperArtCanvas::drawLayer(QPainter *painter, const int layerId)
{
    if (!m_controller || !m_controller->hasDiagram()) return;
    if (!m_controller->isLayerVisible(layerId)) return;
    
    PatternList patterns = m_controller->getDiagram()->layerPatterns(layerId);
    for(auto it = patterns.begin(); it != patterns.end(); ++it) {
        drawPattern(painter, **it);
    }
}

void HyperArtCanvas::drawPattern(QPainter *painter, const Pattern& pat)
{
    ElemList elems = pat.elems();
    for(auto it = elems.begin(); it != elems.end(); ++it) {
        drawElement(painter, *it);
    }
}

void HyperArtCanvas::drawElement(QPainter *painter, const ElementPtr e)
{
    Diagram* dgram = m_controller->getDiagram();
    
    QColor color = dgram->colorMapVal(e->cid());
    QPen pen(color);
    if (e->lineStyle() == DOTS) {
        pen.setStyle(Qt::DotLine);
    }
    painter->setPen(pen);

    if (e->filled()) {
        painter->setBrush(color);
    } else {
        painter->setBrush(Qt::NoBrush);
    }

    if (e->type() == CIRCLE) {
        Point center = e->getPoint(0);
        Point circum = e->getPoint(1);
        QPointF p1 = mapPoint(center);
        QPointF p2 = mapPoint(circum);
        double radius = sqrt(pow(double(p1.x() - p2.x()), 2) + pow(double(p1.y() - p2.y()), 2));
        QRectF rect(p1.x() - radius, p1.y() - radius, 2*radius, 2*radius);
        painter->drawEllipse(rect);
    }
    else if (e->type() == EUCLID_POLY) {
        QPolygonF poly;
        for (int i = 0; i < e->numPoints(); ++i) {
            poly.append(mapPoint(e->getPoint(i)));
        }
        painter->drawPolygon(poly);
    }
    else if (e->type() == EUCLID_POLYLINE) {
        QPolygonF poly;
        for (int i = 0; i < e->numPoints(); ++i) {
            poly.append(mapPoint(e->getPoint(i)));
        }
        painter->drawPolyline(poly);
    }
    else if (e->type() == HYPER_POLYLINE || e->type() == HYPER_POLY) {
        vector<HyperLine> hlines;
        bool filled = false;
        
        if (e->type() == HYPER_POLYLINE) {
            hlines = ((HyperPolyLine*)e)->hyperLines();
        } else {
            hlines = ((HyperPoly*)e)->hyperLines();
            filled = ((HyperPoly*)e)->filled();
        }

        QPainterPath path;
        bool first = true;
        
        for (const auto& mhl : hlines) {
            if (mhl.shouldDrawArc()) {
                QPointF tl = mapPoint(mhl.topLeft());
                double w = mhl.width() * m_frameHalfDiameter;
                double h = mhl.height() * m_frameHalfDiameter;
                QRectF bounds(tl.x(), tl.y(), w, h);
                
                if (first) {
                    // QPainterPath requires us to move to the exact start coordinate of the arc!
                    // Rather than guessing the start point, QPainterPath::arcTo connects from current position.
                    // To avoid a stray line from (0,0), we must explicitly start a subpath.
                    double startRad = qDegreesToRadians((double)mhl.startAngle());
                    double x = bounds.center().x() + bounds.width()/2.0 * cos(startRad);
                    double y = bounds.center().y() - bounds.height()/2.0 * sin(startRad);
                    path.moveTo(x, y);
                }
                path.arcTo(bounds, mhl.startAngle(), mhl.endAngle());
            } else {
                if (first) {
                    path.moveTo(mapPoint(mhl.startPoint()));
                } else {
                    path.lineTo(mapPoint(mhl.startPoint()));
                }
                path.lineTo(mapPoint(mhl.endPoint()));
            }
            first = false;
        }

        // Only draw polygons with WindingFill for complex geometry
        if (filled) {
            path.setFillRule(Qt::WindingFill);
            painter->drawPath(path);
        } else {
            painter->setBrush(Qt::NoBrush);
            painter->drawPath(path);
        }
    }
}
