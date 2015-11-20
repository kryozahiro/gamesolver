/*
 * TerminationCriteria.h
 *
 *  Created on: 2015/02/19
 *      Author: kryozahiro
 */

#ifndef GAMESOLVER_SOLVER_TERMINATIONCRITERIA_H_
#define GAMESOLVER_SOLVER_TERMINATIONCRITERIA_H_

#include <boost/property_tree/ptree.hpp>

#include "SolutionHistory.h"

class TerminationCriteria {
public:
	TerminationCriteria(boost::property_tree::ptree& tree);
	virtual ~TerminationCriteria() = default;

	/// 終了条件を満たしたか判定する
	bool meets(int evaluation, SolutionHistory& solutionHistory);

private:
	double optimum = -DBL_MAX;	//最適解の評価値
	double minimumTime = 0;		//最小経過時間
	double maximumTime = 0;		//最大経過時間
	int minimumEvaluation = 0;	//最小評価回数
	int maximumEvaluation = 0;	//最大評価回数

	/// 解の改善量
	double bestImprovement = 0;
	double meanImprovement = 0;

	/// 改善のなかった期間の割合
	double noBestImprovementScale = 0;
	double best = DBL_MAX;
	int noBestImprovementCount = 0;

	double noMeanImprovementScale = 0;
	double mean = DBL_MAX;
	int noMeanImprovementCount = 0;
};

#endif /* GAMESOLVER_SOLVER_TERMINATIONCRITERIA_H_ */
