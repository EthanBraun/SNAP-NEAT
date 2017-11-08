#include "Genome.h"
#include "Population.h"

Genome::Genome(){
	hiddenNodeKeys = new std::vector<int>();
	nodeKeys = new std::vector<int>();
	connectionKeys = new std::vector<int>();
	nodeGenes = new std::map<int, NodeGene*>();
	connectionGenes = new std::map<int, ConnectionGene*>();
	species = -1;
	fitness = 0.0;
	sharedFitness = 0.0;
	elite = false;
}

// Copy constructor
Genome::Genome(Genome* parent){
	hiddenNodeKeys = new std::vector<int>();
	nodeKeys = new std::vector<int>();
	connectionKeys = new std::vector<int>();
	nodeGenes = new std::map<int, NodeGene*>();
	connectionGenes = new std::map<int, ConnectionGene*>();
	species = parent->getSpecies();
	fitness = 0.0;
	sharedFitness = 0.0;
	elite = false;

	for(int i = 0; i < parent->getHiddenNodeKeys()->size(); i++){
		hiddenNodeKeys->push_back(parent->getHiddenNodeKeys()->operator[](i));
	}
	for(int i = 0; i < parent->getNodeKeys()->size(); i++){
		nodeKeys->push_back(parent->getNodeKeys()->operator[](i));

		NodeGene* newNode = new NodeGene();
		newNode->innovation = parent->getNodeGenes()->operator[](parent->getNodeKeys()->operator[](i))->innovation;
		newNode->type = parent->getNodeGenes()->operator[](parent->getNodeKeys()->operator[](i))->type;
		newNode->enabled = parent->getNodeGenes()->operator[](parent->getNodeKeys()->operator[](i))->enabled;
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
	nodeKeys = new std::vector<int>();
	connectionKeys = new std::vector<int>();
	nodeGenes = new std::map<int, NodeGene*>();
	connectionGenes = new std::map<int, ConnectionGene*>();
	species = (rand() % 2) == 0 ? parentA->getSpecies() : parentB->getSpecies();
	fitness = 0.0;
	sharedFitness = 0.0;
	elite = false;

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
		currentNodeKeyA = currentNodeKeyIndexA <= maxNodeKeyIndexA ? parentA->getNodeKeys()->operator[](currentNodeKeyIndexA) : 1000000;
		currentNodeKeyB = currentNodeKeyIndexB <= maxNodeKeyIndexB ? parentB->getNodeKeys()->operator[](currentNodeKeyIndexB) : 1000000;

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
	//printf("Freeing individual node genes\n");
	//for(int i = 0; i < nodeKeys->size(); i++){
	//	delete nodeGenes->operator[](nodeKeys->operator[](i));
	//}
	//printf("Freeing individual connection genes\n");
	//for(int i = 0; i < connectionKeys->size(); i++){
	//	delete connectionGenes->operator[](connectionKeys->operator[](i));
	//}
	for(std::map<int, NodeGene*>::iterator itr = nodeGenes->begin(); itr != nodeGenes->end(); itr++){
		delete itr->second;
	}

	for(std::map<int, ConnectionGene*>::iterator itr = connectionGenes->begin(); itr != connectionGenes->end(); itr++){
		delete itr->second;
	}


	//printf("Freeing hiddenNodeKeys\n");
	delete hiddenNodeKeys;
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

void Genome::printGenotype(){
	printf("Node Genes -- (%d):\n", (int)nodeKeys->size());
	for(int i = 0; i < nodeKeys->size(); i++){
		printf("\tnodeGenes[%d]->innovation: %d\n", nodeKeys->operator[](i), nodeGenes->operator[](nodeKeys->operator[](i))->innovation);
		printf("\tnodeGenes[%d]->type: %d\n", nodeKeys->operator[](i), nodeGenes->operator[](nodeKeys->operator[](i))->type);
		printf("\tnodeGenes[%d]->enabled: %d\n\n", nodeKeys->operator[](i), nodeGenes->operator[](nodeKeys->operator[](i))->enabled);
	}

	printf("Connection Genes -- (%d):\n", (int)connectionKeys->size());
	for(int i = 0; i < connectionKeys->size(); i++){
		printf("\tconnectionGenes[%d]->innovation: %d\n", connectionKeys->operator[](i), connectionGenes->operator[](connectionKeys->operator[](i))->innovation);
		printf("\tconnectionGenes[%d]->inputId: %d\n", connectionKeys->operator[](i), connectionGenes->operator[](connectionKeys->operator[](i))->inputId);
		printf("\tconnectionGenes[%d]->outputId: %d\n", connectionKeys->operator[](i), connectionGenes->operator[](connectionKeys->operator[](i))->outputId);
		printf("\tconnectionGenes[%d]->weight: %f\n", connectionKeys->operator[](i), connectionGenes->operator[](connectionKeys->operator[](i))->weight);
		printf("\tconnectionGenes[%d]->enabled: %d\n\n", connectionKeys->operator[](i), connectionGenes->operator[](connectionKeys->operator[](i))->enabled);
	}
	fflush(stdout);
	//exit(420);
}

bool Genome::operator==(const Genome& rhs){
	return id == rhs.id;
}