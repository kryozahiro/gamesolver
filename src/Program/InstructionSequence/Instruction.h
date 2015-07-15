/*
 * Instruction.h
 *
 *  Created on: 2015/01/18
 *      Author: kryozahiro
 */

#ifndef SRC_PROGRAM_INSTRUCTIONSEQUENCE_INSTRUCTION_H_
#define SRC_PROGRAM_INSTRUCTIONSEQUENCE_INSTRUCTION_H_

#include <vector>
#include <string>
#include "CppUtil/EnumUtil.h"

class Instruction {
public:
	CPPUTIL_ENUM(Opcode,
		AND,
		OR,
		NOT,
		ADD,
		SUB,
		MUL,
		DIV,
		IF,
		IF_GT,
		IF_LE,
		JMP,
		JG,
		JLE,
		SIN,
		COS,
		SQRT,
		EXP,
		LOG,
		INT,
		IMM
	);

	void set(Opcode op, int ret, int mem1, int mem2);
	void operator()(unsigned int& pc, unsigned int end, bool& condition, std::vector<double>& memory) const;
	std::string toString() const;

	//アクセサ
	Opcode getOpcode() const;
	int getRet() const;
	int getArg1() const;
	int getArg2() const;

private:
	Opcode op;
	int ret;
	int arg1;
	int arg2;
};

static_assert(std::is_pod<Instruction>(), "class Instruction should be POD");

#endif /* SRC_PROGRAM_INSTRUCTIONSEQUENCE_INSTRUCTION_H_ */
