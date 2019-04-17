//
// Created by redwan on 4/6/19.
//

#ifndef BENCHMARKSMC_DESPOTPLANNER_H
#define BENCHMARKSMC_DESPOTPLANNER_H

#include <iostream>
#include <QtCore>
#include <QThread>
#include <QDebug>
#include <despot/planner.h>
#include <despot/core/pomdp_world.h>
#include <functional>
#include "rock_sample.h"
#include "SharedData.h"
extern  shared_ptr<SharedData> SHARE;
using namespace despot;
using namespace std;

class despotPlanner: public QThread, public Planner {

public:
    despotPlanner(QObject *parent);
    despotPlanner(){
        share_ = SHARE->getPtr();
    };

   void run() override;

    bool RunStep(Solver *solver, World *world, Logger *logger) override;



protected:
    std::string ChooseSolver();
    DSPOMDP* InitializeModel(option::Option* options);
    void InitializeDefaultParameters();
    World* InitializeWorld(std::string&  world_type, DSPOMDP* model, option::Option* options);

private:
    shared_ptr<SharedData> share_;


};


#endif //BENCHMARKSMC_DESPOTPLANNER_H
