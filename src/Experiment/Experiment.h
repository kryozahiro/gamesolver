/*
 * Experiment.h
 *
 *  Created on: 2015/07/22
 *      Author: kryozahiro
 */

#ifndef SRC_EXPERIMENT_EXPERIMENT_H_
#define SRC_EXPERIMENT_EXPERIMENT_H_

#include <boost/program_options.hpp>
#include <boost/property_tree/ptree.hpp>
#include "../Solver/Solver.h"
#include "GeneratorStage.h"
#include "SolverStage.h"
#include "ValidatorStage.h"
#include "Output.h"

class Experiment {
public:
	Experiment(const boost::program_options::variables_map& args);
	void operator()();

private:
	//実験設定
	std::mt19937_64 randomEngine;
	boost::property_tree::ptree config;
	boost::property_tree::ptree experiment;
	std::shared_ptr<Game> game;
	std::shared_ptr<GeneratorStage> generatorStage;
	std::shared_ptr<SolverStage> solverStage;
	std::shared_ptr<ValidatorStage> validatorStage;
	std::shared_ptr<Output> output;

	//実験設定の読み込み
	void initExperiment(const boost::program_options::variables_map& args);

	//ゲームの読み込み
	std::shared_ptr<Game> initGame(boost::property_tree::ptree& gameTree);
};

#endif /* SRC_EXPERIMENT_EXPERIMENT_H_ */
