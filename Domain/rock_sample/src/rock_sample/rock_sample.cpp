#include "rock_sample.h"

using namespace std;

namespace despot {

/* =============================================================================
 * RockSample class
 * =============================================================================*/

RockSample::RockSample(string map) :
	BaseRockSample(map) {
	half_efficiency_distance_ = 20;
		share_ = SHARE->getPtr();
}

RockSample::RockSample(int size, int rocks) :
	BaseRockSample(size, rocks) {
	half_efficiency_distance_ = 20;
	share_ = SHARE->getPtr();
}

bool RockSample::Step(State& state, double rand_num, ACT_TYPE action, double& reward,
	OBS_TYPE& obs) const {
	RockSampleState& rockstate = static_cast<RockSampleState&>(state);
	reward = 0;
	obs = E_NONE;

	if (action < E_SAMPLE) { // Move
		switch (action) {
		case Compass::EAST:
			if (GetX(&rockstate) + 1 < size_) {
				IncX(&rockstate);
				break;
			} else {
				reward = +10;
				return true;
			}

		case Compass::NORTH:
			if (GetY(&rockstate) + 1 < size_)
				IncY(&rockstate);
			else
				reward = -100;
			break;

		case Compass::SOUTH:
			if (GetY(&rockstate) - 1 >= 0)
				DecY(&rockstate);
			else
				reward = -100;
			break;

		case Compass::WEST:
			if (GetX(&rockstate) - 1 >= 0)
				DecX(&rockstate);
			else
				reward = -100;
			break;
		}
	}

	if (action == E_SAMPLE) { // Sample
		int rock = grid_(GetRobPosIndex(&rockstate));
		if (rock >= 0) {
			if (GetRock(&rockstate, rock))
				reward = +10;
			else
				reward = -10;
			SampleRock(&rockstate, rock);
		} else {
			reward = -100;
		}
	}

	if (action > E_SAMPLE) { // Sense
		int rock = action - E_SAMPLE - 1;
		assert(rock < num_rocks_);
		obs = GetObservation(rand_num, rockstate, rock);
	}

	// assert(reward != -100);
	return false;
}

int RockSample::NumActions() const {
	return num_rocks_ + 5;
}

double RockSample::ObsProb(OBS_TYPE obs, const State& state, ACT_TYPE action) const {
	if (action <= E_SAMPLE)
		return obs == E_NONE;

	if (obs != E_GOOD && obs != E_BAD)
		return 0;

	const RockSampleState& rockstate =
		static_cast<const RockSampleState&>(state);

	int rock = action - E_SAMPLE - 1;
	double distance = Coord::EuclideanDistance(GetRobPos(&rockstate),
		rock_pos_[rock]);
	double efficiency = (1 + pow(2, -distance / half_efficiency_distance_))
		* 0.5;

	return
		((GetRock(&rockstate, rock) & 1) == obs) ? efficiency : (1 - efficiency);
}

void RockSample::PrintObs(const State& state, OBS_TYPE observation,
	ostream& out) const {
	switch (observation) {
	case E_NONE:
		out << "None" << endl;
		break;
	case E_GOOD:
		out << "Good" << endl;
		break;
	case E_BAD:
		out << "Bad" << endl;
		break;
	}
}

	void RockSample::PrintState(const State &state, std::ostream &out) const {
//		BaseRockSample::PrintState(state, out);
		static int dim_ = size_*size_;
		auto getIndex = [&](int x, int y){ return x+dim_*y;};
		unordered_map<int, int> map;

		out << endl;
		for (int x = 0; x < size_ + 2; x++)
			out << "# ";
		out << endl;
		for (int y = size_ - 1; y >= 0; y--) {
			out << "# ";
			for (int x = 0; x < size_; x++) {
				Coord pos(x, y);
				int rock = grid_(pos);
				int status = GetRock(&state, rock);
				if (GetRobPos(&state) == Coord(x, y)) {
					out << "R ";
					map[getIndex(x,y)] =1;
				}
				else if (rock >= 0) {
					out << rock << (status ? "$" : "X");
					map[getIndex(x,y)] =(status ? 2 : 3);
				}
				else
					out << ". ";
			}
			out << "#" << endl;
		}
		for (int x = 0; x < size_ + 2; x++)
			out << "# ";
		out << endl;


		std::call_once(flag_,[&](){share_->updateSize(dim_); share_->map_size = make_pair(size_,size_);} );

		share_->updateMap(map);
	}

} // namespace despot
