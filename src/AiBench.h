/*
 * AiBench.h
 *
 *  Created on: 2013/11/04
 *      Author: kryozahiro
 */

#ifndef AIBENCH_H_
#define AIBENCH_H_

#include "CppUtil/Application.h"

class AiBench : public cpputil::Application<AiBench> {
protected:
	AiBench();
	virtual ~AiBench() = default;

	//Applicationの実装
	virtual void initOptions(boost::program_options::options_description& options,
			boost::program_options::positional_options_description& positional) override;
	virtual int mainImpl(boost::program_options::variables_map& args) override;
};

#endif /* AIBENCH_H_ */
