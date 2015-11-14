/*
 * GeneticOperable.h
 *
 *  Created on: 2014/06/10
 *      Author: kryozahiro
 */

#ifndef GENETICOPERABLE_H_
#define GENETICOPERABLE_H_

#include <string>
#include <random>

class GeneticOperableBase {
public:
	virtual ~GeneticOperableBase() = default;
	virtual void crossover(const std::string& method, GeneticOperableBase& other, std::mt19937_64& randomEngine) = 0;
	virtual void mutation(const std::string& method, std::mt19937_64& randomEngine) = 0;
};

//遺伝的オペレータを適用可能なクラスのインターフェース
template <class ConcreteGeneticOperable>
class GeneticOperable : public GeneticOperableBase {
public:
	virtual ~GeneticOperable() = default;

	//交叉
	virtual void crossover(const std::string& method, GeneticOperableBase& other, std::mt19937_64& randomEngine) override final;

	//変異
	virtual void mutation(const std::string& method, std::mt19937_64& randomEngine) override final;

private:
	//交叉の実装
	virtual void crossoverImpl(const std::string& method, ConcreteGeneticOperable& other, std::mt19937_64& randomEngine) = 0;

	//変異の実装
	virtual void mutationImpl(const std::string& method, std::mt19937_64& randomEngine) = 0;
};

template <class ConcreteGeneticOperable>
void GeneticOperable<ConcreteGeneticOperable>::crossover(const std::string& method, GeneticOperableBase& other, std::mt19937_64& randomEngine) {
	crossoverImpl(method, dynamic_cast<ConcreteGeneticOperable&>(other), randomEngine);
}

template <class ConcreteGeneticOperable>
void GeneticOperable<ConcreteGeneticOperable>::mutation(const std::string& method, std::mt19937_64& randomEngine) {
	mutationImpl(method, randomEngine);
}

#endif /* GENETICOPERABLE_H_ */
