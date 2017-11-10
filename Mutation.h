#ifndef MUTATION_H
#define MUTATION_H

#include "Config.h"
#include "Genome.h"
#include "Population.h"

class Mutation{
public:
	static void mutate(Genome*, Population*);
	static void addNode(Genome*, Population*);
	static void addConnection(Genome*, Population*);
	static void perturbWeight(Genome*, Population*);
	static void toggleNode(Genome*);
	static void toggleConnection(Genome*);
	static void removeNode(Genome*);
	static void removeConnection(Genome*);
};

#endif