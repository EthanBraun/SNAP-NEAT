#include "Neuron.h"
#include <cstdlib>
#include <stdio.h>

Neuron::Neuron(int innovationNumber, bool rbfNode, float c, float r){
	innovation = innovationNumber;
	activation = 0.0;
	inputs = new std::vector<Connection*>();
	outputs = new std::vector<Connection*>();
	rbf = rbfNode;
	center = c;
	radius = r;
}

Neuron::~Neuron(){
	inputs->clear();
	outputs->clear();
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

	if(rbf){
		activation = radialBasisFunction(newActivation, center, radius);
	}
	else{
		activation = steepSigmoid(newActivation);
	}
}

void Neuron::activateDirect(double input){
	activation = input;
}
