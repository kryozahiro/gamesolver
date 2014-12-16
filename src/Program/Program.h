/*
 * Program.h
 *
 *  Created on: 2013/11/03
 *      Author: kryozahiro
 */

#ifndef PROGRAM_H_
#define PROGRAM_H_

#include <vector>
#include <string>
#include "ProgramType.h"
#include "CppUtil/Cloneable.h"
#include "CppUtil/ReadWritable.h"

class Program : public cpputil::Cloneable<Program>, public cpputil::ReadWritable {
public:
	virtual ~Program() = default;

	//複製
	virtual Program* clone() override = 0;

	//実行
	virtual std::vector<double> operator()(const std::vector<double>& input) = 0;

	//整形文字列出力
	virtual std::string toString() const = 0;

	//ストリーム入出力
	virtual void read(std::istream& is) override {
		throw "unimplemented";
	}
	virtual void write(std::ostream& os) const override {
		os << "class Program" << std::endl;
	}
};

#endif /* PROGRAM_H_ */
