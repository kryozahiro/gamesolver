/*
 * GeneticOperable.h
 *
 *  Created on: 2014/06/10
 *      Author: kryozahiro
 */

#ifndef GENETICOPERABLE_H_
#define GENETICOPERABLE_H_

//遺伝的オペレータを適用可能なクラスのインターフェース
template <class ConcreteGeneticOperable>
class GeneticOperable {
public:
	virtual ~GeneticOperable() = default;

	//交叉
	virtual void crossover(const std::string& method, ConcreteGeneticOperable& other, std::mt19937_64& randomEngine) = 0;

	//変異
	virtual void mutation(const std::string& method, std::mt19937_64& randomEngine) = 0;
};

#endif /* GENETICOPERABLE_H_ */
