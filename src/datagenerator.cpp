#include "datagenerator.h"
#include <QDebug>
#include <QtMath>

// Base DataSource class implementation
DataSource::DataSource(QObject* parent)
    : QObject(parent)
    , m_timer(new QTimer(this))
    , m_running(false)
    , m_frequency(1.0)  // 1 Hz
    , m_amplitude(1.0)
    , m_bufferSize(5000)  // Store 5 seconds of data at 1000 Hz
    , m_time(0.0)
    , m_timeStep(0.001)  // 1ms = 0.001s
    , m_signalCounter(0)
{
    // Use 5ms interval but generate multiple points per tick to achieve 1000 Hz
    // This is more reliable than 1ms timer
    m_timer->setInterval(5);  // 5ms = 200 Hz timer
    m_timer->setSingleShot(false);
    m_timer->setTimerType(Qt::PreciseTimer);

    connect(m_timer, &QTimer::timeout, this, &DataSource::retrieveDataPoint);

    // Reserve buffer space
    m_dataBuffer.reserve(m_bufferSize);
}

DataSource::~DataSource()
{
    if (m_timer->isActive()) {
        m_timer->stop();
    }
}

void DataSource::setRunning(bool running)
{
    if (m_running != running) {
        m_running = running;

        if (m_running) {
            qDebug() << "Starting data generation with freq:" << m_frequency << "amp:" << m_amplitude;
            m_timer->start();
            qDebug() << "Data generation started";
        } else {
            m_timer->stop();
            qDebug() << "Data generation stopped";
        }

        emit runningChanged();
    }
}

void DataSource::setFrequency(double frequency)
{
    if (qAbs(m_frequency - frequency) > 0.001) {
        qDebug() << "Setting frequency from" << m_frequency << "to" << frequency;
        m_frequency = frequency;
        emit frequencyChanged();
    }
}

void DataSource::setAmplitude(double amplitude)
{
    if (qAbs(m_amplitude - amplitude) > 0.001) {
        qDebug() << "Setting amplitude from" << m_amplitude << "to" << amplitude;
        m_amplitude = amplitude;
        emit amplitudeChanged();
    }
}

void DataSource::setBufferSize(int size)
{
    if (m_bufferSize != size && size > 0) {
        QMutexLocker locker(&m_dataMutex);
        m_bufferSize = size;

        // Resize buffer if needed
        if (m_dataBuffer.size() > m_bufferSize) {
            // Keep the most recent data
            int excess = m_dataBuffer.size() - m_bufferSize;
            m_dataBuffer.remove(0, excess);
        }
        m_dataBuffer.reserve(m_bufferSize);

        emit bufferSizeChanged();
    }
}

QVector<QPointF> DataSource::getData()
{
    QMutexLocker locker(&m_dataMutex);
    return m_dataBuffer;
}

void DataSource::clearData()
{
    QMutexLocker locker(&m_dataMutex);
    m_dataBuffer.clear();
    m_time = 0.0;
    m_signalCounter = 0;
    emit dataChanged();
}

void DataSource::retrieveDataPoint()
{
    // Generate 5 points per timer tick to achieve 1000 Hz
    bool shouldEmitSignal = false;

    for (int i = 0; i < 5; ++i) {
        // Generate waveform data point using virtual function
        double y = generateValue(m_time);
        QPointF newPoint(m_time, y);

        {
            QMutexLocker locker(&m_dataMutex);

            // Add new point
            m_dataBuffer.append(newPoint);

            // Remove old points if buffer is full
            if (m_dataBuffer.size() > m_bufferSize) {
                m_dataBuffer.removeFirst();
            }

            // Emit signal every 50 points for UI updates
            m_signalCounter++;
            if (m_signalCounter >= 50) {
                m_signalCounter = 0;
                shouldEmitSignal = true;
            }
        }

        // Advance time
        m_time += m_timeStep;
    }

    // Emit signal for UI update (outside mutex for better performance)
    if (shouldEmitSignal) {
        emit dataChanged();
    }
}

// SineWave implementation
SineWave::SineWave(QObject* parent) : DataSource(parent)
{
}

double SineWave::generateValue(double time)
{
    return m_amplitude * qSin(2.0 * M_PI * m_frequency * time);
}

// SquareWave implementation
SquareWave::SquareWave(QObject* parent) : DataSource(parent)
{
}

double SquareWave::generateValue(double time)
{
    double phase = fmod(m_frequency * time, 1.0);
    return m_amplitude * (phase < 0.5 ? 1.0 : -1.0);
}

// TriangleWave implementation
TriangleWave::TriangleWave(QObject* parent) : DataSource(parent)
{
}

double TriangleWave::generateValue(double time)
{
    double phase = fmod(m_frequency * time, 1.0);
    if (phase < 0.5) {
        return m_amplitude * (4.0 * phase - 1.0);
    }
    else {
        return m_amplitude * (3.0 - 4.0 * phase);
    }
}
