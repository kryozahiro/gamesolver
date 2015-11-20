/*
 * ProgramState.h
 *
 *  Created on: 2015/07/09
 *      Author: kryozahiro
 */

#ifndef GAMESOLVER_PROGRAM_INSTRUCTIONSEQUENCE_PROGRAMSTATE_H_
#define GAMESOLVER_PROGRAM_INSTRUCTIONSEQUENCE_PROGRAMSTATE_H_

#include "Instruction.h"

class ProgramState {
public:
	/// コンストラクタ
	ProgramState(const std::vector<double>& memory);

	/// 命令列の実行
	void execute(const std::vector<Instruction>& instructions, const std::vector<double>& input);

	/// 結果の取得
	const std::vector<double>& getMemory() const;

private:
	unsigned int pc = 0;
	bool condition = true;
	std::vector<double> memory;
};

#endif /* GAMESOLVER_PROGRAM_INSTRUCTIONSEQUENCE_PROGRAMSTATE_H_ */
