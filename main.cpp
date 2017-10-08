#include "Population.h"

int main(int argc, char *argv[]){
	Population* pop = new Population();
	pop->evaluatePopulation(fitnessFunction);	
}

double fitnessFunction(Network* currentNetwork){
	// Write problem-specific fitness function here
	// More fit networks should return a greater value than less fit networks
}