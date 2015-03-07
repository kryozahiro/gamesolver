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
	double minimumTime = 0;		//最小経過時間
	double elapsedTime = 0;		//経過時間
	int minimumEvaluation = 0;	//最小評価回数
	int evaluationCount = 0;	//評価回数

	//解の改善量
	double bestImprovement = 0;
	double meanImprovement = 0;

	//改善のなかった期間の割合
	double noBestImprovementScale = 0;
	double best = DBL_MAX;
	int noBestImprovementCount = 0;

	double noMeanImprovementScale = 0;
	double mean = DBL_MAX;
	int noMeanImprovementCount = 0;
};

#endif /* SRC_SOLVER_TERMINATIONCRITERIA_H_ */
