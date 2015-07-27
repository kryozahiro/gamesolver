/*
 * Instruction.cpp
 *
 *  Created on: 2015/01/18
 *      Author: kryozahiro
 */

#include <cassert>
#include <cmath>
#include <boost/lexical_cast.hpp>
#include "Instruction.h"
using namespace std;

void Instruction::set(Opcode op, int ret, int mem1, int mem2) {
	this->op = op;
	this->ret = ret;
	this->arg1 = mem1;
	this->arg2 = mem2;
}

void Instruction::operator()(unsigned int& pc, unsigned int end, bool& condition, std::vector<double>& memory) const {
	//条件が満たされていないときに分岐以外の命令に到達したらスキップする
	if (!condition and op != Opcode::IF and op != Opcode::IF_GT and op != Opcode::IF_LE) {
		condition = true;
		return;
	}

	switch (op) {
	case Opcode::AND:
		memory[ret] = static_cast<int>(memory[arg1]) & static_cast<int>(memory[arg2]);
		break;
	case Opcode::OR:
		memory[ret] = static_cast<int>(memory[arg1]) | static_cast<int>(memory[arg2]);
		break;
	case Opcode::NOT:
		memory[ret] = ~static_cast<int>(memory[arg1]);
		break;
	case Opcode::ADD:
		memory[ret] = memory[arg1] + memory[arg2];
		break;
	case Opcode::SUB:
		memory[ret] = memory[arg1] - memory[arg2];
		break;
	case Opcode::MUL:
		memory[ret] = memory[arg1] * memory[arg2];
		break;
	case Opcode::DIV:
		memory[ret] = (memory[arg2] != 0) ? memory[arg1] / memory[arg2] : 0;
		break;
	case Opcode::IF:
		condition &= static_cast<int>(memory[arg1]);
		break;
	case Opcode::IF_GT:
		condition &= (memory[arg1] > memory[arg2]);
		break;
	case Opcode::IF_LE:
		condition &= (memory[arg1] <= memory[arg2]);
		break;
	case Opcode::JMP:
		pc += min(static_cast<unsigned int>(abs(memory[arg2])), end);
		break;
	case Opcode::JG:
		if (memory[arg1] > 0) {
			pc += min(static_cast<unsigned int>(abs(memory[arg2])), end);
		}
		break;
	case Opcode::JLE:
		if (memory[arg1] <= 0) {
			pc += min(static_cast<unsigned int>(abs(memory[arg2])), end);
		}
		break;
	case Opcode::SIN:
		memory[ret] = sin(memory[arg1]);
		break;
	case Opcode::COS:
		memory[ret] = cos(memory[arg1]);
		break;
	case Opcode::SQRT:
		memory[ret] = sqrt(abs(memory[arg1]));
		break;
	case Opcode::EXP:
		memory[ret] = exp(memory[arg1]);
		break;
	case Opcode::LOG:
		memory[ret] = (memory[arg1] != 0) ? log(abs(memory[arg1])) : 0;
		break;
	case Opcode::IMM:
		memory[ret] = arg1 * memory.size() + arg2;
		break;
	default:
		assert(false);
	}
}

string Instruction::toString() const {
	return "[" + to_string(ret) + "] " + boost::lexical_cast<string>(op) + " [" + to_string(arg1) + "] [" + to_string(arg2) + "] ";
}

Instruction::Opcode Instruction::getOpcode() const {
	return op;
}

int Instruction::getRet() const {
	return ret;
}

int Instruction::getArg1() const {
	return arg1;
}

int Instruction::getArg2() const {
	return arg2;
}
