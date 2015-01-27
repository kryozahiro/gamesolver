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

string Instruction::toString(Opcode opcode) {
	switch (opcode) {
	case Opcode::ADD:		return "ADD";
	case Opcode::ADD_IMM:	return "ADD_IMM";
	case Opcode::SUB:		return "SUB";
	case Opcode::SUB_IMM:	return "SUB_IMM";
	case Opcode::MUL:		return "MUL";
	case Opcode::MUL_IMM:	return "MUL_IMM";
	case Opcode::DIV:		return "DIV";
	case Opcode::DIV_IMM:	return "DIV_IMM";
	case Opcode::IF_GT:		return "IF_GT";
	case Opcode::IF_GT_IMM:	return "IF_GT_IMM";
	case Opcode::IF_LE:		return "IF_LE";
	case Opcode::IF_LE_IMM:	return "IF_LE_IMM";
	case Opcode::SIN:		return "SIN";
	case Opcode::COS:		return "COS";
	case Opcode::SQRT:		return "SQRT";
	case Opcode::EXP:		return "EXP";
	case Opcode::LOG:		return "LOG";
	default:				assert(false);
	}
	return "";
}

void Instruction::set(Opcode op, int ret, int mem1, int mem2, double constant) {
	this->op = op;
	this->ret = ret;
	this->mem1 = mem1;
	this->mem2 = mem2;
	this->constant = constant;
}

void Instruction::operator()(Register& reg, vector<double>& memory) {
	++reg.pc;
	if (!reg.condition) {
		if (op != Opcode::IF_GT and op != Opcode::IF_GT_IMM and op != Opcode::IF_LE and op != Opcode::IF_LE_IMM) {
			reg.condition = true;
		}
		return;
	}
	switch (op) {
	case Opcode::ADD:
		memory[ret] = memory[mem1] + memory[mem2];
		break;
	case Opcode::ADD_IMM:
		memory[ret] = memory[mem1] + constant;
		break;
	case Opcode::SUB:
		memory[ret] = memory[mem1] - memory[mem2];
		break;
	case Opcode::SUB_IMM:
		memory[ret] = memory[mem1] - constant;
		break;
	case Opcode::MUL:
		memory[ret] = memory[mem1] * memory[mem2];
		break;
	case Opcode::MUL_IMM:
		memory[ret] = memory[mem1] * constant;
		break;
	case Opcode::DIV:
		memory[ret] = (memory[mem2] == 0) ? 0 : memory[mem1] / memory[mem2];
		break;
	case Opcode::DIV_IMM:
		memory[ret] = (mem2 == 0) ? 0 : memory[mem1] / constant;
		break;
	case Opcode::IF_GT:
		reg.condition = (memory[mem1] > memory[mem2]);
		break;
	case Opcode::IF_GT_IMM:
		reg.condition = (memory[mem1] > constant);
		break;
	case Opcode::IF_LE:
		reg.condition = (memory[mem1] <= memory[mem2]);
		break;
	case Opcode::IF_LE_IMM:
		reg.condition = (memory[mem1] <= constant);
		break;
	case Opcode::SIN:
		memory[ret] = sin(memory[mem1]);
		break;
	case Opcode::COS:
		memory[ret] = cos(memory[mem1]);
		break;
	case Opcode::SQRT:
		memory[ret] = (memory[mem1] > 0) ? sqrt(memory[mem1]) : 0;
		break;
	case Opcode::EXP:
		memory[ret] = exp(memory[mem1]);
		break;
	case Opcode::LOG:
		memory[ret] = (memory[mem1] > 0) ? log(memory[mem1]) : 0;
		break;
	default:
		assert(false);
	}
}

string Instruction::toString() const {
	return "[" + to_string(ret) + "] " + toString(op) + " [" + to_string(mem1) + "] [" + to_string(mem2) + "] " + to_string(constant);
}
