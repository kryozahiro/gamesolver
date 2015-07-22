/*
 * GeneratorStage.h
 *
 *  Created on: 2015/07/22
 *      Author: kryozahiro
 */

#ifndef SRC_EXPERIMENT_GENERATORSTAGE_H_
#define SRC_EXPERIMENT_GENERATORSTAGE_H_

#include "../Game/Game.h"
#include "Stage.h"

class GeneratorStage : public Stage {
public:
	GeneratorStage(const boost::property_tree::ptree& config, const boost::property_tree::ptree& generatorStageTree, std::shared_ptr<Game>& game);
	virtual ~GeneratorStage() = default;

	//Stageの実装
	virtual void operator()(std::vector<std::shared_ptr<Solution>>& solutions, std::mt19937_64& randomEngine);

	std::string getProgramName() const;

private:
	//プログラムの読み込み
	std::vector<std::shared_ptr<Program>> initPrograms(std::mt19937_64& randomEngine);
	//std::vector<std::shared_ptr<Program>> readPrograms(const Game& game, GameType gameType, ProgramType programType, std::istream& is);

	std::shared_ptr<Game> game;
	boost::property_tree::ptree programTree;
	std::string programName;
	int size;
};

#endif /* SRC_EXPERIMENT_GENERATORSTAGE_H_ */
