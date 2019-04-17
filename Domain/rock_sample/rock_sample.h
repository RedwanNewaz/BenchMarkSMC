#ifndef ROCKSAMPLE_H
#define ROCKSAMPLE_H

#include <despot/interface/pomdp.h>
#include <despot/core/mdp.h>
#include "base/base_rock_sample.h"
#include <despot/util/coord.h>
#include <despot/util/grid.h>
#include <unordered_map>
#include <functional>
#include "SharedData.h"

using namespace std;
extern  shared_ptr<SharedData> SHARE;
namespace despot {

/* =============================================================================
 * RockSample class
 * =============================================================================*/

class RockSample: public BaseRockSample {
public:
	RockSample(std::string map);
	RockSample(int size, int rocks);

	bool Step(State& state, double rand_num, ACT_TYPE action, double& reward,
		OBS_TYPE& obs) const;
	int NumActions() const;
	double ObsProb(OBS_TYPE obs, const State& state, ACT_TYPE action) const;
	void PrintObs(const State& state, OBS_TYPE observation,
		std::ostream& out = std::cout) const;

    void PrintState(const State &state, std::ostream &out) const override;

private:
    shared_ptr<SharedData> share_;


};

} // namespace despot

#endif
