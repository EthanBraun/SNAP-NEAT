#include "Population.h"

Population::Population(){
	innovationNumber = 0;
	organisms = new std::map<int, std::vector<Genome *>*>();
}

int Population::getInnovationNumber(){
	return innovationNumber;
}

void Population::incrementInnovationNumber(){
	innovationNumber += 1;
}

std::map<int, std::vector<Genome *>*>* Population::getOrganisms(){
	return organisms;
}