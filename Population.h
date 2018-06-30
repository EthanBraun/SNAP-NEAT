#ifndef POPULATION_H
#define POPULATION_H

#include "Network.h"
#include <algorithm>
#include <math.h>
#include <cstdio>
#include <cstdlib>

enum MutationType { AddNode, AddConnection, PerturbWeight, ToggleNode, ToggleConnection, RemoveNode, RemoveConnection, AddRbfNode, PerturbRbfNode, AddCascadeNode};
enum GeneType { GeneTypeConnection, GeneTypeNode };
enum CoreMutation { Neat, Rbf, Cascade };
enum AltMutation { AmToggleNode, AmToggleConnection, AmRemoveNode, AmRemoveConnection, AmPerturbRbfNode, End = AmPerturbRbfNode }; 

struct Species{
	double averageFitness;
	Genome* representative;
	std::vector<Genome*>* members;
	int spawnRate;
	int cullRate;
	int stagnation;
	bool stagnant;
	bool exceedsMaxStagnation;
	double currentMaxFitness;
	double globalMaxFitness;
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
	long genomeId;
	int innovationNumber;
	double speciesAverageFitnessSum;
	std::vector<Genome*>* organisms;
	std::vector<Species*>* speciesList;
	std::vector<Innovation*>* innovations;
	Config* config;
	double maxFitness;
	bool populationStagnant;
	// SNAP-NEAT variables
	long evaluations;
	std::vector<double> neatRewards;
	std::vector<double> rbfRewards;
	std::vector<double> cascadeRewards;
	std::vector<float> operatorValues;
	std::vector<float> operatorProbs;

public:
	Population(Config*);
	~Population();

	void resetPopulation();
	long updateGenomeId();
	int updateInnovations(MutationType, GeneType, int, int);
	std::vector<Genome*>* getOrganisms();
	std::vector<Species*>* getSpeciesList();
	std::vector<Innovation*>* getInnovations();
	Config* getConfig();
	void initializePopulation();
	void checkSpeciesStagnation();
	void speciatePopulation();
	void adjustGenomeCompatibilityThreshold();
	void calculateSpeciesFitnesses();
	void checkPopulationStagnation();
	void calculateSpeciesSizeChanges();
	void reducePopulation();
	void updateElites();
	void repopulate();
	void removeEmptySpecies();
	void setSpeciesReps();
	double evaluatePopulation(void* evaluationFunction(Network* network, double* fitness, void*), void*);
	bool evaluateGenome(void* evaluationFunction(Network* network, double* fitness, void* data), Genome*, void*);
	void printPopulationStats();
	bool _innovationEqual(Innovation*, MutationType, GeneType, int, int);
	double calculateCompatibilityDistance(Genome*, Genome*);
	static void copyNodeGeneBernoulli(Genome*, NodeGene*, NodeGene*, int);
	static void copyNodeGeneBernoulli(Genome*, NodeGene*, int);
	static void copyNodeGene(Genome*, NodeGene*, int);
	static void copyConnectionGeneBernoulli(Genome*, ConnectionGene*, ConnectionGene*, int);
	static void copyConnectionGeneBernoulli(Genome*, ConnectionGene*, int);
	static void copyConnectionGene(Genome*, ConnectionGene*, int);
	// SNAP-NEAT methods
	bool initialized();
	void setInitialOperatorValues();
	void updateOperatorValue(CoreMutation, double);
	void updateOperatorProbs();
	CoreMutation chooseOpWeighted();
	void printSnapneatStats();
};

double _getMean(std::vector<double>);
double _getStdDev(std::vector<double>, double);

#endif
