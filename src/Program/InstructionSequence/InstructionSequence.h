/*
 * InstructionSequence.h
 *
 *  Created on: 2015/01/18
 *      Author: kryozahiro
 */

#ifndef SRC_PROGRAM_INSTRUCTIONSEQUENCE_INSTRUCTIONSEQUENCE_H_
#define SRC_PROGRAM_INSTRUCTIONSEQUENCE_INSTRUCTIONSEQUENCE_H_

#include <random>
#include "../Program.h"
#include "../../Solver/GeneticOperable.h"
#include "Instruction.h"

class InstructionSequence : public Program, public GeneticOperable<InstructionSequence> {
public:
	//コンストラクタ
	InstructionSequence(const ProgramType& programType, int maxSize, int memorySize);
	InstructionSequence(const ProgramType& programType, const boost::property_tree::ptree& node, std::mt19937_64& randomEngine);
	virtual ~InstructionSequence() = default;

	//Programの実装
	CPPUTIL_CLONEABLE;
	virtual std::vector<double> operator()(const std::vector<double>& input) override;
	virtual std::string toString() const override;

	//GeneticOperableの実装
	virtual void crossover(const std::string& method, InstructionSequence& other, std::mt19937_64& randomEngine) override;
	virtual void mutation(const std::string& method, std::mt19937_64& randomEngine) override;

	//使用するオペレータの設定
	void addOperator(Instruction::Opcode op);

	//ランダムに初期化
	void randomize(std::mt19937_64& randomEngine);

private:
	//問題の仕様
	int maxSize = 100;
	int memorySize;
	std::pair<double, double> constantRange;

	//命令列
	std::vector<Instruction> instructions;

	//使用する命令セット
	std::vector<Instruction::Opcode> instructionSet;

	Instruction makeRandomInstruction(std::mt19937_64& randomEngine);
};

#endif /* SRC_PROGRAM_INSTRUCTIONSEQUENCE_INSTRUCTIONSEQUENCE_H_ */
