/*
 * main.cpp
 *
 *  Created on: 2013/05/13
 *      Author: kryozahiro
 */

#include "Standalone.h"
using namespace std;

#ifdef GAMESOLVER_STANDALONE
int main(int argc, char* argv[]) {
	return Standalone::get_mutable_instance().main(argc, argv);
}
#endif
