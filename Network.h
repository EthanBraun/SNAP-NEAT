#ifndef NETWORK_H
#define NETWORK_H

#include "Neuron.h"
#include "Genome.h"
#include "Config.h"
#include <map>

class Network {
private:
	std::vector<int>* inputLayerKeys;
	std::vector<int>* hiddenLayerKeys;
	std::vector<int>* outputLayerKeys;
	std::map<int, Neuron*>* inputLayer;
	std::map<int, Neuron*>* hiddenLayer;
	std::map<int, Neuron*>* outputLayer;
	std::vector<Connection*>* connections;

public:
	Network(Genome*);
	~Network();

	std::vector<int>* getInputLayerKeys();
	std::vector<int>* getHiddenLayerKeys();
	std::vector<int>* getOutputLayerKeys();
	std::map<int, Neuron*>* getInputLayer();
	std::map<int, Neuron*>* getHiddenLayer();
	std::map<int, Neuron*>* getOutputLayer();
	std::vector<Connection*>* getConnections();

	void activate(std::vector<double>*);
	void compileOutputs(std::vector<double>*);
	void resetActivations();
};

#endif