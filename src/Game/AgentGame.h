/*
 * AgentGame.h
 *
 *  Created on: 2013/11/20
 *      Author: kryozahiro
 */

#ifndef AGENTGAME_H_
#define AGENTGAME_H_

#include "Game.h"

class AgentGame : public Game {
public:
	AgentGame(std::pair<int, int> programSize, ProgramType programType, int maxStep);
	virtual ~AgentGame() = default;

	//Gameの実装
	virtual std::vector<double> evaluate(std::vector<Program*>& programs) override final;
	virtual std::string toString() const override = 0;

protected:
	//エージェントと環境を初期化する
	virtual void initEvaluation(int agentSize) = 0;

	//エージェントへの入力を求める
	virtual std::vector<double> percept(int agentIndex) = 0;

	//エージェントからの出力を反映する
	virtual void affect(int agentIndex, const std::vector<double>& action) = 0;

	//エージェントと環境を更新する
	virtual void update() {};

	//全てのエージェントの適応度を求める
	virtual std::vector<double> getFitness() = 0;

	//現在のステップ数の取得
	int getStep() const;

private:
	//最大ステップ数
	int maxStep = 0;

	//現在の状態
	int step = 0;
};

#endif /* AGENTGAME_H_ */
