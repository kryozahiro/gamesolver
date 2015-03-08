/*
 * InstructionSequence.cpp
 *
 *  Created on: 2015/01/18
 *      Author: kryozahiro
 */

#include <cassert>
#include <algorithm>
#include <boost/property_tree/ptree.hpp>
#include "CppUtil/GenericIo.h"
#include "InstructionSequence.h"
using namespace std;
using namespace cpputil;
namespace pt = boost::property_tree;

InstructionSequence::InstructionSequence(const ProgramType& programType, int maxSize, int memorySize) :
		Program(programType), maxSize(maxSize), memorySize(memorySize) {
	assert(programType.getInputType().getSize() < memorySize);
	assert(programType.getOutputType().getSize() < memorySize);
}

InstructionSequence::InstructionSequence(const ProgramType& programType, const boost::property_tree::ptree& node, std::mt19937_64& randomEngine) :
		Program(programType) {
	instructions.resize(node.get<int>("InitialSize"));
	maxSize = node.get<int>("MaxSize");
	memorySize = node.get<int>("MemorySize");
	assert(programType.getInputType().getSize() < memorySize);
	assert(programType.getOutputType().getSize() < memorySize);

	constantRange = node.get<pair<double, double>>("Constant");

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
	assert(getProgramType().getInputType().accepts(input));

	//実行環境の初期化
	Instruction::Register reg;
	vector<double> memory(memorySize, 0);
	copy(input.begin(), input.end(), memory.begin());

	//命令列の実行
	while (reg.pc < instructions.size()) {
		instructions[reg.pc](reg, memory);
	}

	//出力のクリッピング
	vector<double> output(memory.begin(), memory.begin() + getProgramType().getOutputType().getSize());
	getProgramType().getOutputType().clip(output);
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
	int size1 = instructions.size();
	int size2 = other.instructions.size();

	uniform_int_distribution<int> diffDist(max(-size2, size1 - maxSize), min(size1, maxSize - size2));
	int diff = diffDist(randomEngine);

	uniform_int_distribution<int> lengthDist1(max(0, diff), min(size1, size2 + diff));
	int frag1 = lengthDist1(randomEngine);
	int frag2 = frag1 - diff;
	assert(0 <= size1 - frag1 + frag2);
	assert(size1 - frag1 + frag2 <= maxSize);
	assert(0 <= size2 + frag1 - frag2);
	assert(size2 + frag1 - frag2 <= maxSize);

	uniform_int_distribution<int> begin1Dist(0, size1 - frag1);
	int begin1 = begin1Dist(randomEngine);
	uniform_int_distribution<int> begin2Dist(0, size2 - frag2);
	int begin2 = begin2Dist(randomEngine);

	if (diff >= 0) {
		swap_ranges(instructions.begin() + begin1, instructions.begin() + begin1 + frag2, other.instructions.begin() + begin2);
		other.instructions.insert(other.instructions.begin() + begin2 + frag2, instructions.begin() + begin1 + frag2, instructions.begin() + begin1 + frag1);
		instructions.erase(instructions.begin() + begin1 + frag2, instructions.begin() + begin1 + frag1);
	} else {
		swap_ranges(instructions.begin() + begin1, instructions.begin() + begin1 + frag1, other.instructions.begin() + begin2);
		instructions.insert(instructions.begin() + begin1 + frag1, other.instructions.begin() + begin2 + frag1, other.instructions.begin() + begin2 + frag2);
		other.instructions.erase(other.instructions.begin() + begin2 + frag1, other.instructions.begin() + begin2 + frag2);
	}
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
	uniform_real_distribution<double> constantDist(constantRange.first, constantRange.second);
	Instruction inst;
	inst.set(
		instructionSet[opDist(randomEngine)],
		memDist(randomEngine),
		memDist(randomEngine),
		memDist(randomEngine),
		constantDist(randomEngine)
	);
	return inst;
}
