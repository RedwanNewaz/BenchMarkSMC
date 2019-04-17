//
// Created by redwan on 4/6/19.
//

#ifndef BENCHMARKSMC_DISPLAYBASE_H
#define BENCHMARKSMC_DISPLAYBASE_H

#include <QObject>
#include <QTimer>
#include "qcustomplot.h"
#include "SharedData.h"

class DisplayBase : public QObject
{
Q_OBJECT
public:
    explicit DisplayBase(QObject *parent = nullptr):QObject(parent){};

    DisplayBase(QCustomPlot *simView, QCustomPlot *logView, QProgressBar *progress);

signals:

public slots:
    void simulationUpdate();

protected:
    QCustomPlot *simView_, *logView_;
    QProgressBar *progress_;
    QTimer *timer;
    shared_ptr<SharedData> share_;

    void RobotViz(const QCPScatterStyle &myScatter) const;

    void EnvViz(QCPScatterStyle &myScatter) const;

    void RewardViz(const QVector<double> &x0, const QVector<double> &y0, const QVector<double> &y1) const;
};

#endif //BENCHMARKSMC_DISPLAYBASE_H
