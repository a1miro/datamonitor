#ifndef SQUAREWAVE_H
#define SQUAREWAVE_H

#include "datasource.h"

class SquareWave : public DataSource
{
    Q_OBJECT
public:
    explicit SquareWave(QObject *parent = nullptr);

protected:
    double generateValue(double time) override;
};

#endif // SQUAREWAVE_H
