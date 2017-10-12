#include "Population.h"

Population::Population(){
	innovationNumber = 0;
	organisms = new std::vector<Genome*>();
	speciesList = new std::vector<Species*>();
	innovations = new std::vector<Innovation*>();
}

int Population::updateGenomeId(){
	int currentGenomeId = genomeId;
	genomeId++;
	return currentGenomeId;
}

int Population::updateInnovations(MutationType mType, GeneType gType, int input, int output){
	for(int i = 0; i < innovations->size(); i++){
		if(_innovationEqual(innovations->operator[](i), mType, gType, input, output)){
			return innovations->operator[](i)->innovationNumber;
		}
	}

	Innovation* newInnovation = new Innovation();
	newInnovation->innovationNumber = innovationNumber;
	newInnovation->mutationType = mType;
	newInnovation->geneType = gType;
	newInnovation->inputId = input;
	newInnovation->outputId = output;
innovations->push_back(newInnovation);
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
		genome->setId(updateGenomeId());
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
		organisms->push_back(genome);
	}
	innovationNumber = GENOME_NUM_INPUT_NODES + GENOME_NUM_OUTPUT_NODES;
}

void Population::speciatePopulation(){
	// For each organism:
	//		For each species:
	//			If compatibility distance between organism and representative is below threshold:
	//				add organism to species and break
	//		If no compatible species exists:
	//			Create new species with organism as representative
	bool compatibleSpeciesFound;
	for(int i = 0; i < organisms->size(); i++){
		compatibleSpeciesFound = false;
		for(int j = 0; j < speciesList->size(); j++){
			if(calculateCompatibilityDistance(organisms->operator[](i), speciesList->operator[](j)->representative) < GENOME_COMPATIBILITY_THRESHOLD){
				speciesList->operator[](j)->members->push_back(organisms->operator[](i));
				organisms->operator[](i)->setSpecies(j);
				compatibleSpeciesFound = true;
				break;
			}
		}
		if(!compatibleSpeciesFound){
			Species* newSpecies = new Species();
			newSpecies->averageFitness = 0.0;
			newSpecies->representative = organisms->operator[](i);
			newSpecies->members = new std::vector<Genome*>();
			newSpecies->members->push_back(newSpecies->representative);
			newSpecies->spawnRate = 0;
			newSpecies->cullRate = 0;
			speciesList->push_back(newSpecies);
		}
	}
}

void Population::calculateSpeciesAverageFitnesses(){
	speciesAverageFitnessSum = 0.0;
	for(int i = 0; i < speciesList->size(); i++){
		speciesList->operator[](i)->averageFitness = 0.0;
		for(int j = 0; j < speciesList->operator[](i)->members->size(); i++){
			speciesList->operator[](i)->averageFitness += speciesList->operator[](i)->members->operator[](j)->getSharedFitness();
		}
		speciesAverageFitnessSum += speciesList->operator[](i)->averageFitness;
	}
}

void Population::calculateSpeciesSizeChanges(){
	Species* currentSpecies;
	for(int i = 0; i < speciesList->size(); i++){
		currentSpecies = speciesList->operator[](i);
		currentSpecies->spawnRate = (int)round((currentSpecies->averageFitness / speciesAverageFitnessSum) * POPULATION_PURGE_COUNT);
		currentSpecies->cullRate = (int)round((currentSpecies->members->size() / POPULATION_SIZE) * POPULATION_PURGE_COUNT);
	}
}

void Population::reducePopulation(){
	// For each species:
	// Determine n organisms with lowest fitness (where n = cull rate of species)
	// Remove n organisms from members and organisms
	int index;
	for(int i = 0; i < speciesList->size(); i++){
		speciesList->operator[](i)->cullRate;
	}
}

void Population::repopulate(){
	// Use crossover to create new genomes until the population is back to size
}

void Population::setSpeciesReps(){
	// Randomly select representative genomes for each species
	Species* currentSpecies;
	for(int i = 0; i < speciesList->size(); i++){
		currentSpecies = speciesList->operator[](i);
		currentSpecies->representative = currentSpecies->members->operator[](rand() % currentSpecies->members->size());
	}
}

void Population::evaluatePopulation(void* evaluationFunction(Network* network)){
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
		for(int j = 0; j < innovations->size(); j++){
			delete innovations->operator[](j);
		}
		innovations->clear();
		for(int j = 0; j < speciesList->size(); j++){
			speciesList->operator[](j)->members->clear();
		}
		speciatePopulation();
		for(int j = 0; j < organisms->size(); j++){
			evaluateGenome(evaluationFunction, organisms->operator[](j));
		}
		calculateSpeciesAverageFitnesses();
		reducePopulation();
		repopulate();
		for(int j = 0; j < organisms->size(); j++){
			Mutation::mutate(organisms->operator[](j), this);
		}
		setSpeciesReps();
	}
}

void Population::evaluateGenome(void* evaluationFunction(Network* network), Genome* currentGenome){
	Network* phenotype = new Network(currentGenome);
	currentGenome->setFitness(*(double*)(evaluationFunction(phenotype)));
	currentGenome->setSharedFitness(currentGenome->getFitness() / (double)speciesList->operator[](currentGenome->getSpecies())->members->size());
	delete phenotype;
}

bool Population::_innovationEqual(Innovation* innovation, MutationType mType, GeneType gType, int input, int output){
	return (innovation->mutationType == mType && innovation->geneType == gType && innovation->inputId == input && innovation->outputId == output);
}

// Optimize this once everything works 
double Population::calculateCompatibilityDistance(Genome* a, Genome* b){
	int excessGenes = 0;
	int disjointGenes = 0;
	int compatibilityDistance = 0;	
	int maxPossibleMatchingNodeGeneInnovation = 0;
	int maxNodeGeneInnovation = 0;
	int maxPossibleMatchingConnectionGeneInnovation = 0;
	int maxConnectionGeneInnovation = 0;

	int matchingConnectionGeneCount = 0;
	double matchingConnectionGeneWeightDifferenceSum = 0.0;
	int maxTotalGenomeSize = std::max(a->getNodeKeys()->size() + a->getConnectionKeys()->size(), b->getNodeKeys()->size() + b->getConnectionKeys()->size());

	if(a->getNodeKeys()->operator[](a->getNodeKeys()->size() - 1) < b->getNodeKeys()->operator[](b->getNodeKeys()->size() - 1)){
		maxPossibleMatchingNodeGeneInnovation = a->getNodeKeys()->operator[](a->getNodeKeys()->size() - 1);
		maxNodeGeneInnovation = b->getNodeKeys()->operator[](b->getNodeKeys()->size() - 1);
	}
	else{
		maxPossibleMatchingNodeGeneInnovation = b->getNodeKeys()->operator[](b->getNodeKeys()->size() - 1);
		maxNodeGeneInnovation = a->getNodeKeys()->operator[](a->getNodeKeys()->size() - 1);
	}

	if(a->getConnectionKeys()->operator[](a->getConnectionKeys()->size() - 1) < b->getConnectionKeys()->operator[](b->getConnectionKeys()->size() - 1)){
		maxPossibleMatchingConnectionGeneInnovation = a->getConnectionKeys()->operator[](a->getConnectionKeys()->size() - 1);
		maxConnectionGeneInnovation = b->getConnectionKeys()->operator[](b->getConnectionKeys()->size() - 1);
	}
	else{
		maxPossibleMatchingConnectionGeneInnovation = b->getConnectionKeys()->operator[](b->getConnectionKeys()->size() - 1);
		maxConnectionGeneInnovation = a->getConnectionKeys()->operator[](a->getConnectionKeys()->size() - 1);
	}

	bool aContainsInnovation = false;
	bool bContainsInnovation = false;

	for(int i = 0; i < maxPossibleMatchingNodeGeneInnovation; i++){
		aContainsInnovation = std::find(a->getNodeKeys()->begin(), a->getNodeKeys()->end(), i) != a->getNodeKeys()->end();
		bContainsInnovation = std::find(b->getNodeKeys()->begin(), b->getNodeKeys()->end(), i) != b->getNodeKeys()->end();
		if(aContainsInnovation != bContainsInnovation){
			disjointGenes++;
		}
	}	
	for(int i = maxPossibleMatchingNodeGeneInnovation + 1; i <= maxNodeGeneInnovation; i++){
		aContainsInnovation = std::find(a->getNodeKeys()->begin(), a->getNodeKeys()->end(), i) != a->getNodeKeys()->end();
		bContainsInnovation = std::find(b->getNodeKeys()->begin(), b->getNodeKeys()->end(), i) != b->getNodeKeys()->end();
		if(aContainsInnovation || bContainsInnovation){
			excessGenes++;
		}
	}

	for(int i = 0; i < maxPossibleMatchingConnectionGeneInnovation; i++){
		aContainsInnovation = std::find(a->getConnectionKeys()->begin(), a->getConnectionKeys()->end(), i) != a->getConnectionKeys()->end();
		bContainsInnovation = std::find(b->getConnectionKeys()->begin(), b->getConnectionKeys()->end(), i) != b->getConnectionKeys()->end();
		if(aContainsInnovation != bContainsInnovation){
			disjointGenes++;
		}
		else if(aContainsInnovation && bContainsInnovation){
			matchingConnectionGeneCount++;
			matchingConnectionGeneWeightDifferenceSum += std::abs(a->getConnectionGenes()->operator[](i)->weight - b->getConnectionGenes()->operator[](i)->weight);
		}
	}

	for(int i = maxPossibleMatchingConnectionGeneInnovation + 1; i <= maxConnectionGeneInnovation; i++){
		aContainsInnovation = std::find(a->getConnectionKeys()->begin(), a->getConnectionKeys()->end(), i) != a->getConnectionKeys()->end();
		bContainsInnovation = std::find(b->getConnectionKeys()->begin(), b->getConnectionKeys()->end(), i) != b->getConnectionKeys()->end();
		if(aContainsInnovation || bContainsInnovation){
			excessGenes++;
		}
	}

	compatibilityDistance += (GENOME_COMPATIBILITY_COEFFICIENT_ONE * excessGenes) / maxTotalGenomeSize;
	compatibilityDistance += (GENOME_COMPATIBILITY_COEFFICIENT_TWO * disjointGenes) / maxTotalGenomeSize;
	compatibilityDistance += GENOME_COMPATIBILITY_COEFFICIENT_THREE * ((double)matchingConnectionGeneWeightDifferenceSum / (double)matchingConnectionGeneCount);
	// Potentially add fourth coefficient for matching node biases

	return compatibilityDistance;
}