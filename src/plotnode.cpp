#include "plotnode.h"
#include <QSGGeometry>
#include <QtMath>

PlotNode::PlotNode()
    : m_geometry(nullptr)
    , m_material(new QSGFlatColorMaterial)
    , m_lineWidth(2.0f)
{
    setMaterial(m_material);
    setFlag(QSGNode::OwnsMaterial, true);
    
    // Set default color to blue
    m_material->setColor(QColor(0, 100, 200));
    
    // Initialize with empty geometry
    allocateGeometry(0);
}

PlotNode::~PlotNode()
{
    delete m_geometry;
}

void PlotNode::allocateGeometry(int pointCount)
{
    if (m_geometry) {
        delete m_geometry;
    }
    
    if (pointCount < 2) {
        m_geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 0);
        setGeometry(m_geometry);
        return;
    }
    
    // Create line strip geometry
    m_geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), pointCount);
    m_geometry->setLineWidth(m_lineWidth);
    m_geometry->setDrawingMode(QSGGeometry::DrawLineStrip);
    
    setGeometry(m_geometry);
    setFlag(QSGNode::OwnsGeometry, true);
}

void PlotNode::updateGeometry(const QVector<QPointF> &points, 
                             const QRectF &bounds, 
                             const QSizeF &viewSize)
{
    if (points.isEmpty()) {
        allocateGeometry(0);
        return;
    }
    
    // Reallocate geometry if point count changed significantly
    if (!m_geometry || qAbs(m_geometry->vertexCount() - points.size()) > 100) {
        allocateGeometry(points.size());
    }
    
    if (!m_geometry || m_geometry->vertexCount() == 0) {
        return;
    }
    
    // Update vertex positions
    QSGGeometry::Point2D *vertices = m_geometry->vertexDataAsPoint2D();
    int vertexCount = qMin(points.size(), m_geometry->vertexCount());
    
    for (int i = 0; i < vertexCount; ++i) {
        QPointF viewPoint = mapToView(points[i], bounds, viewSize);
        vertices[i].set(viewPoint.x(), viewPoint.y());
    }
    
    // Update the actual vertex count used
    m_geometry->allocate(vertexCount);
    
    // Mark geometry as dirty
    markDirty(QSGNode::DirtyGeometry);
}

void PlotNode::setColor(const QColor &color)
{
    if (m_material->color() != color) {
        m_material->setColor(color);
        markDirty(QSGNode::DirtyMaterial);
    }
}

void PlotNode::setLineWidth(float width)
{
    if (qAbs(m_lineWidth - width) > 0.1f) {
        m_lineWidth = width;
        if (m_geometry) {
            m_geometry->setLineWidth(width);
            markDirty(QSGNode::DirtyGeometry);
        }
    }
}

QPointF PlotNode::mapToView(const QPointF &dataPoint, const QRectF &bounds, const QSizeF &viewSize)
{
    if (bounds.width() == 0 || bounds.height() == 0) {
        return QPointF(0, 0);
    }
    
    // Map from data coordinates to view coordinates
    double x = (dataPoint.x() - bounds.left()) / bounds.width() * viewSize.width();
    double y = viewSize.height() - (dataPoint.y() - bounds.bottom()) / bounds.height() * viewSize.height();
    
    return QPointF(x, y);
}
