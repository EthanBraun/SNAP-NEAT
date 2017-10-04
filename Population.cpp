#include "Population.h"

Population::Population(){
	innovationNumber = 0;
	organisms = new std::vector<Genome*>();
	speciesList = new std::vector<Species*>();
	innovations = new std::vector<Innovation*>();
}

int Population::getGeneration(){
	return generation;
}

int Population::updateInnovations(MutationType mType, GeneType gType, int input, int output){
	for(int i = 0; i < innovations->size(); i++){
		if(_innovationEqual(innovations->operator[][i], mType, gType, input, output)){
			return innovations->operator[][i]->innovationNumber;
		}
	}

	Innovation* newInnovation = new Innovation();
	newInnovation->innovationNumber = innovationNumber;
	newInnovation->mutationType = mType;
	newInnovation->geneType = gType;
	newInnovation->inputId = input;
	newInnovation->outputId = output;
	innovationNumber += 1;
	
	return newInnovation->innovationNumber;
}

std::vector<Genome*>* Population::getOrganisms(){
	return organisms;
}

std::vector<Species*>* Population::getSpeciesList(){
	return speciesList;
}

std::vector<Innovation*>* Population::getInnovations(){
	return innovations;
}

void Population::initializePopulation(){
	for(int i = 0; i < POPULATION_SIZE; i++){
		Genome* genome = new Genome();
		for(int j = 0; j < GENOME_NUM_INPUT_NODES; j++){
			NodeGene* inputNode = new NodeGene();
			inputNode->innovation = j;
			inputNode->bias = 0.0;
			inputNode->type = NodeType::Input;
			inputNode->enabled = true;

			genome->getNodeKeys()->push_back(j);
			genome->getNodeGenes()->insert(std::pair<int, NodeGene*>(j, inputNode));
		}
		for(int j = 0; j < GENOME_NUM_OUTPUT_NODES; j++){
			NodeGene* outputNode = new NodeGene();
			outputNode->innovation = GENOME_NUM_OUTPUT_NODES + j;
			outputNode->bias = 0.0;
			outputNode->type = NodeType::Output;
			outputNode->enabled = true;

			genome->getNodeKeys()->push_back(GENOME_NUM_OUTPUT_NODES + j);
			genome->getNodeGenes()->insert(std::pair<int, NodeGene*>(GENOME_NUM_OUTPUT_NODES + j, outputNode));
		}
	}
	innovationNumber = GENOME_NUM_INPUT_NODES + GENOME_NUM_OUTPUT_NODES;
}

void Population::speciatePopulation(){

}

void Population::evaluate(void* evaluationFunction){
	// Initialize population
	// For each generation:
	//		Clear innovations and species members
	//		Speciate population
	//		Calculate base/shared fitness
	//		Cull organisms with low fitness
	//		Reproduce to full population and mutate
	//		Randomly select representative genomes for each species

	initializePopulation();
	for(int i = 0; i < POPULATION_MAX_GENERATION; i++){
		innovations->clear();
		for(int j = 0; j < speciesList->size(); j++){
			speciesList->operator[][j]->members->clear();
		}
		speciatePopulation();
	}
}

bool Population::_innovationEqual(Innovation* innovation, MutationType mType, GeneType gType, int input, int output){
	return (innovation->mutationType == mType && innovation->geneType == gType && innovation->inputId == input && innovation->outputId == output);
}

double Population::_calculateCompatibilityDistance(Genome* a, Genome* b){
	int excessGenes = 0;
	int disjointGenes = 0;
	int maxTotalGenomeSize = std::max(a->getNodeKeys()->size() + a->getConnectionKeys()->size(), b->getNodeKeys()->size() + b->getConnectionKeys()->size());
	int compatibilityDistance = 0;

	// TODO: optimize this function once engine is working

	excessGenes += std::abs((int)a->getNodeKeys()->size() - (int)b->getNodeKeys()->size());
	excessGenes += std::abs((int)a->getConnectionKeys()->size() - (int)b->getConnectionKeys()->size());

	int minNodeGeneSize = std::min(a->getNodeKeys()->size(), b->getNodeKeys()->size());
	for(int i = 0; i < minNodeGeneSize; i++){
		if(std::find(b->getNodeKeys()->begin(), b->getNodeKeys()->end(), a->getNodeKeys()->operator[][i]) == b->getNodeKeys()->end()){
			disjointGenes++;
		}
		if(std::find(a->getNodeKeys()->begin(), a->getNodeKeys()->end(), b->getNodeKeys()->operator[][i]) == a->getNodeKeys()->end()){
			disjointGenes++;
		}
	}
	
	int matchingConnectionGeneCount = 0;
	double matchingGeneWeightSum = 0.0;

	int minConnectionGeneSize = std::min(a->getConnectionKeys()->size(), b->getConnectionKeys()->size());
	for(int i = 0; i < minConnectionGeneSize; i++){
		if(std::find(b->getConnectionKeys()->begin(), b->getConnectionKeys()->end(), a->getConnectionKeys()->operator[][i]) == b->getConnectionKeys()->end()){
			disjointGenes++;
		}
		if(std::find(a->getConnectionKeys()->begin(), a->getConnectionKeys()->end(), b->getConnectionKeys()->operator[][i]) == a->getConnectionKeys()->end()){
			disjointGenes++;
		}
	}

	compatibilityDistance += (GENOME_COMPATIBILITY_COEFFICIENT_ONE * excessGenes) / maxTotalGenomeSize;
	compatibilityDistance += (GENOME_COMPATIBILITY_COEFFICIENT_TWO * disjointGenes) / maxTotalGenomeSize;
	compatibilityDistance += GENOME_COMPATIBILITY_COEFFICIENT_THREE * 
}