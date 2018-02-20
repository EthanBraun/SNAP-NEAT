#include "Neuron.h"
#include <cstdlib>
#include <stdio.h>

Neuron::Neuron(int innovationNumber){
	innovation = innovationNumber;
	activation = 0.0;
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
		if(innovation == 3){
			printf("\t+= %f * %f\n", inputs->operator[](i)->from->getActivation(), inputs->operator[](i)->weight);
		} 
		newActivation += (inputs->operator[](i)->from->getActivation()) * (inputs->operator[](i)->weight);
	}
	activation = steepSigmoid(newActivation);
	printf("Neuron %d activation: %f -> %f\n", innovation, newActivation, activation);
}

void Neuron::activateDirect(double input){
	activation = input;
}
