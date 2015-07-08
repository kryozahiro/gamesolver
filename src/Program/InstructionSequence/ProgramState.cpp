/*
 * ProgramState.cpp
 *
 *  Created on: 2015/07/09
 *      Author: kryozahiro
 */

#include "ProgramState.h"
using namespace std;

ProgramState::ProgramState(const std::vector<double>& memory) : memory(memory) {
}

void ProgramState::execute(const std::vector<Instruction>& instructions, const std::vector<double>& input) {
	//入力の読み込み
	copy(input.begin(), input.end(), memory.begin());

	//命令列の実行
	for (pc = 0; pc < instructions.size(); ++pc) {
		instructions[pc](pc, instructions.size(), condition, memory);
	}
}

const std::vector<double>& ProgramState::getMemory() const {
	return memory;
}
