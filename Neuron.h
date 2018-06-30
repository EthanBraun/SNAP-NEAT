#ifndef NEURON_H
#define NEURON_H

#include "NodeType.h"
#include "Activation.h"
#include <vector>

struct Connection;

class Neuron {
private:
	int innovation;
	double activation;
	std::vector<Connection*>* inputs;
	std::vector<Connection*>* outputs;
	bool rbf;
	float center;
	float radius;

public:
	Neuron(int, bool, float, float);
	~Neuron();
	
	int getInnovation();
	double getActivation();
	void resetActivation();

	void addInput(Connection*);
	void addOutput(Connection*);
	void activate();
	void activateDirect(double);
};

struct Connection {
	Neuron* from;
	Neuron* to;
	double weight;
};

#endif
