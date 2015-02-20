/*
 * TerminationCriteria.h
 *
 *  Created on: 2015/02/19
 *      Author: kryozahiro
 */

#ifndef SRC_SOLVER_TERMINATIONCRITERIA_H_
#define SRC_SOLVER_TERMINATIONCRITERIA_H_

#include <boost/property_tree/ptree.hpp>
#include "SolutionHistory.h"

class TerminationCriteria {
public:
	TerminationCriteria(boost::property_tree::ptree& tree);
	virtual ~TerminationCriteria() = default;

	//終了条件を満たしたか判定する
	bool meets(int evaluation, SolutionHistory& solutionHistory);

private:
	int minimumEvaluation = 0;
	double elapsedTime = 0;		//経過時間
	int evaluationCount = 0;	//評価回数
	double improvement = 0;		//解の改善量

	//改善のなかった期間の割合
	double noImprovementScale = 0;
	double best = DBL_MAX;
	int count = 0;
};

#endif /* SRC_SOLVER_TERMINATIONCRITERIA_H_ */
