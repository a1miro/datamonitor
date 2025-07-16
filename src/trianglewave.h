#ifndef TRIANGLEWAVE_H
#define TRIANGLEWAVE_H

#include "datasource.h"

class TriangleWave : public DataSource
{
    Q_OBJECT
public:
    explicit TriangleWave(QObject *parent = nullptr);

protected:
    double generateValue(double time) override;
};

#endif // TRIANGLEWAVE_H
