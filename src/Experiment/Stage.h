/*
 * Stage.h
 *
 *  Created on: 2015/07/22
 *      Author: kryozahiro
 */

#ifndef SRC_EXPERIMENT_STAGE_H_
#define SRC_EXPERIMENT_STAGE_H_

#include "../Solver/Solution.h"

class Stage {
public:
	virtual ~Stage() = default;
	virtual void operator()(std::vector<std::shared_ptr<Solution>>& solutions, std::mt19937_64& randomEngine) = 0;
};

#endif /* SRC_EXPERIMENT_STAGE_H_ */
