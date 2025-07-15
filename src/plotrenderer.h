#ifndef PLOTRENDERER_H
#define PLOTRENDERER_H

#include <QQuickItem>
#include <QVector>
#include <QPointF>
#include <QColor>
#include <QTimer>

class PlotNode;

class PlotRenderer : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QVector<QPointF> data READ data WRITE setData NOTIFY dataChanged)
    Q_PROPERTY(QColor lineColor READ lineColor WRITE setLineColor NOTIFY lineColorChanged)
    Q_PROPERTY(float lineWidth READ lineWidth WRITE setLineWidth NOTIFY lineWidthChanged)
    Q_PROPERTY(QRectF dataBounds READ dataBounds WRITE setDataBounds NOTIFY dataBoundsChanged)
    Q_PROPERTY(bool autoScale READ autoScale WRITE setAutoScale NOTIFY autoScaleChanged)
    Q_PROPERTY(double timeWindow READ timeWindow WRITE setTimeWindow NOTIFY timeWindowChanged)

public:
    explicit PlotRenderer(QQuickItem *parent = nullptr);

    QVector<QPointF> data() const { return m_data; }
    void setData(const QVector<QPointF> &data);

    QColor lineColor() const { return m_lineColor; }
    void setLineColor(const QColor &color);

    float lineWidth() const { return m_lineWidth; }
    void setLineWidth(float width);

    QRectF dataBounds() const { return m_dataBounds; }
    void setDataBounds(const QRectF &bounds);

    bool autoScale() const { return m_autoScale; }
    void setAutoScale(bool autoScale);

    double timeWindow() const { return m_timeWindow; }
    void setTimeWindow(double window);

signals:
    void dataChanged();
    void lineColorChanged();
    void lineWidthChanged();
    void dataBoundsChanged();
    void autoScaleChanged();
    void timeWindowChanged();

protected:
    QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *) override;

private slots:
    void scheduleUpdate();

private:
    void calculateAutoBounds();
    QVector<QPointF> getVisibleData();

    QVector<QPointF> m_data;
    QColor m_lineColor;
    float m_lineWidth;
    QRectF m_dataBounds;
    bool m_autoScale;
    double m_timeWindow;
    
    QTimer *m_updateTimer;
    bool m_geometryChanged;
};

#endif // PLOTRENDERER_H
