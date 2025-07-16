#include "trianglewave.h"
#include <QtMath>

TriangleWave::TriangleWave(QObject *parent) : DataSource(parent)
{
}

double TriangleWave::generateValue(double time)
{
    double phase = fmod(m_frequency * time, 1.0);
    if (phase < 0.5) {
        return m_amplitude * (4.0 * phase - 1.0);
    } else {
        return m_amplitude * (3.0 - 4.0 * phase);
    }
}
