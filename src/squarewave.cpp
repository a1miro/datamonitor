#include "squarewave.h"
#include <QtMath>

SquareWave::SquareWave(QObject *parent) : DataSource(parent)
{
}

double SquareWave::generateValue(double time)
{
    double phase = fmod(m_frequency * time, 1.0);
    return m_amplitude * (phase < 0.5 ? 1.0 : -1.0);
}
