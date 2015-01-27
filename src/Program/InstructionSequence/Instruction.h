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

class Instruction {
public:
	enum class Opcode {
		ADD,
		ADD_IMM,
		SUB,
		SUB_IMM,
		MUL,
		MUL_IMM,
		DIV,
		DIV_IMM,
		IF_GT,
		IF_GT_IMM,
		IF_LE,
		IF_LE_IMM,
		SIN,
		COS,
		SQRT,
		EXP,
		LOG,
		OP_END
	};
	static std::string toString(Opcode opcode);

	struct Register {
		unsigned int pc = 0;
		bool condition = false;
	};

	void set(Opcode op, int ret, int mem1, int mem2, double constant);
	void operator()(Register& reg, std::vector<double>& memory);
	std::string toString() const;

private:
	Opcode op;
	int ret;
	int mem1;
	int mem2;
	double constant;
};

static_assert(std::is_pod<Instruction>(), "class Instruction should be POD");

#endif /* SRC_PROGRAM_INSTRUCTIONSEQUENCE_INSTRUCTION_H_ */
