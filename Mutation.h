#ifndef MUTATION_H
#define MUTATION_H

#include "Config.h"
#include "Genome.h"
#include "Population.h"

enum MutationType { AddNode, AddConnection, PerturbBias, PerturbWeight, ToggleNode, ToggleConnection, End = ToggleConnection };
enum GeneType { GeneTypeConnection, GeneTypeNode };

class Mutation{
public:
	static void mutate(Genome*, Population*);
	static void addNode(Genome*, Population*);
	static void addConnection(Genome*, Population*);
	static void perturbBias(Genome*);
	static void perturbWeight(Genome*);
	static void toggleNode(Genome*);
	static void toggleConnection(Genome*);
};

#endif