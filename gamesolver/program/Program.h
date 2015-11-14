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
#include "cpputil/Cloneable.h"
#include "cpputil/ReadWritable.h"
#include "cpputil/reflection/Reflection.h"

#include "ProgramType.h"

#define GAMESOLVER_PROGRAM_MODULE(Class)\
	CPPUTIL_AT_START(Class) {\
		auto global = cpputil::Reflection::get_mutable_instance().getGlobal();\
		auto theClass = global->addScope(cpputil::Scope::CLASS, #Class);\
		theClass->addFunction("generate", std::function<std::vector<std::shared_ptr<Program>>(const ProgramType&, const boost::property_tree::ptree&, int size, std::mt19937_64&)>([](const ProgramType& programType, const boost::property_tree::ptree& node, int size, std::mt19937_64& randomEngine) {\
			return Class::generate(programType, node, size, randomEngine);\
		}));\
	}

class Program : public cpputil::Cloneable<Program>, public cpputil::ReadWritable {
public:
	Program(const ProgramType& programType);
	virtual ~Program() = default;

	//実行
	virtual std::vector<double> operator()(const std::vector<double>& input) = 0;

	//整形文字列出力
	virtual std::string toString() const = 0;

	//ReadWritableの実装
	virtual void read(std::istream& is) override;
	virtual void write(std::ostream& os) const override;

protected:
	const ProgramType& getProgramType() const;

private:
	const ProgramType programType;
};

#endif /* PROGRAM_H_ */
