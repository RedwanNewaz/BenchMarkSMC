//
// Created by redwan on 4/6/19.
//

#include "Planner/despotPlanner.h"

despotPlanner::despotPlanner(QObject *parent) : QThread(parent) {}


DSPOMDP* despotPlanner::InitializeModel(option::Option* options) {
    DSPOMDP* model = NULL;
    if (options[E_PARAMS_FILE]) {
        model = new RockSample(options[E_PARAMS_FILE].arg);

    } else {
        int size = 7, number = 8;
        if (options[E_SIZE])
            size = atoi(options[E_SIZE].arg);
        else {
            cerr << "Specify map size using --size option" << endl;
            exit(0);
        }
        if (options[E_NUMBER]) {
            number = atoi(options[E_NUMBER].arg);
        } else {
            cerr << "Specify number of rocks using --number option" << endl;
            exit(0);
        }
        model = new RockSample(size, number);

    }

    return model;
}

World* despotPlanner::InitializeWorld(std::string&  world_type, DSPOMDP* model, option::Option* options)
{
    return InitializePOMDPWorld(world_type, model, options);
}

void despotPlanner::InitializeDefaultParameters() {
}

std::string despotPlanner::ChooseSolver(){
    return "DESPOT";
}

void despotPlanner::run() {
    qDebug()<<" Planning Thread started";
    char *argv[]={"none","--size","7","--number","8"};
    int argc = int(sizeof (argv)/sizeof(argv[0]));
    qDebug()<<argc;
    RunEvaluation(argc, argv);
    qDebug()<<" Planning Thread Finished";
}

bool despotPlanner::RunStep(Solver *solver, World *world, Logger *logger) {

    bool isTerminate = Planner::RunStep(solver, world, logger);

    //Record step reward
    double reward = static_cast<POMDPWorld*>(world)->step_reward_;
    
    double discounter_reward = Globals::Discount(step_-1) * reward;

    share_->updateValue(discounter_reward,logger->EndRound());



    return isTerminate;
}

