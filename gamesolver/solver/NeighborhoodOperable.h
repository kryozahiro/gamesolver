/*
 * NeighborhoodOperable.h
 *
 *  Created on: 2015/11/11
 *      Author: kryozahiro
 */

#ifndef GAMESOLVER_SOLVER_NEIGHBORHOODOPERABLE_H_
#define GAMESOLVER_SOLVER_NEIGHBORHOODOPERABLE_H_

#include <string>
#include <random>
#include <vector>
#include <memory>

class NeighborhoodOperableBase {
public:
	virtual ~NeighborhoodOperableBase() = default;
	virtual std::vector<std::shared_ptr<NeighborhoodOperableBase>> neighborhood(const std::string& method, int size, std::mt19937_64& randomEngine) = 0;
};

/// 近傍操作を適用可能なクラスのインターフェース
template <class ConcreteNeighborhoodOperable>
class NeighborhoodOperable : public NeighborhoodOperableBase {
public:
	virtual ~NeighborhoodOperable() = default;

	/// 近傍
	virtual std::vector<std::shared_ptr<NeighborhoodOperableBase>> neighborhood(const std::string& method, int size, std::mt19937_64& randomEngine) override final;

private:
	/// 近傍の実装
	virtual std::vector<std::shared_ptr<ConcreteNeighborhoodOperable>> neighborhoodImpl(const std::string& method, int size, std::mt19937_64& randomEngine) = 0;
};

template <class ConcreteNeighborhoodOperable>
std::vector<std::shared_ptr<NeighborhoodOperableBase>> NeighborhoodOperable<ConcreteNeighborhoodOperable>::neighborhood(const std::string& method, int size, std::mt19937_64& randomEngine) {
	const std::vector<std::shared_ptr<ConcreteNeighborhoodOperable>>& concrete = neighborhoodImpl(method, size, randomEngine);
	return std::vector<std::shared_ptr<NeighborhoodOperableBase>>(begin(concrete), end(concrete));
}

#endif /* GAMESOLVER_SOLVER_NEIGHBORHOODOPERABLE_H_ */
