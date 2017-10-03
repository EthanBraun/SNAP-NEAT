#include "Population.h"

Population::Population(){
	innovationNumber = 0;
	organisms = new std::vector<Genome*>();
	speciesList = new std::vector<Species*>();
	innovations = new std::vector<Innovation*>();
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

void Population::evaluate(void* evaluationFunction){
	// Initialize population
	// For each generation:
	//		Clear innovations and soft wipe species
	//		Speciate population
	//		Calculate base/shared fitness
	//		Cull inferior organisms
	//		Reproduce to full population and mutate
}

bool Population::_innovationEqual(Innovation* innovation, MutationType mType, GeneType gType, int input, int output){
	return (innovation->mutationType == mType && innovation->geneType == gType && innovation->inputId == input && innovation->outputId == output);
}