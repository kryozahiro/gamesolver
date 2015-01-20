/*
 * Solution.h
 *
 *  Created on: 2014/07/23
 *      Author: kryozahiro
 */

#ifndef SOLUTION_H_
#define SOLUTION_H_

#include <cfloat>
#include <memory>
#include "../Program/Program.h"

class Solution {
public:
	Solution(std::shared_ptr<Program> program);
	Solution(const Solution& solution);
	virtual ~Solution() = default;

	//プログラムの取得
	std::shared_ptr<Program> getProgram();
	std::vector<double> operator()(const std::vector<double>& input);

	//文字列化
	std::string toString() const;
	std::string showRelation() const;

	//評価値の設定
	void setFitness(double fitness);
	double getFitness() const;

	//世代の設定
	void setGeneration(int generation);
	int getGeneration() const;

	//次の世代を作る
	std::shared_ptr<Solution> createChild(bool resetFitness = true);
	void addChild(std::shared_ptr<Solution>& child);
	//SolutionLoggerへの間隔ロギングの実装
	//addParent
	//sakujo&renketu

	void setTime(int time);

private:
	std::shared_ptr<Program> program;

	//評価値
	double fitness = DBL_MAX;

	//世代
	int generation = -1;

	//次の世代
	std::vector<std::shared_ptr<Solution>> children;

	//経過時間(msec)
	int time = -1;
};

#endif /* SOLUTION_H_ */
