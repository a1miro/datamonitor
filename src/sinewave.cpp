#include "sinewave.h"
#include <QtMath>

SineWave::SineWave(QObject *parent) : DataSource(parent)
{
}

double SineWave::generateValue(double time)
{
    return m_amplitude * qSin(2.0 * M_PI * m_frequency * time);
}
