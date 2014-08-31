/*
 * main.cpp
 *
 *  Created on: 2013/05/13
 *      Author: kryozahiro
 */

#include "AiBench.h"
using namespace std;

int main(int argc, char* argv[]) {
	return AiBench::get_mutable_instance().main(argc, argv);
}
