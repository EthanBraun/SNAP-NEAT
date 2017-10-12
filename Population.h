#ifndef POPULATION_H
#define POPULATION_H

#include "Network.h"
#include "Mutation.h"
#include <algorithm>
#include <math.h>

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
	void setSpeciesReps();
	void evaluatePopulation(void* evaluationFunction(Network* network));
	void evaluateGenome(void* evaluationFunction(Network* network), Genome*);
	bool _innovationEqual(Innovation*, MutationType, GeneType, int, int);
	double calculateCompatibilityDistance(Genome*, Genome*);
};

#endif