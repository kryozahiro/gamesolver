/*
 * ProgramAdaptor.h
 *
 *  Created on: 2014/01/14
 *      Author: kryozahiro
 */

#ifndef PROGRAMADAPTOR_H_
#define PROGRAMADAPTOR_H_

#include <memory>
#include <functional>
#include "Program.h"

//TODO: untested, suspend
class ProgramAdaptor : public Program {
public:
	using Adaptor = std::function<std::vector<double>(const std::vector<double>& base)>;
	static std::vector<double> IdentityAdaptor(const std::vector<double>& base);

	ProgramAdaptor(const ProgramType& programType, std::shared_ptr<Program> program);
	//ProgramAdaptor(const ProgramType& externalType, Adaptor inputAdaptor, std::shared_ptr<Program> program, Adaptor outputAdaptor);
	virtual ~ProgramAdaptor() = default;

	virtual Program* clone();
	virtual std::vector<double> operator()(const std::vector<double>& input);
	virtual std::string toString() const;

	//virtual void read(std::istream& is);
	//virtual void write(std::ostream& os) const;


private:
	ProgramType programType;
	std::shared_ptr<Program> program;

	Adaptor inputAdaptor;
	Adaptor outputAdaptor;
};

#endif /* PROGRAMADAPTOR_H_ */
