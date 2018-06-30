#ifndef GENOME_H
#define GENOME_H

#include "NodeType.h"
#include <vector>
#include <map>

struct NodeGene {
	int innovation;
	NodeType type;
	bool enabled;
	// RBF-NEAT parameters
	bool rbf;
	float center;
	float radius;
	// Cascade-NEAT parameter
	bool cascade;
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
	std::vector<int>* rbfNodeKeys;
	std::vector<int>* cascadeNodeKeys;
	std::vector<int>* nodeKeys;
	std::vector<int>* connectionKeys;
	std::map<int, NodeGene*>* nodeGenes;
	std::map<int, ConnectionGene*>* connectionGenes;
	int species;
	double fitness;
	double sharedFitness;
	bool elite;
	int recentCoreOp;
public:
	Genome();
	Genome(Genome*);
	Genome(Genome*, Genome*);
	~Genome();

	std::vector<int>* getHiddenNodeKeys();
	std::vector<int>* getRbfNodeKeys();
	std::vector<int>* getCascadeNodeKeys();
	std::vector<int>* getNodeKeys();
	std::vector<int>* getConnectionKeys();
	std::map<int, NodeGene*>* getNodeGenes();
	std::map<int, ConnectionGene*>* getConnectionGenes();
	long getId();
	int getSpecies();
	double getFitness();
	double getSharedFitness();
	bool getElite();
	int getRecentCoreOp();
	bool hasActiveCascade();
	std::vector<int> getActiveCascadeConnections();
	void setId(long);
	void setSpecies(int);
	void setFitness(double);
	void setSharedFitness(double);
	void setElite(bool);
	void setRecentCoreOp(int);
	void printGenotype();
	bool operator==(const Genome&);
};

#endif
