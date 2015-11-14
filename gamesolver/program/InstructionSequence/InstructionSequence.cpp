/*
 * InstructionSequence.cpp
 *
 *  Created on: 2015/01/18
 *      Author: kryozahiro
 */

#include "InstructionSequence.h"

#include <cassert>
#include <algorithm>
#include <boost/lexical_cast.hpp>
#include "cpputil/GenericIo.h"

#include "ProgramState.h"
using namespace std;
using namespace cpputil;
namespace pt = boost::property_tree;

std::vector<std::shared_ptr<Program>> InstructionSequence::generate(const ProgramType& programType, const boost::property_tree::ptree& node, int size, std::mt19937_64& randomEngine) {
	std::vector<std::shared_ptr<Program>> programs;
	for (int i = 0; i < size; ++i) {
		programs.push_back(make_shared<InstructionSequence>(programType, node, randomEngine));
	}
	return programs;
}

void InstructionSequence::homologousCrossover(InstructionSequence& parent1, InstructionSequence& parent2, std::mt19937_64& randomEngine) {
	int size = min(parent1.instructions.size(), parent2.instructions.size());

	uniform_int_distribution<int> cutDist(0, size);
	int cut1 = cutDist(randomEngine);
	int cut2 = cutDist(randomEngine);
	pair<int, int> cross = minmax(cut1, cut2);
	swap_ranges(parent1.instructions.begin() + cross.first, parent1.instructions.begin() + cross.second, parent2.instructions.begin() + cross.first);
}

void InstructionSequence::fixedLengthCrossover(InstructionSequence& parent1, InstructionSequence& parent2, std::mt19937_64& randomEngine) {
	int size1 = parent1.instructions.size();
	int size2 = parent2.instructions.size();

	uniform_int_distribution<int> lengthDist(0, min(size1, size2));
	int length = lengthDist(randomEngine);

	uniform_int_distribution<int> begin1Dist(0, size1 - length);
	auto begin1 = parent1.instructions.begin() + begin1Dist(randomEngine);
	uniform_int_distribution<int> begin2Dist(0, size2 - length);
	auto begin2 = parent2.instructions.begin() + begin2Dist(randomEngine);
	swap_ranges(begin1, begin1 + length, begin2);
}

void InstructionSequence::variableLengthCrossover(InstructionSequence& parent1, InstructionSequence& parent2, std::mt19937_64& randomEngine) {
	int maxSize = parent1.maxSize;
	int size1 = parent1.instructions.size();
	int size2 = parent2.instructions.size();

	//0 <= f1 <= s1
	//0 <= f2 <= s2
	//0 <= s1 - f1 + f2 <= m  ->  f1 - s1 <= f2 <= m + f1 - s1
	//0 <= s2 + f1 - f2 <= m  ->  s2 + f1 - m <= f2 <= s2 + f1
	uniform_int_distribution<int> frag1Dist(0, size1);
	int frag1 = frag1Dist(randomEngine);
	uniform_int_distribution<int> frag2Dist(max(0, size2 + frag1 - maxSize), min(maxSize + frag1 - size1, size2));
	int frag2 = frag2Dist(randomEngine);

	assert(0 <= frag1 and frag1 <= size1);
	assert(0 <= frag2 and frag2 <= size2);
	assert(0 <= size1 - frag1 + frag2 and size1 - frag1 + frag2 <= maxSize);
	assert(0 <= size2 + frag1 - frag2 and size2 + frag1 - frag2 <= maxSize);

	uniform_int_distribution<int> begin1Dist(0, size1 - frag1);
	auto begin1 = parent1.instructions.begin() + begin1Dist(randomEngine);
	uniform_int_distribution<int> begin2Dist(0, size2 - frag2);
	auto begin2 = parent2.instructions.begin() + begin2Dist(randomEngine);

	if (frag2 - frag1 >= 0) {
		swap_ranges(begin1, begin1 + frag1, begin2);
		parent1.instructions.insert(begin1 + frag1, begin2 + frag1, begin2 + frag2);
		parent2.instructions.erase(begin2 + frag1, begin2 + frag2);
	} else {
		swap_ranges(begin2, begin2 + frag2, begin1);
		parent2.instructions.insert(begin2 + frag2, begin1 + frag2, begin1 + frag1);
		parent1.instructions.erase(begin1 + frag2, begin1 + frag1);
	}

	assert((int)parent1.instructions.size() == size1 - frag1 + frag2);
	assert((int)parent2.instructions.size() == size2 + frag1 - frag2);
}

void InstructionSequence::fixedLengthMutation(InstructionSequence& parent, std::mt19937_64& randomEngine) {
	uniform_real_distribution<double> mutateDist(0, 1);
	uniform_int_distribution<int> opDist(0, parent.instructionSet.size() - 1);
	uniform_int_distribution<int> argDist(0, parent.getMemorySize() - parent.getProgramType().getOutputType().getSize() - 1);
	uniform_int_distribution<int> retDist(parent.getProgramType().getInputType().getSize(), parent.getMemorySize() - 1);
	for (Instruction& inst : parent.instructions) {
		Instruction::Opcode op = (mutateDist(randomEngine) < 0.1) ? parent.instructionSet[opDist(randomEngine)] : inst.getOpcode();
		int ret = (mutateDist(randomEngine) < 0.1) ? retDist(randomEngine) : inst.getRet();
		int arg1 = (mutateDist(randomEngine) < 0.1) ? argDist(randomEngine) : inst.getArg1();
		int arg2 = (mutateDist(randomEngine) < 0.1) ? argDist(randomEngine) : inst.getArg2();
		inst.set(op, ret, arg1, arg2);
	}
}

void InstructionSequence::variableLengthMutation(InstructionSequence& parent, std::mt19937_64& randomEngine) {
	fixedLengthMutation(parent, randomEngine);

	uniform_real_distribution<double> mutateDist(0, 1);
	if (parent.instructions.size() < (unsigned int)parent.maxSize and mutateDist(randomEngine) < 0.1) {
		uniform_int_distribution<int> insertDist(0, parent.instructions.size());
		Instruction newInst = parent.makeRandomInstruction(randomEngine);
		parent.instructions.insert(parent.instructions.begin() + insertDist(randomEngine), newInst);
	}
	if (parent.instructions.size() > 0 and mutateDist(randomEngine) < 0.1) {
		uniform_int_distribution<int> eraseDist(0, parent.instructions.size() - 1);
		parent.instructions.erase(parent.instructions.begin() + eraseDist(randomEngine));
	}
}

InstructionSequence::InstructionSequence(const ProgramType& programType, int variableSize, int maxSize) :
		Program(programType),
		variableSize(variableSize),
		maxSize(maxSize) {
}

InstructionSequence::InstructionSequence(const ProgramType& programType, const boost::property_tree::ptree& node, std::mt19937_64& randomEngine) :
		Program(programType),
		variableSize(node.get<int>("VariableSize")),
		maxSize(node.get<int>("MaxSize")) {
	instructions.resize(node.get<int>("InitialSize"));
	const pt::ptree& operators = node.get_child("Operators");
	for (const pt::ptree::value_type& kvp : operators) {
		addOperator(boost::lexical_cast<Instruction::Opcode>(kvp.first));
	}
	randomize(randomEngine);
}

vector<double> InstructionSequence::operator()(const vector<double>& input) {
	assert(getProgramType().getInputType().accepts(input));

	//実行環境の初期化
	ProgramState ps(vector<double>(getMemorySize(), 1));

	//命令列の実行
	ps.execute(instructions, input);

	//出力のクリッピング
	auto outputBegin = ps.getMemory().begin() + getProgramType().getInputType().getSize() + variableSize;
	vector<double> output(outputBegin, outputBegin + getProgramType().getOutputType().getSize());
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

void InstructionSequence::crossoverImpl(const std::string& method, InstructionSequence& other, std::mt19937_64& randomEngine) {
	if (method.find("Homo") != string::npos) {
		homologousCrossover(*this, other, randomEngine);
	} else if (method.find("Fixed") != string::npos) {
		fixedLengthCrossover(*this, other, randomEngine);
	} else {
		variableLengthCrossover(*this, other, randomEngine);
	}
}

void InstructionSequence::mutationImpl(const std::string& method, std::mt19937_64& randomEngine) {
	if (method.find("Fixed") != string::npos) {
		fixedLengthMutation(*this, randomEngine);
	} else {
		variableLengthMutation(*this, randomEngine);
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

int InstructionSequence::getMemorySize() const {
	return getProgramType().getInputType().getSize() + variableSize + getProgramType().getOutputType().getSize();
}

Instruction InstructionSequence::makeRandomInstruction(std::mt19937_64& randomEngine) {
	uniform_int_distribution<int> opDist(0, instructionSet.size() - 1);
	uniform_int_distribution<int> argDist(0, getMemorySize() - getProgramType().getOutputType().getSize() - 1);
	uniform_int_distribution<int> retDist(getProgramType().getInputType().getSize(), getMemorySize() - 1);
	Instruction inst;
	inst.set(
		instructionSet[opDist(randomEngine)],
		retDist(randomEngine),
		argDist(randomEngine),
		argDist(randomEngine)
	);
	return inst;
}
