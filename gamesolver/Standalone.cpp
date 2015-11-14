/*
 * AiBench.cpp
 *
 *  Created on: 2013/11/04
 *      Author: kryozahiro
 */

#include <boost/log/utility/setup/common_attributes.hpp>
#include "Standalone.h"

#include "experiment/Experiment.h"
using namespace std;
using namespace cpputil;
namespace po = boost::program_options;
namespace lg = boost::log;

Standalone::Standalone() {
	lg::add_common_attributes();
}

void Standalone::initOptions(boost::program_options::options_description& options,
		boost::program_options::positional_options_description& positional) {
	options.add_options()
		("multithread,m", po::value<int>()->default_value(0), "specify the number of threads")
		("config,c", po::value<string>()->default_value("config.xml"), "specify config file")
		("experiment,e", po::value<string>()->default_value(""), "specify experiment")
		("output,o", po::value<string>()->default_value(""), "specify output methods")
		("input,i", po::value<string>()->default_value(""), "specify input file");
}

int Standalone::mainImpl(boost::program_options::variables_map& args) {
	Experiment experiment(args);
	experiment();
	return EXIT_SUCCESS;
}
