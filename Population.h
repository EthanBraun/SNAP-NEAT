#ifndef POPULATION_H
#define POPULATION_H

#include "Network.h"
#include "Mutation.h"

class Population{
private:
    static int innovationNumber;
    std::map<int, std::vector<Genome *>*>* organisms;

public:
    Population();

    static int getInnovationNumber();
	static void incrementInnovationNumber();
	std::map<int, std::vector<Genome *>*>* getOrganisms();
	void evaluate(void *);
}

#endif