#ifndef MUTATION_H
#define MUTATION_H

#include "Config.h"
#include "Genome.h"
#include "Population.h"

class Mutation{
public:
	// NEAT mutations
	static void mutate(Genome*, Population*);
	static void addNode(Genome*, Population*);
	static void addConnection(Genome*, Population*);
	static void perturbWeight(Genome*, Population*);
	static void toggleNode(Genome*);
	static void toggleConnection(Genome*);
	static void removeNode(Genome*);
	static void removeConnection(Genome*);

	// RBF-NEAT mutations
	static void addRbfNode(Genome*, Population*);
	static void perturbRbfNode(Genome*, Population*);

	// Cascade-NEAT mutation
	static void addCascadeNode(Genome*, Population*);
};

void _ordInsert(std::vector<int>*, int);

#endif
