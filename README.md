# Game Solver Library {#mainpage}
[WIP]
Extensible game solver/optimizer library for c++, mainly focused on genetic algorithm.

## Dependencies
<https://github.com/kryozahiro/cpputil>

## Features
Game
- AgentGame
- AverageAdaptor
- Problem (as a kind of game that each player can be evaluated separately and absolutely)
	- HomoAdaptor (game that played by same, copied players)
	- Regression
	- SantaFeTrail
	- TravellingSalesman

Program (representation of solution)
- IoMapping (simple table)
- FeedforwardNetwork (neural network)
- ExpressionTree
- InstructionSequence (pseudo machine code)

Solver
- GeneticAlgorithm
- SimulatedAnnealing
