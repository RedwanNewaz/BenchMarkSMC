//
// Created by redwan on 4/6/19.
//

#include "include/Util/DisplayBase.h"
#include <include/qcustomplot.h>


bool removeDirFiles(const QString & dirName)
{
    QDir dir(dirName);

    dir.setNameFilters(QStringList() << "*.png");
    dir.setFilter(QDir::Files);
    foreach(QString dirFile, dir.entryList())
        {
            dir.remove(dirFile);
        }
    return true;
}


shared_ptr<SharedData> SHARE = make_shared<SharedData>();

/** *
 * @param simView - simulation view  : robot is blue dot which mission is to sample red dots and leaves on the east
 * @param logView  - performance view: red line for undiscounted and blue area for discounted rewards respectively
 * @param SHARE - Shared class for communicating with planning thread
 * @param MUTE - mutex lock for shared class. It eanbels thread safety- data race.
 */

DisplayBase::DisplayBase(QCustomPlot *simView, QCustomPlot *logView, QProgressBar *progress):simView_(simView),logView_(logView), progress_(progress) {




    bool status =removeDirFiles("./output/sim/");
    bool status2 = removeDirFiles("./output/metric/");

    qDebug()<< "dir removed "<< status<<" metric "<< status2;


    share_ = SHARE->getPtr();
    qDebug()<<"logView_ init";
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(simulationUpdate()));
    timer->start(50);

    logView_->addGraph();
    logView_->graph(0)->setPen(QPen(Qt::blue)); // line color blue for first graph
    logView_->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20))); // first graph will be filled with translucent blue
    logView_->addGraph();
    logView_->graph(1)->setPen(QPen(Qt::red)); // line color red for second graph
    logView_->xAxis2->setVisible(true);
    logView_->xAxis2->setTickLabels(false);
    logView_->yAxis2->setVisible(true);
    logView_->yAxis2->setTickLabels(false);
    // make left and bottom axes always transfer their ranges to right and top axes:
    connect(logView_->xAxis, SIGNAL(rangeChanged(QCPRange)), logView_->xAxis2, SLOT(setRange(QCPRange)));
    connect(logView_->yAxis, SIGNAL(rangeChanged(QCPRange)), logView_->yAxis2, SLOT(setRange(QCPRange)));

    logView_->xAxis->setLabel("step");
    logView_->yAxis->setLabel("reward");
    progress_->setMaximum(35); // default simulation steps 90



}

void DisplayBase::RewardViz(const QVector<double> &x0, const QVector<double> &y0, const QVector<double> &y1) const {


    // pass data points to graphs:
    logView_->graph(0)->setData(x0, y0);
    logView_->graph(1)->setData(x0, y1);
    // let the ranges scale themselves so graph 0 fits perfectly in the visible area:
    logView_->graph(0)->rescaleAxes();
    // same thing for graph 1, but only enlarge ranges (in case graph 1 is smaller than graph 0):
    logView_->graph(1)->rescaleAxes(true);
    // Note: we could have also just called logView_->rescaleAxes(); instead
// Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
    logView_->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    logView_->replot();
}

void DisplayBase::simulationUpdate() {
    /**
     * The idea is two use separate views. Simulation uses scatter plots and performance uses line plots.
     * Worksapce can be visualized using grid cells.
     */

    if(!share_->showSim)
        return;
    QCPScatterStyle myScatter;
    myScatter.setShape(QCPScatterStyle::ssCircle);
    myScatter.setPen(QPen(Qt::white));
    myScatter.setBrush(Qt::blue);
    myScatter.setSize(50);

    QVector<double> x0, y0, y1;
    share_->getReward(x0, y0, y1);
    RewardViz(x0, y0, y1);

    RobotViz(myScatter);

    EnvViz(myScatter);

    progress_->setValue(share_->count_);
    simView_->replot();
    share_->toggleSim();

    // save screenshot
    QString imgSim = "./output/sim/"+QString::number(share_->count_)+".png";
    QString imgMetric = "./output/metric/"+QString::number(share_->count_)+".png";
    simView_->grab().save(imgSim);
    logView_->grab().save(imgMetric);

}

void DisplayBase::EnvViz(QCPScatterStyle &myScatter) const {

    //FIXME -remove blue lines that connects dots

    QVector<double> sample_x, sample_y, rock_x, rock_y;
    share_->getMap(sample_x, sample_y, rock_x, rock_y);
    if(sample_x.empty() && rock_x.empty())
        return;

    // add unknown rock as red
    simView_->addGraph();
    myScatter.setBrush(Qt::red);
    simView_->graph(1)->setScatterStyle(myScatter);
    simView_->graph(1)->setData(sample_x, sample_y);

    // add known rock as black
    simView_->addGraph();
    myScatter.setBrush(Qt::black);
    simView_->graph(2)->setScatterStyle(myScatter);
    simView_->graph(2)->setData(rock_x, rock_y);



    // give the axes some labels:
    simView_->xAxis->setLabel("x");
    simView_->yAxis->setLabel("y");
    // set axes ranges, so we see all data:
    int size = share_->mapSize();
    simView_->xAxis->setRange(0, size);
    simView_->yAxis->setRange(0, size);
    simView_->xAxis->ticker()->setTickCount(size);
    simView_->yAxis->ticker()->setTickCount(size);
}

void DisplayBase::RobotViz(const QCPScatterStyle &myScatter) const {
    QVector<double> robo_x, robo_y;
    share_->getRobotPos(robo_x, robo_y);

    // add robot location
    simView_->addGraph();
    simView_->graph(0)->setScatterStyle(myScatter);
    simView_->graph(0)->setData(robo_x, robo_y);
}

