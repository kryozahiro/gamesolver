/*
 * AiBench.cpp
 *
 *  Created on: 2013/11/04
 *      Author: kryozahiro
 */

#include <boost/log/utility/setup/common_attributes.hpp>
#include "Experiment/Experiment.h"
#include "AiBench.h"
using namespace std;
using namespace cpputil;
namespace po = boost::program_options;
namespace lg = boost::log;

AiBench::AiBench() {
	lg::add_common_attributes();
}

void AiBench::initOptions(boost::program_options::options_description& options,
		boost::program_options::positional_options_description& positional) {
	options.add_options()
		("help,h", "produce help message")
		("multithread,m", po::value<int>()->default_value(0), "specify thread number")
		("config,c", po::value<string>()->default_value("config.xml"), "specify config file")
		("experiment,e", po::value<string>()->default_value(""), "specify experiment")
		("agents,a", po::value<int>()->default_value(0), "specify the size of agents")
		("program,p", po::value<string>()->default_value(""), "specify program representation")
		("history,y", po::value<int>()->default_value(0), "specify history size")
		("output,o", po::value<string>()->default_value(""), "specify output methods")
		("input,i", po::value<string>()->default_value(""), "specify input file");
}

int AiBench::mainImpl(boost::program_options::variables_map& args) {
	Experiment experiment(args);
	experiment();
	return EXIT_SUCCESS;
}
