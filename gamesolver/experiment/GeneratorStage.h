/*
 * GeneratorStage.h
 *
 *  Created on: 2015/07/22
 *      Author: kryozahiro
 */

#ifndef GAMESOLVER_EXPERIMENT_GENERATORSTAGE_H_
#define GAMESOLVER_EXPERIMENT_GENERATORSTAGE_H_

#include <boost/property_tree/ptree.hpp>
#include "../game/Game.h"
#include "Stage.h"

class GeneratorStage : public Stage {
public:
	GeneratorStage(const boost::property_tree::ptree& config, const boost::property_tree::ptree& generatorStageTree, std::shared_ptr<Game>& game);
	virtual ~GeneratorStage() = default;

	/// Stageの実装
	virtual void operator()(std::vector<std::shared_ptr<Solution>>& solutions, std::mt19937_64& randomEngine);

	/// プログラムの種類名を取得する
	std::string getProgramName() const;

private:
	/// プログラムの読み込み
	std::vector<std::shared_ptr<Program>> createPrograms(boost::property_tree::ptree& programTree, std::mt19937_64& randomEngine);
	//std::vector<std::shared_ptr<Program>> readPrograms(const Game& game, GameType gameType, ProgramType programType, std::istream& is);

	/// \<Config>ノード
	const boost::property_tree::ptree config;

	/// \<GeneratorStage>ノード
	const boost::property_tree::ptree generatorStageTree;

	/// 使用するGame
	std::shared_ptr<Game> game;

	/// プログラム
	std::string programName;
	int size;
};

#endif /* GAMESOLVER_EXPERIMENT_GENERATORSTAGE_H_ */
