#ifndef PLOTNODE_H
#define PLOTNODE_H

#include <QSGGeometryNode>
#include <QSGFlatColorMaterial>
#include <QVector>
#include <QPointF>
#include <QColor>

class PlotNode : public QSGGeometryNode
{
public:
    PlotNode();
    ~PlotNode();

    void updateGeometry(const QVector<QPointF> &points, 
                       const QRectF &bounds, 
                       const QSizeF &viewSize);
    
    void setColor(const QColor &color);
    void setLineWidth(float width);

private:
    QSGGeometry *m_geometry;
    QSGFlatColorMaterial *m_material;
    QVector<QPointF> m_points;
    float m_lineWidth;
    
    void allocateGeometry(int pointCount);
    QPointF mapToView(const QPointF &dataPoint, const QRectF &bounds, const QSizeF &viewSize);
};

#endif // PLOTNODE_H
