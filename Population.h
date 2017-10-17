#ifndef POPULATION_H
#define POPULATION_H

#include "Network.h"
#include <algorithm>
#include <math.h>
#include <cstdio>
#include <cstdlib>

enum MutationType { AddNode, AddConnection, PerturbBias, PerturbWeight, ToggleNode, ToggleConnection, End = ToggleConnection };
enum GeneType { GeneTypeConnection, GeneTypeNode };

struct Species{
	double averageFitness;
	Genome* representative;
	std::vector<Genome*>* members;
	int spawnRate;
	int cullRate;
};

struct Innovation{
	int innovationNumber;
	MutationType mutationType;
	GeneType geneType;
	int inputId;
	int outputId;
};

class Population{
private:
	int genomeId;
	int innovationNumber;
	double speciesAverageFitnessSum;
    std::vector<Genome*>* organisms;
	std::vector<Species*>* speciesList;
	std::vector<Innovation*>* innovations;

public:
    Population();

	int updateGenomeId();
	int updateInnovations(MutationType, GeneType, int, int);
	std::vector<Genome*>* getOrganisms();
	std::vector<Species*>* getSpeciesList();
	std::vector<Innovation*>* getInnovations();
	void initializePopulation();
	void speciatePopulation();
	void calculateSpeciesAverageFitnesses();
	void calculateSpeciesSizeChanges();
	void reducePopulation();
	void repopulate();
	void removeEmptySpecies();
	void setSpeciesReps();
	void evaluatePopulation(void* evaluationFunction(Network* network, double* fitness));
	void evaluateGenome(void* evaluationFunction(Network* network, double* fitness), Genome*);
	void printPopulationStats();
	bool _innovationEqual(Innovation*, MutationType, GeneType, int, int);
	double calculateCompatibilityDistance(Genome*, Genome*);
	static void copyNodeGeneBernoulli(Genome*, NodeGene*, NodeGene*, int);
	static void copyNodeGeneBernoulli(Genome*, NodeGene*, int);
	static void copyNodeGene(Genome*, NodeGene*, int);
	static void copyConnectionGeneBernoulli(Genome*, ConnectionGene*, ConnectionGene*, int);
	static void copyConnectionGeneBernoulli(Genome*, ConnectionGene*, int);
	static void copyConnectionGene(Genome*, ConnectionGene*, int);
};

#endif