#include "Genome.h"

Genome::Genome(){
	nodeKeys = new std::vector<int>();
	connectionKeys = new std::vector<int>();
	nodeGenes = new std::map<int, NodeGene*>();
	connectionGenes = new std::map<int, ConnectionGene*>();
	species = -1;
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