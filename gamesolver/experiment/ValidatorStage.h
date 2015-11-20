/*
 * ValidatorStage.h
 *
 *  Created on: 2015/07/22
 *      Author: kryozahiro
 */

#ifndef GAMESOLVER_EXPERIMENT_VALIDATORSTAGE_H_
#define GAMESOLVER_EXPERIMENT_VALIDATORSTAGE_H_

#include <boost/property_tree/ptree.hpp>

#include "../game/Game.h"
#include "Stage.h"

class ValidatorStage : public Stage {
public:
	ValidatorStage(const boost::property_tree::ptree& config, const boost::property_tree::ptree& validatorStageTree, std::shared_ptr<Game>& game);
	virtual ~ValidatorStage() = default;

	/// Stageの実装
	virtual void operator()(std::vector<std::shared_ptr<Solution>>& solutions, std::mt19937_64& randomEngine);

private:
	/// 最適化の結果を検証する
	void validate(std::vector<std::shared_ptr<Solution>>& solutions);

	/// 使用するGame
	std::shared_ptr<Game> game;

	/// 検証の回数
	int times;
};

#endif /* GAMESOLVER_EXPERIMENT_VALIDATORSTAGE_H_ */
