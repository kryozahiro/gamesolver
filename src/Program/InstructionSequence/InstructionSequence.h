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
	//遺伝的操作の具体例
	static void homologousCrossover(InstructionSequence& parent1, InstructionSequence& parent2, std::mt19937_64& randomEngine);
	static void fixedLengthCrossover(InstructionSequence& parent1, InstructionSequence& parent2, std::mt19937_64& randomEngine);
	static void variableLengthCrossover(InstructionSequence& parent1, InstructionSequence& parent2, std::mt19937_64& randomEngine);
	static void fixedLengthMutation(InstructionSequence& parent, std::mt19937_64& randomEngine);
	static void variableLengthMutation(InstructionSequence& parent, std::mt19937_64& randomEngine);

	//コンストラクタ
	InstructionSequence(const ProgramType& programType, int variableSize, int maxSize);
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
	//メモリは入力(read)・出力(read/write)・変数(read/write)が順に並んでいる
	const int variableSize;

	//命令列
	const int maxSize;
	std::vector<Instruction> instructions;

	//使用する命令セット
	std::vector<Instruction::Opcode> instructionSet;

	int getMemorySize() const;
	Instruction makeRandomInstruction(std::mt19937_64& randomEngine);
};

#endif /* SRC_PROGRAM_INSTRUCTIONSEQUENCE_INSTRUCTIONSEQUENCE_H_ */
