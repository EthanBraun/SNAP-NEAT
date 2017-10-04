#ifndef POPULATION_H
#define POPULATION_H

#include "Network.h"
#include "Mutation.h"
#include <algorithm>

struct Species{
	Genome* representative;
	std::vector<Genome*>* members;
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
	int generation;
	int innovationNumber;
    std::vector<Genome*>* organisms;
	std::vector<Species*>* speciesList;
	std::vector<Innovation*>* innovations;

public:
    Population();

	int updateInnovations(MutationType, GeneType, int, int);
	int getGeneration();
	std::vector<Genome*>* getOrganisms();
	std::vector<Species*>* getSpeciesList();
	std::vector<Innovation*>* getInnovations();
	void initializePopulation();
	void speciatePopulation();
	void evaluate(void*);
	bool _innovationEqual(Innovation*, MutationType, GeneType, int, int);
	double _calculateCompatibilityDistance(Genome*, Genome*);
};

#endif