#include "Genome.h"
#include "Population.h"
#include <limits.h>

Genome::Genome(){
	hiddenNodeKeys = new std::vector<int>();
	rbfNodeKeys = new std::vector<int>();
	cascadeNodeKeys = new std::vector<int>();
	nodeKeys = new std::vector<int>();
	connectionKeys = new std::vector<int>();
	nodeGenes = new std::map<int, NodeGene*>();
	connectionGenes = new std::map<int, ConnectionGene*>();
	species = -1;
	fitness = 0.0;
	sharedFitness = 0.0;
	elite = false;
	recentCoreOp = -1;
}

// Copy constructor
Genome::Genome(Genome* parent){
	hiddenNodeKeys = new std::vector<int>();
	rbfNodeKeys = new std::vector<int>();
	cascadeNodeKeys = new std::vector<int>();
	nodeKeys = new std::vector<int>();
	connectionKeys = new std::vector<int>();
	nodeGenes = new std::map<int, NodeGene*>();
	connectionGenes = new std::map<int, ConnectionGene*>();
	species = parent->getSpecies();
	fitness = 0.0;
	sharedFitness = 0.0;
	elite = false;
	recentCoreOp = parent->getRecentCoreOp();

	for(int i = 0; i < parent->getHiddenNodeKeys()->size(); i++){
		hiddenNodeKeys->push_back(parent->getHiddenNodeKeys()->operator[](i));
	}
	for(int i = 0; i < parent->getRbfNodeKeys()->size(); i++){
		rbfNodeKeys->push_back(parent->getRbfNodeKeys()->operator[](i));
	}
	for(int i = 0; i < parent->getCascadeNodeKeys()->size(); i++){
		cascadeNodeKeys->push_back(parent->getCascadeNodeKeys()->operator[](i));
	}
	for(int i = 0; i < parent->getNodeKeys()->size(); i++){
		nodeKeys->push_back(parent->getNodeKeys()->operator[](i));

		NodeGene* newNode = new NodeGene();
		newNode->innovation = parent->getNodeGenes()->operator[](parent->getNodeKeys()->operator[](i))->innovation;
		newNode->type = parent->getNodeGenes()->operator[](parent->getNodeKeys()->operator[](i))->type;
		newNode->enabled = parent->getNodeGenes()->operator[](parent->getNodeKeys()->operator[](i))->enabled;
		newNode->rbf = parent->getNodeGenes()->operator[](parent->getNodeKeys()->operator[](i))->rbf;
		newNode->center = parent->getNodeGenes()->operator[](parent->getNodeKeys()->operator[](i))->center;
		newNode->radius = parent->getNodeGenes()->operator[](parent->getNodeKeys()->operator[](i))->radius;
		newNode->cascade = parent->getNodeGenes()->operator[](parent->getNodeKeys()->operator[](i))->cascade;
		nodeGenes->insert(std::pair<int, NodeGene*>(newNode->innovation, newNode));
	}
	for(int i = 0; i < parent->getConnectionKeys()->size(); i++){
		connectionKeys->push_back(parent->getConnectionKeys()->operator[](i));

		ConnectionGene* newConnection = new ConnectionGene();
		newConnection->innovation = parent->getConnectionGenes()->operator[](parent->getConnectionKeys()->operator[](i))->innovation;
		newConnection->inputId = parent->getConnectionGenes()->operator[](parent->getConnectionKeys()->operator[](i))->inputId;
		newConnection->outputId = parent->getConnectionGenes()->operator[](parent->getConnectionKeys()->operator[](i))->outputId;
		newConnection->weight = parent->getConnectionGenes()->operator[](parent->getConnectionKeys()->operator[](i))->weight;
		newConnection->enabled = parent->getConnectionGenes()->operator[](parent->getConnectionKeys()->operator[](i))->enabled;
		connectionGenes->insert(std::pair<int, ConnectionGene*>(newConnection->innovation, newConnection));
	}
}

// Crossover constructor
Genome::Genome(Genome* parentA, Genome* parentB){
	hiddenNodeKeys = new std::vector<int>();
	rbfNodeKeys = new std::vector<int>();
	cascadeNodeKeys = new std::vector<int>();
	nodeKeys = new std::vector<int>();
	connectionKeys = new std::vector<int>();
	nodeGenes = new std::map<int, NodeGene*>();
	connectionGenes = new std::map<int, ConnectionGene*>();
	species = (rand() % 2) == 0 ? parentA->getSpecies() : parentB->getSpecies();
	fitness = 0.0;
	sharedFitness = 0.0;
	elite = false;
	recentCoreOp = -1;

	int currentNodeKeyIndexA = 0;
	int currentNodeKeyIndexB = 0;
	int maxNodeKeyIndexA = parentA->getNodeKeys()->size() - 1;
	int maxNodeKeyIndexB = parentB->getNodeKeys()->size() - 1;
	long currentNodeKeyA = 0;
	long currentNodeKeyB = 0;

	while(true){
		if((currentNodeKeyIndexA > maxNodeKeyIndexA) && (currentNodeKeyIndexB > maxNodeKeyIndexB)){
			break;
		}
		currentNodeKeyA = currentNodeKeyIndexA <= maxNodeKeyIndexA ? parentA->getNodeKeys()->operator[](currentNodeKeyIndexA) : LONG_MAX;
		currentNodeKeyB = currentNodeKeyIndexB <= maxNodeKeyIndexB ? parentB->getNodeKeys()->operator[](currentNodeKeyIndexB) : LONG_MAX;

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
	long currentConnectionKeyA = 0;
	long currentConnectionKeyB = 0;

	while(true){
		if((currentConnectionKeyIndexA > maxConnectionKeyIndexA) && (currentConnectionKeyIndexB > maxConnectionKeyIndexB)){
			break;
		}
		currentConnectionKeyA = currentConnectionKeyIndexA <= maxConnectionKeyIndexA ? parentA->getConnectionKeys()->operator[](currentConnectionKeyIndexA) : LONG_MAX;
		currentConnectionKeyB = currentConnectionKeyIndexB <= maxConnectionKeyIndexB ? parentB->getConnectionKeys()->operator[](currentConnectionKeyIndexB) : LONG_MAX;

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
	for(std::map<int, NodeGene*>::iterator itr = nodeGenes->begin(); itr != nodeGenes->end(); itr++){
		delete itr->second;
	}

	for(std::map<int, ConnectionGene*>::iterator itr = connectionGenes->begin(); itr != connectionGenes->end(); itr++){
		delete itr->second;
	}


	//printf("Freeing hiddenNodeKeys\n");
	delete hiddenNodeKeys;
	delete rbfNodeKeys;
	delete cascadeNodeKeys;
	//printf("Freeing nodeKeys\n");
	delete nodeKeys;
	//printf("Freeing connectionKeys\n");
	delete connectionKeys;
	//printf("Freeing nodeGenes\n");
	delete nodeGenes;
	//printf("Freeing connectionGenes\n");
	delete connectionGenes;
}

std::vector<int>* Genome::getHiddenNodeKeys(){
	return hiddenNodeKeys;
}

std::vector<int>* Genome::getRbfNodeKeys(){
	return rbfNodeKeys;
}

std::vector<int>* Genome::getCascadeNodeKeys(){
	return cascadeNodeKeys;
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

long Genome::getId(){
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

bool Genome::getElite(){
	return elite;
}

int Genome::getRecentCoreOp(){
	return recentCoreOp;
}

// Check if genome has a cascade node that is enabled
bool Genome::hasActiveCascade(){
	for(int i = 0; i < this->getCascadeNodeKeys()->size(); i++){
		if(this->getNodeGenes()->operator[](this->getCascadeNodeKeys()->operator[](i))->enabled){
			return true;
		}
	}
	return false;
}

// Gather and return a vector of innovations for connections to and from the active cascade node
//    (only call if hasActiveCascade() returns true)
std::vector<int> Genome::getActiveCascadeConnections(){
	int activeCascadeInnovation;
	std::vector<int> cascadeConnections;
	ConnectionGene* currentConnection;

	// Determine most recently added cascade node that is enabled
	for(int i = this->getCascadeNodeKeys()->size() - 1; i >= 0; i--){
		if(this->getNodeGenes()->operator[](this->getCascadeNodeKeys()->operator[](i))->enabled){
			activeCascadeInnovation = this->getCascadeNodeKeys()->operator[](i);
			break;
		}
	}

	// Gather vector of connections to and from active cascade node
	for(int i = 0; i < this->getConnectionKeys()->size(); i++){
		currentConnection = this->getConnectionGenes()->operator[](this->getConnectionKeys()->operator[](i));
		if((currentConnection->inputId == activeCascadeInnovation) || (currentConnection->outputId == activeCascadeInnovation)){
			cascadeConnections.push_back(currentConnection->innovation);
		}
	}
	return cascadeConnections;
}

void Genome::setId(long idVal){
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

void Genome::setElite(bool eliteVal){
	elite = eliteVal;
}

void Genome::setRecentCoreOp(int opVal){
	recentCoreOp = opVal;
}

void Genome::printGenotype(){
	printf("Node Genes -- (%d -- %d):\n", (int)nodeKeys->size(), (int)nodeGenes->size());
	for(int i = 0; i < nodeKeys->size(); i++){
		printf("\tnodeGenes[%d]->innovation: %d\n", nodeKeys->operator[](i), nodeGenes->operator[](nodeKeys->operator[](i))->innovation);
		printf("\tnodeGenes[%d]->type: %d\n", nodeKeys->operator[](i), nodeGenes->operator[](nodeKeys->operator[](i))->type);
		printf("\tnodeGenes[%d]->enabled: %d\n", nodeKeys->operator[](i), nodeGenes->operator[](nodeKeys->operator[](i))->enabled);
		if(nodeGenes->operator[](nodeKeys->operator[](i))->rbf){
			printf("\tnodeGenes[%d]->rbf: %d\n", nodeKeys->operator[](i), nodeGenes->operator[](nodeKeys->operator[](i))->rbf);
			printf("\tnodeGenes[%d]->center: %f\n", nodeKeys->operator[](i), nodeGenes->operator[](nodeKeys->operator[](i))->center);
			printf("\tnodeGenes[%d]->radius: %f\n", nodeKeys->operator[](i), nodeGenes->operator[](nodeKeys->operator[](i))->radius);
		}
		if(nodeGenes->operator[](nodeKeys->operator[](i))->cascade){
			printf("\tnodeGenes[%d]->cascade: %d\n", nodeKeys->operator[](i), nodeGenes->operator[](nodeKeys->operator[](i))->cascade);
		}
		printf("\n");
	}

	printf("Connection Genes -- (%d -- %d):\n", (int)connectionKeys->size(), (int)connectionGenes->size());
	for(int i = 0; i < connectionKeys->size(); i++){
		printf("\tconnectionGenes[%d]->innovation: %d\n", connectionKeys->operator[](i), connectionGenes->operator[](connectionKeys->operator[](i))->innovation);
		printf("\tconnectionGenes[%d]->inputId: %d\n", connectionKeys->operator[](i), connectionGenes->operator[](connectionKeys->operator[](i))->inputId);
		printf("\tconnectionGenes[%d]->outputId: %d\n", connectionKeys->operator[](i), connectionGenes->operator[](connectionKeys->operator[](i))->outputId);
		printf("\tconnectionGenes[%d]->weight: %f\n", connectionKeys->operator[](i), connectionGenes->operator[](connectionKeys->operator[](i))->weight);
		printf("\tconnectionGenes[%d]->enabled: %d\n\n", connectionKeys->operator[](i), connectionGenes->operator[](connectionKeys->operator[](i))->enabled);
	}
	fflush(stdout);
}

bool Genome::operator==(const Genome& rhs){
	return id == rhs.id;
}
