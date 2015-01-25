/*
 * FeedforwardNetwork.h
 *
 *  Created on: 2014/01/13
 *      Author: kryozahiro
 */

#ifndef FEEDFORWARDNETWORK_H_
#define FEEDFORWARDNETWORK_H_

#include <random>
#include <functional>
#include <boost/container/vector.hpp>
#include <boost/property_tree/ptree.hpp>
#include "../Program.h"
#include "../../Solver/GeneticOperable.h"

class FeedforwardNetwork : public Program, public GeneticOperable<FeedforwardNetwork> {
public:
	//遺伝的操作の例
	static void crossover(FeedforwardNetwork& parent1, FeedforwardNetwork& parent2, std::mt19937_64& randomEngine);
	static void normalMutation(FeedforwardNetwork& net, std::mt19937_64& randomEngine);

	//活性化関数の例
	static double unitStep(double x);
	static double sigmoid(double a, double x);

	FeedforwardNetwork(const ProgramType& programType, int hiddenSize, std::function<double(double)> activation);
	FeedforwardNetwork(const ProgramType& programType, const boost::property_tree::ptree& node, std::mt19937_64& randomEngine);
	virtual ~FeedforwardNetwork() = default;

	//Programの実装
	CPPUTIL_CLONEABLE;
	virtual std::vector<double> operator()(const std::vector<double>& input);
	virtual std::string toString() const;
	virtual void read(std::istream& is);
	virtual void write(std::ostream& os) const;

	//GeneticOperableの実装
	virtual void crossover(const std::string& method, FeedforwardNetwork& other, std::mt19937_64& randomEngine);
	virtual void mutation(const std::string& method, std::mt19937_64& randomEngine);

	//ノード間の接続を設定する
	//ノード番号は0から始まり定数1・入力層・中間層・出力層の順に振られる
	int getBias();
	int getInputBegin();
	int getInputEnd();
	int getHiddenBegin();
	int getHiddenEnd();
	int getOutputBegin();
	int getOutputEnd();
	void setAdjacency(int from, int to, bool isAdjacent);
	void setAdjacency(int first1, int last1, int first2, int last2, bool isAdjacent);
	void setChangeable(int from, int to, bool isChangeable);
	void setChangeable(int first1, int last1, int first2, int last2, bool isChangeable);

	//重みをランダム値で初期化する
	template <class URNG>
	void randomizeWeights(URNG& randomEngine) {
		std::uniform_real_distribution<double> dist(-0.1, 0.1);
		for (unsigned int y = 0; y < weights.size(); ++y) {
			for (unsigned int x = y + 1; x < weights[y].size(); ++x) {
				weights[y][x] = dist(randomEngine);
			}
		}
	}

	void insertNode(int pos, int size, bool addToBegin);

private:
	//隣接行列
	boost::container::vector<boost::container::vector<bool>> adjacency;

	//重み行列
	std::vector<std::vector<double>> weights;

	//活性化関数
	std::function<double(double)> activation;

	//
	boost::container::vector<boost::container::vector<bool>> changeable;

	int readConnectionNotation(const std::string& position);
};

#endif /* FEEDFORWARDNETWORK_H_ */
