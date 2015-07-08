/*
 * Instruction.cpp
 *
 *  Created on: 2015/01/18
 *      Author: kryozahiro
 */

#include <cassert>
#include <cmath>
#include "Instruction.h"
using namespace std;

void Instruction::set(Opcode op, int ret, int mem1, int mem2) {
	this->op = op;
	this->ret = ret;
	this->mem1 = mem1;
	this->mem2 = mem2;
}

void Instruction::operator()(unsigned int& pc, unsigned int end, bool& condition, std::vector<double>& memory) const {
	//条件が満たされていないときに分岐以外の命令に到達したらスキップする
	if (!condition and op != Opcode::IF and op != Opcode::IF_GT and op != Opcode::IF_LE) {
		condition = true;
		return;
	}

	switch (op) {
	case Opcode::AND:
		//memory[ret] = memory[mem1] & memory[mem2];
		break;
	case Opcode::OR:
		//memory[ret] = memory[mem1] | memory[mem2];
		break;
	case Opcode::NOT:
		//memory[ret] = ~memory[mem1];
		break;
	case Opcode::ADD:
		memory[ret] = memory[mem1] + memory[mem2];
		break;
	case Opcode::SUB:
		memory[ret] = memory[mem1] - memory[mem2];
		break;
	case Opcode::MUL:
		memory[ret] = memory[mem1] * memory[mem2];
		break;
	case Opcode::DIV:
		memory[ret] = (memory[mem2] == 0) ? memory[mem1] : memory[mem1] / memory[mem2];
		break;
	case Opcode::IF:
		//condition &= memory[mem1];
		break;
	case Opcode::IF_GT:
		condition &= (memory[mem1] > memory[mem2]);
		break;
	case Opcode::IF_LE:
		condition &= (memory[mem1] <= memory[mem2]);
		break;
	case Opcode::JMP:
		pc += min(static_cast<unsigned int>(abs(memory[mem2])), end);
		break;
	case Opcode::JG:
		if (memory[mem1] > 0) {
			pc += min(static_cast<unsigned int>(abs(memory[mem2])), end);
		}
		break;
	case Opcode::JLE:
		if (memory[mem1] <= 0) {
			pc += min(static_cast<unsigned int>(abs(memory[mem2])), end);
		}
		break;
	case Opcode::SIN:
		memory[ret] = sin(memory[mem1]);
		break;
	case Opcode::COS:
		memory[ret] = cos(memory[mem1]);
		break;
	case Opcode::SQRT:
		memory[ret] = sqrt(abs(memory[mem1]));
		break;
	case Opcode::EXP:
		memory[ret] = exp(memory[mem1]);
		break;
	case Opcode::LOG:
		memory[ret] = (memory[mem1] != 0) ? log(abs(memory[mem1])) : memory[mem1];
		break;
	default:
		assert(false);
	}
}

string Instruction::toString() const {
	return "[" + to_string(ret) + "] " + boost::lexical_cast<string>(op) + " [" + to_string(mem1) + "] [" + to_string(mem2) + "] ";
}
