#include "Population.h"

int main(int argc, char *argv[]){
	Population* pop = new Population();
	pop->evaluate(fitnessFunction);	
}

double fitnessFunction(Genome* organism){
	// Write problem-specific fitness function here
	// More fit networks should return a greater value than less fit networks
}