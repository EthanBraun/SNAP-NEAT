#include "Population.h"
#include "Config.h"
#include <time.h>

void* fitnessFunction(Network* network, double* fitness){
	// Write problem-specific fitness function here
	// More fit networks should return a greater value than less fit networks
	std::vector<std::vector<double>*>* inputs = new std::vector<std::vector<double>*>();
	std::vector<double>* desiredOutputs = new std::vector<double>();
	double outputDifference;
	
	std::vector<double>* outputs = new std::vector<double>();
	
	for(int i = 0; i < 4; i++){
		inputs->push_back(new std::vector<double>());
		inputs->operator[](i)->push_back((i < 2) ? 0.0 : 1.0);
		inputs->operator[](i)->push_back((double)(i % 2));
		desiredOutputs->push_back((i == 0) ? 0.0 : 1.0);
	}

	for(int i = 0; i < 4; i++){
		network->activate(inputs->operator[](i));
		outputs->push_back(network->getOutputLayer()->operator[](network->getOutputLayerKeys()->operator[](0))->getActivation());
		outputDifference = std::abs(desiredOutputs->operator[](i) - network->getOutputLayer()->operator[](network->getOutputLayerKeys()->operator[](0))->getActivation());
		*fitness += (outputDifference <= 1.0) ? ((1.0 - outputDifference) / 4.0) : 0.0;
	}

	if((*fitness) >= POPULATION_MAX_GENOME_FITNESS){
		printf("\nDANK NET DETECTED:   (fitness -- %f)\n", (*fitness));
		for(int i = 0; i < 4; i++){
			outputDifference = std::abs(desiredOutputs->operator[](i) - outputs->operator[](i));
			printf("\tOutput %d: |%f - %f| = %f   (%f)\n", i, desiredOutputs->operator[](i), outputs->operator[](i), outputDifference, (outputDifference <= 1.0) ? ((1.0 - outputDifference) / 4.0) : 0.0);
		}
		printf("\n");
		for(int i = 0; i < 4; i++){
			network->activate(inputs->operator[](i));
			printf("\t%f && %f = %f\n", inputs->operator[](i)->operator[](0), inputs->operator[](i)->operator[](1), outputs->operator[](i));
		}
	}

	for(int i = 0; i < 4; i++){
		delete inputs->operator[](i);
	}
	delete inputs;
	delete desiredOutputs;
	inputs = NULL;
	desiredOutputs = NULL;
	network = NULL;

	delete outputs;
	outputs = NULL;
}

int main(int argc, char *argv[]){
	srand(time(NULL));
	Population* pop = new Population();
	pop->evaluatePopulation(fitnessFunction);
	delete pop;
}