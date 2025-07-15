#include "datagenerator.h"
#include <QDebug>
#include <QtMath>

DataGenerator::DataGenerator(QObject *parent)
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

    connect(m_timer, &QTimer::timeout, this, &DataGenerator::generateDataPoint);

    // Reserve buffer space
    m_dataBuffer.reserve(m_bufferSize);
}

DataGenerator::~DataGenerator()
{
    if (m_timer->isActive()) {
        m_timer->stop();
    }
}

void DataGenerator::setRunning(bool running)
{
    if (m_running != running) {
        m_running = running;

        if (m_running) {
            m_timer->start();
            qDebug() << "Data generation started";
        } else {
            m_timer->stop();
            qDebug() << "Data generation stopped";
        }

        emit runningChanged();
    }
}

void DataGenerator::setFrequency(double frequency)
{
    if (qAbs(m_frequency - frequency) > 0.001) {
        m_frequency = frequency;
        emit frequencyChanged();
    }
}

void DataGenerator::setAmplitude(double amplitude)
{
    if (qAbs(m_amplitude - amplitude) > 0.001) {
        m_amplitude = amplitude;
        emit amplitudeChanged();
    }
}

void DataGenerator::setBufferSize(int size)
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

QVector<QPointF> DataGenerator::getData()
{
    QMutexLocker locker(&m_dataMutex);
    return m_dataBuffer;
}

void DataGenerator::clearData()
{
    QMutexLocker locker(&m_dataMutex);
    m_dataBuffer.clear();
    m_time = 0.0;
    m_signalCounter = 0;
    emit dataChanged();
}

void DataGenerator::generateDataPoint()
{
    // Generate 5 points per timer tick to achieve 1000 Hz (5ms * 5 points = 25ms = 40 Hz * 25 = 1000 Hz)
    bool shouldEmitSignal = false;

    for (int i = 0; i < 5; ++i) {
        // Generate sine wave data point
        double y = m_amplitude * qSin(2.0 * M_PI * m_frequency * m_time);
        QPointF newPoint(m_time, y);

        // Debug output for the first few points
        static int debugCount = 0;
        if (debugCount < 20) {
            qDebug() << "Generated point:" << debugCount << "time:" << m_time
                << "y:" << y << "freq:" << m_frequency << "amp:" << m_amplitude;
            debugCount++;
        }

        {
            QMutexLocker locker(&m_dataMutex);

            // Add new point
            m_dataBuffer.append(newPoint);

            // Remove old points if buffer is full
            if (m_dataBuffer.size() > m_bufferSize) {
                m_dataBuffer.removeFirst();
            }

            // Only emit signal every ~80ms (approximately 12.5 FPS for UI updates)
            // This reduces the signal emission significantly
            m_signalCounter++;
            if (m_signalCounter >= 80) {  // 80 points = 80ms
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
