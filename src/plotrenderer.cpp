#include "plotrenderer.h"
#include "plotnode.h"
#include <QSGNode>
#include <QtMath>
#include <QDebug>

PlotRenderer::PlotRenderer(QQuickItem *parent)
    : QQuickItem(parent)
    , m_lineColor(0, 100, 200)
    , m_lineWidth(2.0f)
    , m_dataBounds(0, -2, 5, 2)  // Default: 5 seconds, -2 to +2 amplitude
    , m_autoScale(true)
    , m_timeWindow(5.0)  // 5 seconds
    , m_updateTimer(new QTimer(this))
    , m_geometryChanged(false)
{
    setFlag(ItemHasContents, true);

    // Set up 60 FPS update timer
    m_updateTimer->setInterval(16);  // ~60 FPS (1000/60 ≈ 16.67ms)
    m_updateTimer->setSingleShot(false);
    connect(m_updateTimer, &QTimer::timeout, this, &PlotRenderer::scheduleUpdate);

    // Connect to size changes
    connect(this, &QQuickItem::widthChanged, this, &PlotRenderer::scheduleUpdate);
    connect(this, &QQuickItem::heightChanged, this, &PlotRenderer::scheduleUpdate);
}

void PlotRenderer::setData(const QVector<QPointF> &data)
{
    if (m_data != data) {
        m_data = data;

        if (m_autoScale) {
            calculateAutoBounds();
        }

        emit dataChanged();

        // Start/stop update timer based on data availability
        if (!m_data.isEmpty() && !m_updateTimer->isActive()) {
            m_updateTimer->start();
        } else if (m_data.isEmpty() && m_updateTimer->isActive()) {
            m_updateTimer->stop();
        }
    }
}

void PlotRenderer::setLineColor(const QColor &color)
{
    if (m_lineColor != color) {
        m_lineColor = color;
        emit lineColorChanged();
        update();
    }
}

void PlotRenderer::setLineWidth(float width)
{
    if (qAbs(m_lineWidth - width) > 0.1f) {
        m_lineWidth = width;
        emit lineWidthChanged();
        update();
    }
}

void PlotRenderer::setDataBounds(const QRectF &bounds)
{
    if (m_dataBounds != bounds) {
        m_dataBounds = bounds;
        emit dataBoundsChanged();
        update();
    }
}

void PlotRenderer::setAutoScale(bool autoScale)
{
    if (m_autoScale != autoScale) {
        m_autoScale = autoScale;

        if (m_autoScale) {
            calculateAutoBounds();
        }

        emit autoScaleChanged();
    }
}

void PlotRenderer::setTimeWindow(double window)
{
    if (qAbs(m_timeWindow - window) > 0.001) {
        m_timeWindow = window;

        if (m_autoScale) {
            calculateAutoBounds();
        }

        emit timeWindowChanged();
        update();
    }
}

void PlotRenderer::scheduleUpdate()
{
    update();
}

QSGNode *PlotRenderer::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    PlotNode *plotNode = static_cast<PlotNode *>(oldNode);

    if (!plotNode) {
        plotNode = new PlotNode;
    }

    // Get visible data points
    QVector<QPointF> visibleData = getVisibleData();

    // Update the plot node
    plotNode->setColor(m_lineColor);
    plotNode->setLineWidth(m_lineWidth);
    plotNode->updateGeometry(visibleData, m_dataBounds, QSizeF(width(), height()));

    return plotNode;
}

void PlotRenderer::calculateAutoBounds()
{
    if (m_data.isEmpty()) {
        return;
    }

    // Find the latest time point
    double maxTime = m_data.last().x();
    double minTime = maxTime - m_timeWindow;

    // Find Y bounds for visible data
    double minY = std::numeric_limits<double>::max();
    double maxY = std::numeric_limits<double>::lowest();

    bool foundAnyPoints = false;
    int visiblePointCount = 0;
    for (const QPointF &point : m_data) {
        if (point.x() >= minTime && point.x() <= maxTime) {
            minY = qMin(minY, point.y());
            maxY = qMax(maxY, point.y());
            foundAnyPoints = true;
            visiblePointCount++;
        }
    }

    if (foundAnyPoints) {
        // Add some padding (10% on each side)
        double yRange = maxY - minY;
        if (yRange < 0.001) {
            yRange = 2.0;  // Default range if all values are the same
            minY -= 1.0;
            maxY += 1.0;
        } else {
            double padding = yRange * 0.1;
            minY -= padding;
            maxY += padding;
        }

        QRectF newBounds(minTime, minY, m_timeWindow, maxY - minY);
        setDataBounds(newBounds);
    }
}

QVector<QPointF> PlotRenderer::getVisibleData()
{
    if (m_data.isEmpty()) {
        return QVector<QPointF>();
    }

    QVector<QPointF> visibleData;
    double minTime = m_dataBounds.left();
    double maxTime = m_dataBounds.right();

    // Find all points within the visible time range
    for (const QPointF &point : m_data) {
        if (point.x() >= minTime && point.x() <= maxTime) {
            visibleData.append(point);
        }
    }

    return visibleData;
}
