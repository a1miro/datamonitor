#ifndef DATASOURCE_H
#define DATASOURCE_H

#include <QObject>
#include <QTimer>
#include <QVector>
#include <QPointF>
#include <QMutex>
#include <QMutexLocker>

class DataSource : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool running READ isRunning WRITE setRunning NOTIFY runningChanged)
    Q_PROPERTY(double frequency READ frequency WRITE setFrequency NOTIFY frequencyChanged)
    Q_PROPERTY(double amplitude READ amplitude WRITE setAmplitude NOTIFY amplitudeChanged)
    Q_PROPERTY(int bufferSize READ bufferSize WRITE setBufferSize NOTIFY bufferSizeChanged)

public:
    explicit DataSource(QObject *parent = nullptr);
    virtual ~DataSource();

    bool isRunning() const { return m_running; }
    void setRunning(bool running);

    double frequency() const { return m_frequency; }
    void setFrequency(double frequency);

    double amplitude() const { return m_amplitude; }
    void setAmplitude(double amplitude);

    int bufferSize() const { return m_bufferSize; }
    void setBufferSize(int size);

    Q_INVOKABLE QVector<QPointF> getData();
    Q_INVOKABLE void clearData();

signals:
    void runningChanged();
    void frequencyChanged();
    void amplitudeChanged();
    void bufferSizeChanged();
    void dataChanged();

protected:
    virtual double generateValue(double time) = 0;

private slots:
    void retrieveDataPoint();

protected:
    QTimer *m_timer;
    QVector<QPointF> m_dataBuffer;
    QMutex m_dataMutex;

    bool m_running;
    double m_frequency;
    double m_amplitude;
    int m_bufferSize;

    double m_time;
    double m_timeStep;
    int m_signalCounter;  // Counter to reduce signal emission frequency
};

#endif // DATASOURCE_H
