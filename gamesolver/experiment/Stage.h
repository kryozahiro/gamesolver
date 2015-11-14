/*
 * Stage.h
 *
 *  Created on: 2015/07/22
 *      Author: kryozahiro
 */

#ifndef GAMESOLVER_EXPERIMENT_STAGE_H_
#define GAMESOLVER_EXPERIMENT_STAGE_H_

#include "../solver/Solution.h"

class Stage {
public:
	virtual ~Stage() = default;
	virtual void operator()(std::vector<std::shared_ptr<Solution>>& solutions, std::mt19937_64& randomEngine) = 0;
};

#endif /* GAMESOLVER_EXPERIMENT_STAGE_H_ */
