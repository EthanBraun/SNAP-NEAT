#ifndef NETWORK_H
#define NETWORK_H

#include "Neuron.h"
#include "Genome.h"
#include <map>

class Network {
private:
	std::vector<int>* inputLayerKeys;
	std::vector<int>* hiddenLayerKeys;
	std::vector<int>* outputLayerKeys;
	std::map<int, Neuron*>* inputLayer;
	std::map<int, Neuron*>* hiddenLayer;
	std::map<int, Neuron*>* outputLayer;

public:
	Network(Genome*);

};


#endif