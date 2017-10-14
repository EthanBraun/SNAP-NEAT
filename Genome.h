#ifndef GENOME_H
#define GENOME_H

#include "NodeType.h"
#include <vector>
#include <map>

struct NodeGene {
	int innovation;
	double bias;
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
	int id;
	std::vector<int>* hiddenNodeKeys; 
	std::vector<int>* nodeKeys;
	std::vector<int>* connectionKeys;
	std::map<int, NodeGene*>* nodeGenes;
	std::map<int, ConnectionGene*>* connectionGenes;
	int species;
	double fitness;
	double sharedFitness;
public:
	Genome();
	Genome(Genome*, Genome*);
	~Genome();

	std::vector<int>* getHiddenNodeKeys();
	std::vector<int>* getNodeKeys();
	std::vector<int>* getConnectionKeys();
	std::map<int, NodeGene*>* getNodeGenes();
	std::map<int, ConnectionGene*>* getConnectionGenes();
	int getId();
	int getSpecies();
	double getFitness();
	double getSharedFitness();
	void setId(int);
	void setSpecies(int);
	void setFitness(double);
	void setSharedFitness(double);
	bool operator==(const Genome&);
};

#endif