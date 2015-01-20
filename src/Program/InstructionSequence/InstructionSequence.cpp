/*
 * InstructionSequence.cpp
 *
 *  Created on: 2015/01/18
 *      Author: kryozahiro
 */

#include <cassert>
#include <algorithm>
#include <boost/property_tree/ptree.hpp>
#include "InstructionSequence.h"
using namespace std;
using namespace cpputil;
namespace pt = boost::property_tree;

InstructionSequence::InstructionSequence(const ProgramType& programType, int maxSize, int memorySize) :
		programType(programType), maxSize(maxSize), memorySize(memorySize) {
	assert(programType.getInputSize() < memorySize);
	assert(programType.getOutputSize() < memorySize);
}

InstructionSequence::InstructionSequence(const ProgramType& programType, const boost::property_tree::ptree& node, std::mt19937_64& randomEngine) :
		programType(programType) {
	instructions.resize(node.get<int>("InitialSize"));
	maxSize = node.get<int>("MaxSize");
	memorySize = node.get<int>("MemorySize");
	assert(programType.getInputSize() < memorySize);
	assert(programType.getOutputSize() < memorySize);

	const pt::ptree& operators = node.get_child("Operators");
	for (const pt::ptree::value_type& kvp : operators) {
		if (kvp.first == "Add") {
			addOperator(Instruction::Opcode::ADD);
			addOperator(Instruction::Opcode::ADD_IMM);
		} else if (kvp.first == "Sub") {
			addOperator(Instruction::Opcode::SUB);
			addOperator(Instruction::Opcode::SUB_IMM);
		} else if (kvp.first == "Mul") {
			addOperator(Instruction::Opcode::MUL);
			addOperator(Instruction::Opcode::MUL_IMM);
		} else if (kvp.first == "Div") {
			addOperator(Instruction::Opcode::DIV);
			addOperator(Instruction::Opcode::DIV_IMM);
		} else if (kvp.first == "If") {
			addOperator(Instruction::Opcode::IF_GT);
			addOperator(Instruction::Opcode::IF_GT_IMM);
			addOperator(Instruction::Opcode::IF_LE);
			addOperator(Instruction::Opcode::IF_LE_IMM);
		} else if (kvp.first == "Sin") {
			addOperator(Instruction::Opcode::SIN);
		} else if (kvp.first == "Cos") {
			addOperator(Instruction::Opcode::COS);
		} else if (kvp.first == "Sqrt") {
			addOperator(Instruction::Opcode::SQRT);
		} else if (kvp.first == "Exp") {
			addOperator(Instruction::Opcode::EXP);
		} else if (kvp.first == "Log") {
			addOperator(Instruction::Opcode::LOG);
		} else {
			assert(false);
		}
	}
	randomize(randomEngine);
}

vector<double> InstructionSequence::operator()(const vector<double>& input) {
	assert(programType.acceptsInput(input));

	//実行環境の初期化
	Instruction::Register reg;
	vector<double> memory(memorySize, 0);
	copy(input.begin(), input.end(), memory.begin());

	//命令列の実行
	while (reg.pc < instructions.size()) {
		instructions[reg.pc](reg, memory);
	}

	//出力のクリッピング
	vector<double> output(memory.begin(), memory.begin() + programType.getOutputSize());
	output = programType.clipOutput(output);
	return output;
}

string InstructionSequence::toString() const {
	string ret;
	for (const Instruction& inst : instructions) {
		ret += inst.toString() + "\n";
	}
	return ret;
}

void InstructionSequence::crossover(const std::string& method, InstructionSequence& other, std::mt19937_64& randomEngine) {
	int maxLength = min(instructions.size(), other.instructions.size());
	if (maxLength == 0) {
		return;
	}
	uniform_int_distribution<int> lengthDist(1, maxLength);
	int length = lengthDist(randomEngine);

	uniform_int_distribution<int> begin1Dist(0, instructions.size() - length);
	int begin1 = begin1Dist(randomEngine);
	uniform_int_distribution<int> begin2Dist(0, other.instructions.size() - length);
	int begin2 = begin2Dist(randomEngine);

	swap_ranges(instructions.begin() + begin1, instructions.begin() + begin1 + length, other.instructions.begin() + begin2);
}

void InstructionSequence::mutation(const std::string& method, std::mt19937_64& randomEngine) {
	uniform_real_distribution<double> mutateDist(0, 1);
	for (Instruction& inst : instructions) {
		if (mutateDist(randomEngine) < 0.05) {
			inst = makeRandomInstruction(randomEngine);
		}
	}

	if (instructions.size() < (unsigned int)maxSize and mutateDist(randomEngine) < 0.1) {
		uniform_int_distribution<int> insertDist(0, instructions.size());
		Instruction newInst = makeRandomInstruction(randomEngine);
		instructions.insert(instructions.begin() + insertDist(randomEngine), newInst);
	}
	if (instructions.size() > 0 and mutateDist(randomEngine) < 0.1) {
		uniform_int_distribution<int> eraseDist(0, instructions.size() - 1);
		instructions.erase(instructions.begin() + eraseDist(randomEngine));
	}
}

void InstructionSequence::addOperator(Instruction::Opcode op) {
	instructionSet.push_back(op);
}

void InstructionSequence::randomize(std::mt19937_64& randomEngine) {
	for (Instruction& inst : instructions) {
		inst = makeRandomInstruction(randomEngine);
	}
}

Instruction InstructionSequence::makeRandomInstruction(std::mt19937_64& randomEngine) {
	uniform_int_distribution<int> opDist(0, instructionSet.size() - 1);
	uniform_int_distribution<int> memDist(0, memorySize - 1);
	Instruction inst;
	inst.set(
		instructionSet[opDist(randomEngine)],
		memDist(randomEngine),
		memDist(randomEngine),
		memDist(randomEngine)
	);
	return inst;
}
