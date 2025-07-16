#ifndef SINEWAVE_H
#define SINEWAVE_H

#include "datasource.h"

class SineWave : public DataSource
{
    Q_OBJECT
public:
    explicit SineWave(QObject *parent = nullptr);

protected:
    double generateValue(double time) override;
};

#endif // SINEWAVE_H
