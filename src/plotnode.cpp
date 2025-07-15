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
    // Don't manually delete m_geometry - Qt Scene Graph will handle it
    // since we set QSGNode::OwnsGeometry flag
}

void PlotNode::allocateGeometry(int pointCount)
{
    // Don't manually delete - let Qt handle it through OwnsGeometry flag
    if (m_geometry) {
        // Qt will clean up the old geometry when we set a new one
        m_geometry = nullptr;
    }

    if (pointCount < 2) {
        m_geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 0);
        m_geometry->setDrawingMode(QSGGeometry::DrawPoints);
        setGeometry(m_geometry);
        setFlag(QSGNode::OwnsGeometry, true);
        return;
    }

    // Create line strip geometry
    m_geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), pointCount);
    m_geometry->setLineWidth(m_lineWidth);
    m_geometry->setDrawingMode(QSGGeometry::DrawLineStrip);

    setGeometry(m_geometry);
    setFlag(QSGNode::OwnsGeometry, true);
}

void PlotNode::updateGeometry(const QVector<QPointF>& points,
    const QRectF& bounds,
                             const QSizeF &viewSize)
{
    if (points.isEmpty()) {
        allocateGeometry(0);
        return;
    }

    // Reallocate geometry only if point count changed significantly or geometry is null
    if (!m_geometry || qAbs(m_geometry->vertexCount() - points.size()) > 500) {
        allocateGeometry(points.size());
    }

    if (!m_geometry || m_geometry->vertexCount() == 0) {
        return;
    }

    // Ensure we don't exceed allocated vertex count
    int maxVertices = m_geometry->vertexCount();
    int vertexCount = qMin(points.size(), maxVertices);

    if (vertexCount <= 0) {
        return;
    }

    // Update vertex positions
    QSGGeometry::Point2D *vertices = m_geometry->vertexDataAsPoint2D();
    if (!vertices) {
        return; // Safety check
    }

    for (int i = 0; i < vertexCount; ++i) {
        QPointF viewPoint = mapToView(points[i], bounds, viewSize);
        vertices[i].set(static_cast<float>(viewPoint.x()), static_cast<float>(viewPoint.y()));
    }

    // Update the actual vertex count used (only if different)
    if (m_geometry->vertexCount() != vertexCount) {
        m_geometry->allocate(vertexCount);
    }

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
    if (width <= 0.0f) {
        width = 1.0f; // Ensure positive line width
    }

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
    if (bounds.width() <= 0 || bounds.height() <= 0 ||
        viewSize.width() <= 0 || viewSize.height() <= 0) {
        return QPointF(0, 0);
    }

    // Map from data coordinates to view coordinates
    double x = (dataPoint.x() - bounds.left()) / bounds.width() * viewSize.width();

    // For Y mapping: bounds.top() is minimum Y, bounds.bottom() is maximum Y
    // We want minimum Y to map to bottom of view, maximum Y to map to top of view
    double normalizedY = (dataPoint.y() - bounds.top()) / bounds.height();
    double y = viewSize.height() * (1.0 - normalizedY);

    return QPointF(x, y);
}
