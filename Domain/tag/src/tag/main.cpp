#include "dialog.h"
#include <QApplication>
#include <despot/planner.h>
#include <despot/core/pomdp_world.h>
#include <despot/planner.h>
#include "tag.h"
#include <thread>

using namespace despot;

class MyPlanner: public Planner {
public:
    MyPlanner(string lower_bound_str,
            string base_lower_bound_str,
            string upper_bound_str,
            string base_upper_bound_str) : Planner(lower_bound_str,
                base_lower_bound_str,
                upper_bound_str,
                base_lower_bound_str) {
    }

    DSPOMDP* InitializeModel(option::Option* options) {
    DSPOMDP* model = !options[E_PARAMS_FILE] ?
      new Tag() : new Tag(options[E_PARAMS_FILE].arg);
    return model;
    }

    World* InitializeWorld(std::string&  world_type, DSPOMDP* model, option::Option* options)
    {
      return InitializePOMDPWorld(world_type, model, options);
    }

    void InitializeDefaultParameters() {
    Globals::config.pruning_constant = 0.01;
    }

    std::string ChooseSolver(){
      return "DESPOT";
    }


    bool RunStep(Solver *solver, World *world, Logger *logger) {

        bool isTerminate = Planner::RunStep(solver, world, logger);

        //Record step reward
        double reward = static_cast<POMDPWorld*>(world)->step_reward_;

        double discounter_reward = Globals::Discount(step_-1) * reward;

        SHARE->updateValue(discounter_reward,logger->EndRound());


        return isTerminate;
    }

};

using namespace std;

int main(int argc, char* argv[]) {

    QApplication a(argc, argv);
    Dialog w;

    MyPlanner * taskPtr = new MyPlanner("w/o base lower bounds: TRIVIAL; with base lower bounds: RANDOM, SHR, MODE-MDP, MODE-SP, MAJORITY-MDP, MAJORITY-SP (default to MODE-MDP)",
                                        "TRIVIAL (default)",
                                        "w/o base lower bounds: TRIVIAL, MDP, SP, MANHATTAN; with base lower bounds: LOOKAHEAD -- default to SP",
                                        "TRIVIAL, MDP, SP, MANHATTAN (default to SP)");

    // Create a thread using member function
    std::thread th(&MyPlanner::RunEvaluation, taskPtr, argc, argv);

    w.show();
    int res = a.exec();

    th.join();

    delete taskPtr;

    return res;




}
