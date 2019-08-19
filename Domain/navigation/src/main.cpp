#include "dialog.h"
#include <QApplication>
#include <despot/planner.h>
#include <despot/core/pomdp_world.h>
#include <thread>

#include "navigation.h"
#include <despot/core/builtin_lower_bounds.h>
#include <despot/core/builtin_policy.h>
#include <despot/core/builtin_upper_bounds.h>
#include <despot/core/particle_belief.h>
#include <despot/planner.h>

using namespace despot;

class MyPlanner: public Planner {
public:
    MyPlanner() {
    }

    DSPOMDP* InitializeModel(option::Option* options) {
      DSPOMDP* model = !options[E_PARAMS_FILE] ?
        new Navigation() : new Navigation(options[E_PARAMS_FILE].arg);
      return model;
    }

    World* InitializeWorld(std::string&  world_type, DSPOMDP* model, option::Option* options)
    {
        return InitializePOMDPWorld(world_type, model, options);
    }

    void InitializeDefaultParameters() {
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


//STEP - NAVIGATION PROBLEM 

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);
  Dialog w;
  MyPlanner * taskPtr = new MyPlanner();
  // Create a thread using member function
  std::thread th(&MyPlanner::RunEvaluation, taskPtr, argc, argv);

  w.show();
  int res = a.exec();

  th.join();

  delete taskPtr;

  return res;
}


