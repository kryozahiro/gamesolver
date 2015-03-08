/*
 * AiBench.h
 *
 *  Created on: 2013/11/04
 *      Author: kryozahiro
 */

#ifndef AIBENCH_H_
#define AIBENCH_H_

#include <boost/property_tree/ptree.hpp>
#include "CppUtil/Application.h"
#include "Program/IoMapping/GrayCodeMapping.h"
#include "Program/ExpressionTree/ExpressionTree.h"
#include "Program/InstructionSequence/InstructionSequence.h"
#include "Program/NeuralNetwork/FeedforwardNetwork.h"
#include "Solver/Solver.h"

class AiBench : public cpputil::Application<AiBench> {
protected:
	AiBench();
	virtual ~AiBench() = default;

	//Applicationの実装
	virtual void initOptions(boost::program_options::options_description& options,
			boost::program_options::positional_options_description& positional) override;
	virtual int mainImpl(boost::program_options::variables_map& args) override;

private:
	//実験設定
	boost::property_tree::ptree config;
	int size = 10;
	std::shared_ptr<Game> game;
	std::vector<std::shared_ptr<Program>> programs;
	std::shared_ptr<Solver<Game>> solver;
	std::shared_ptr<TerminationCriteria> termination;
	int historySize = 10;

	//その他の実験用オブジェクト
	std::mt19937_64 randomEngine;
	std::string relationFile;
	std::pair<int, int> evaluationLoggerRange;
	int validationTimes = 10;

	//実験設定の読み込み
	void initExperiment(boost::program_options::variables_map& args);

	//ゲームの読み込み
	std::shared_ptr<Game> initGame(boost::property_tree::ptree& gameTree);

	//プログラムの読み込み
	std::vector<std::shared_ptr<Program>> initPrograms(boost::property_tree::ptree& programTree, std::shared_ptr<Game> game);
	//std::vector<std::shared_ptr<Program>> readPrograms(const Game& game, GameType gameType, ProgramType programType, std::istream& is);

	//ソルバーの読み込み
	std::shared_ptr<Solver<Game>> initSolver(boost::property_tree::ptree& solverTree, boost::property_tree::ptree& programTree);

	//出力方式の読み込み
	void initOutput(boost::property_tree::ptree& outputTree);

	template <template <class ConcreteProgram> class ConcreteSolver>
	std::shared_ptr<Solver<Game>> initSolverImpl(boost::property_tree::ptree& solverTree, boost::property_tree::ptree& programTree) {
		std::shared_ptr<Solver<Game>> solver;
		std::string programName = programTree.rbegin()->first;
		if (programName == "GrayCodeMapping") {
			//solver = ConcreteSolver<GrayCodeMapping>(solverTree, programs, history);
		} else if (programName == "ExpressionTree") {
			solver = std::make_shared<ConcreteSolver<ExpressionTree>>(solverTree, programs);
		} else if (programName == "InstructionSequence") {
			solver = std::make_shared<ConcreteSolver<InstructionSequence>>(solverTree, programs);
		} else if (programName == "FeedforwardNetwork") {
			solver = std::make_shared<ConcreteSolver<FeedforwardNetwork>>(solverTree, programs);
		} else {
			assert(false);
		}
		return solver;
	}

	//最適化の結果の検証
	void validate(std::vector<std::shared_ptr<Solution>>& solutions);
};

#endif /* AIBENCH_H_ */
