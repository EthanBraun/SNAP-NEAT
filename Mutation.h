#ifndef MUTATION_H
#define MUTATION_H

#include "Config.h"
#include "Genome.h"

enum MutationType { AddNode, AddConnection, PerturbBias, PerturbWeight, ToggleNode, ToggleConnection, End = ToggleConnection };

class Mutation{
public:
	static void mutate(Genome*);
	static void addNode(Genome*);
	static void addConnection(Genome*);
	static void perturbBias(Genome*);
	static void perturbWeight(Genome*);
	static void toggleNode(Genome*);
	static void toggleConnection(Genome*);
};
#endif