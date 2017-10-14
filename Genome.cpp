#include "Genome.h"

Genome::Genome(){
	hiddenNodeKeys = new std::vector<int>();
	nodeKeys = new std::vector<int>();
	connectionKeys = new std::vector<int>();
	nodeGenes = new std::map<int, NodeGene*>();
	connectionGenes = new std::map<int, ConnectionGene*>();
	species = -1;
	fitness = 0.0;
	sharedFitness = 0.0;
}

Genome::Genome(Genome* parentA, Genome* parentB){
	hiddenNodeKeys = new std::vector<int>();
	nodeKeys = new std::vector<int>();
	connectionKeys = new std::vector<int>();
	nodeGenes = new std::map<int, NodeGene*>();
	connectionGenes = new std::map<int, ConnectionGene*>();
	species = (rand() % 2) == 0 ? parentA->getSpecies() : parentB->getSpecies();
	fitness = 0.0;
	sharedFitness = 0.0;

	int currentNodeKeyIndexA = 0;
	int currentNodeKeyIndexB = 0;
	int maxNodeKeyIndexA = parentA->getNodeKeys()->size() - 1;
	int maxNodeKeyIndexB = parentB->getNodeKeys()->size() - 1;
	int currentNodeKeyA = 0;
	int currentNodeKeyB = 0;

	while(true){
		if((currentNodeKeyIndexA > maxNodeKeyIndexA) && (currentNodeKeyIndexB > maxNodeKeyIndexB)){
			break;
		}
		currentNodeKeyA = currentNodeKeyIndexA <= maxNodeKeyIndexA ? parentA->getNodeKeys()->operator[](currentNodeKeyIndexA) : 100000;
		currentNodeKeyB = currentNodeKeyIndexB <= maxNodeKeyIndexB ? parentB->getNodeKeys()->operator[](currentNodeKeyIndexB) : 100000;

		if(currentNodeKeyA == currentNodeKeyB){
			Population::copyNodeGeneBernoulli(this, parentA->getNodeGenes()->operator[](currentNodeKeyA), parentB->getNodeGenes()->operator[](currentNodeKeyB), currentNodeKeyA);
			currentNodeKeyIndexA++;
			currentNodeKeyIndexB++;
		}
		else if((currentNodeKeyA < currentNodeKeyB) || (currentNodeKeyIndexB > maxNodeKeyIndexB)){
			if(parentA->getFitness() == parentB->getFitness()){
				Population::copyNodeGeneBernoulli(this, parentA->getNodeGenes()->operator[](currentNodeKeyA), currentNodeKeyA);
			}
			else if(parentA->getFitness() > parentB->getFitness()){
				Population::copyNodeGene(this, parentA->getNodeGenes()->operator[](currentNodeKeyA), currentNodeKeyA);
			}
			currentNodeKeyIndexA++;
		}
		else if((currentNodeKeyA > currentNodeKeyB) || (currentNodeKeyIndexA > maxNodeKeyIndexA)){
			if(parentA->getFitness() == parentB->getFitness()){
				Population::copyNodeGeneBernoulli(this, parentB->getNodeGenes()->operator[](currentNodeKeyB), currentNodeKeyB);
			}
			else if(parentA->getFitness() < parentB->getFitness()){
				Population::copyNodeGene(this, parentB->getNodeGenes()->operator[](currentNodeKeyB), currentNodeKeyB);
			}
			currentNodeKeyIndexB++;
		}
	}

	int currentConnectionKeyIndexA = 0;
	int currentConnectionKeyIndexB = 0;
	int maxConnectionKeyIndexA = parentA->getConnectionKeys()->size() - 1;
	int maxConnectionKeyIndexB = parentB->getConnectionKeys()->size() - 1;
	int currentConnectionKeyA = 0;
	int currentConnectionKeyB = 0;

	while(true){
		if((currentConnectionKeyIndexA > maxConnectionKeyIndexA) && (currentConnectionKeyIndexB > maxConnectionKeyIndexB)){
			break;
		}
		currentConnectionKeyA = currentConnectionKeyIndexA <= maxConnectionKeyIndexA ? parentA->getConnectionKeys()->operator[](currentConnectionKeyIndexA) : 100000;
		currentConnectionKeyB = currentConnectionKeyIndexB <= maxConnectionKeyIndexB ? parentB->getConnectionKeys()->operator[](currentConnectionKeyIndexB) : 100000;

		if(currentConnectionKeyA == currentConnectionKeyB){
			Population::copyConnectionGeneBernoulli(this, parentA->getConnectionGenes()->operator[](currentConnectionKeyA), parentB->getConnectionGenes()->operator[](currentConnectionKeyB), currentConnectionKeyA);
			currentConnectionKeyIndexA++;
			currentConnectionKeyIndexB++;
		}
		else if((currentConnectionKeyA < currentConnectionKeyB) || (currentConnectionKeyIndexB > maxConnectionKeyIndexB)){
			if(parentA->getFitness() == parentB->getFitness()){
				Population::copyConnectionGeneBernoulli(this, parentA->getConnectionGenes()->operator[](currentConnectionKeyA), currentConnectionKeyA);
			}
			else if(parentA->getFitness() > parentB->getFitness()){
				Population::copyConnectionGene(this, parentA->getConnectionGenes()->operator[](currentConnectionKeyA), currentConnectionKeyA);
			}
			currentConnectionKeyIndexA++;
		}
		else if((currentConnectionKeyA > currentConnectionKeyB) || (currentConnectionKeyIndexA > maxConnectionKeyIndexA)){
			if(parentA->getFitness() == parentB->getFitness()){
				Population::copyConnectionGeneBernoulli(this, parentB->getConnectionGenes()->operator[](currentConnectionKeyB), currentConnectionKeyB);
			}
			else if(parentA->getFitness() < parentB->getFitness()){
				Population::copyConnectionGene(this, parentB->getConnectionGenes()->operator[](currentConnectionKeyB), currentConnectionKeyB);
			}
			currentConnectionKeyIndexB++;
		}
	}
}

Genome::~Genome(){
	for(int i = 0; i < nodeKeys->size(); i++){
		delete nodeGenes->operator[](nodeKeys->operator[](i));
	}
	for(int i = 0; i < connectionKeys->size(); i++){
		delete connectionGenes->operator[](connectionKeys->operator[](i));
	}
	delete hiddenNodeKeys;
	delete nodeKeys;
	delete connectionKeys;
	delete nodeGenes;
	delete connectionGenes;
}

std::vector<int>* Genome::getHiddenNodeKeys(){
	return hiddenNodeKeys;
}

std::vector<int>* Genome::getNodeKeys(){
	return nodeKeys;
}

std::vector<int>* Genome::getConnectionKeys(){
	return connectionKeys;
}

std::map<int, NodeGene*>* Genome::getNodeGenes(){
	return nodeGenes;
}

std::map<int, ConnectionGene*>* Genome::getConnectionGenes(){
	return connectionGenes;
}

int Genome::getId(){
	return id;
}

int Genome::getSpecies(){
	return species;
}

double Genome::getFitness(){
	return fitness;
}

double Genome::getSharedFitness(){
	return sharedFitness;
}

void Genome::setId(int idVal){
	id = idVal;
}

void Genome::setSpecies(int speciesVal){
	species = speciesVal;
}

void Genome::setFitness(double fitnessVal){
	fitness = fitnessVal;
}

void Genome::setSharedFitness(double sharedFitnessVal){
	sharedFitness = sharedFitnessVal;
}

bool Genome::operator==(const Genome& rhs){
	return id == rhs.id;
}