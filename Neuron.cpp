#include "Neuron.h"
#include <cstdlib>

Neuron::Neuron(int innovationNumber, double biasVal){
	innovation = innovationNumber;
	activation = 0.0;
	bias = biasVal;
	inputs = new std::vector<Connection*>();
	outputs = new std::vector<Connection*>();
}

Neuron::~Neuron(){
	delete inputs;
	delete outputs;
}

int Neuron::getInnovation(){
	return innovation;
}

double Neuron::getActivation(){
	return activation;
}

void Neuron::resetActivation(){
	activation = 0.0;
}

void Neuron::addInput(Connection* inConnection){
	inputs->push_back(inConnection);
}

void Neuron::addOutput(Connection* outConnection){
	outputs->push_back(outConnection);
}

void Neuron::activate(){
	double newActivation = 0.0;

	for(int i = 0; i < inputs->size(); i++){
		newActivation += (inputs->operator[](i)->from->getActivation()) * (inputs->operator[](i)->weight);
	}
	newActivation += bias;
	activation = sigmoid(newActivation);
}

void Neuron::activateDirect(double input){
	activation = input;
}