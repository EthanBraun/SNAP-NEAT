#include "Neuron.h"

Neuron::Neuron(int innovationNumber, double biasVal){
	innovation = innovationNumber;
	activation = 0.0;
	bias = biasVal;
	inputs = new std::vector<Connection*>();
	outputs = new std::vector<Connection*>();
}

Neuron::~Neuron(){
	for(int i = 0; i < inputs->size(); i++){
		delete inputs->operator[][i];
	}
	delete inputs;

	for(int i = 0; i < outputs->size(); i++){
		delete outputs->operator[][i];
	}
	delete outputs;
}

int Neuron::getInnovation(){
	return innovation;
}

double Neuron::getActivation(){
	return activation;
}

void Neuron::addInput(Connection* in){
	inputs->push_back(in);
}

void Neuron::addOutput(Connection* out){
	outputs->push_back(out);
}

void Neuron::activate(){
	double newActivation = 0.0;

	for(int i = 0; i < inputs->size(); i++){
		newActivation += (inputs->operator[][i]->from->getActivation()) * (inputs->operator[][i]->weight);
	}
	newActivation += bias;
	activation = sigmoid(newActivation);
}

void Neuron::activateDirect(double input){
	activation = input + bias;
}