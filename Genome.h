#ifndef GENOME_H
#define GENOME_H

#include "NodeType.h"
#include <vector>
#include <map>

struct NodeGene {
	int innovation;
	NodeType type;
	bool enabled;
};

struct ConnectionGene {
	int innovation;
	int inputId;
	int outputId;
	double weight;
	bool enabled;
};

class Genome {
private:
	long id;
	std::vector<int>* hiddenNodeKeys; 
	std::vector<int>* nodeKeys;
	std::vector<int>* connectionKeys;
	std::map<int, NodeGene*>* nodeGenes;
	std::map<int, ConnectionGene*>* connectionGenes;
	int species;
	double fitness;
	double sharedFitness;
	bool elite;
public:
	Genome();
	Genome(Genome*);
	Genome(Genome*, Genome*);
	~Genome();

	std::vector<int>* getHiddenNodeKeys();
	std::vector<int>* getNodeKeys();
	std::vector<int>* getConnectionKeys();
	std::map<int, NodeGene*>* getNodeGenes();
	std::map<int, ConnectionGene*>* getConnectionGenes();
	long getId();
	int getSpecies();
	double getFitness();
	double getSharedFitness();
	bool getElite();
	void setId(long);
	void setSpecies(int);
	void setFitness(double);
	void setSharedFitness(double);
	void setElite(bool);
	void printGenotype();
	bool operator==(const Genome&);
};

#endif