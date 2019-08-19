#include "dialog.h"
#include <QApplication>

#include <despot/planner.h>
#include <despot/core/pomdp_world.h>
#include <thread>

#include <despot/planner.h>
#include "rock_sample.h"


using namespace std;
using namespace despot;

class MyPlanner: public Planner {
public:
  MyPlanner() {
  }

  DSPOMDP* InitializeModel(option::Option* options) {
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

int main(int argc, char* argv[]) {
	QApplication a(argc, argv);
	Dialog w;
	MyPlanner * taskPtr = new MyPlanner();
	// Create a thread using member function
	char *argvv[]={"none","--size","7","--number","8"};
	int argcc = int(sizeof (argvv)/sizeof(argvv[0]));
	std::thread th(&MyPlanner::RunEvaluation, taskPtr, argcc, argvv);

	w.show();
	int res = a.exec();

	th.join();

	delete taskPtr;

	return res;
}
