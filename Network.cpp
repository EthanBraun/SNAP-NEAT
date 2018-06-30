#include "Network.h"
#include <cstdlib>

Network::Network(Genome* genome){
	inputLayerKeys = new std::vector<int>();
	hiddenLayerKeys = new std::vector<int>();
	outputLayerKeys = new std::vector<int>();
	inputLayer = new std::map<int, Neuron*>();
	hiddenLayer = new std::map<int, Neuron*>();
	outputLayer = new std::map<int, Neuron*>();
	connections = new std::vector<Connection*>();

	std::vector<int>* nodeKeys = genome->getNodeKeys();
	std::map<int, NodeGene*>* nodeGenes = genome->getNodeGenes();
	NodeGene* currentNodeGene;

	for(int i = 0; i < nodeKeys->size(); i++){
		currentNodeGene = nodeGenes->operator[](nodeKeys->operator[](i));
		switch(currentNodeGene->type){
		case(Input):
			inputLayerKeys->push_back(currentNodeGene->innovation);
			inputLayer->insert(std::pair<int, Neuron*>(currentNodeGene->innovation, new Neuron(currentNodeGene->innovation, false, 0.0, 0.0)));
			break;
		case(Hidden):
			if(currentNodeGene->enabled){
				hiddenLayerKeys->push_back(currentNodeGene->innovation);
				hiddenLayer->insert(std::pair<int, Neuron*>(currentNodeGene->innovation, new Neuron(currentNodeGene->innovation, currentNodeGene->rbf, currentNodeGene->center, currentNodeGene->radius)));
			}
			break;
		case(Output):
			outputLayerKeys->push_back(currentNodeGene->innovation);
			outputLayer->insert(std::pair<int, Neuron*>(currentNodeGene->innovation, new Neuron(currentNodeGene->innovation, false, 0.0, 0.0)));
			break;
		}
	}

	std::vector<int>* connectionKeys = genome->getConnectionKeys();
	std::map<int, ConnectionGene*>* connectionGenes = genome->getConnectionGenes();
	ConnectionGene* currentConnectionGene;
	Neuron* inputNode;
	Neuron* outputNode;
	Connection* connection;

	for(int i = 0; i < connectionKeys->size(); i++){
		currentConnectionGene = connectionGenes->operator[](connectionKeys->operator[](i));
		inputNode = NULL;
		outputNode = NULL;
		connection = NULL;

		if(inputLayer->count(currentConnectionGene->inputId) != 0){
			inputNode = inputLayer->operator[](currentConnectionGene->inputId);
		}
		else if(hiddenLayer->count(currentConnectionGene->inputId) != 0){
			inputNode = hiddenLayer->operator[](currentConnectionGene->inputId);
		}
		else if(outputLayer->count(currentConnectionGene->inputId) != 0){
			inputNode = outputLayer->operator[](currentConnectionGene->inputId);
		}
		else{
			// vestigial connection gene
			break;
		}

		if(inputLayer->count(currentConnectionGene->outputId) != 0){
			outputNode = inputLayer->operator[](currentConnectionGene->outputId);
		}
		else if(hiddenLayer->count(currentConnectionGene->outputId) != 0){
			outputNode = hiddenLayer->operator[](currentConnectionGene->outputId);
		}
		else if(outputLayer->count(currentConnectionGene->outputId) != 0){
			outputNode = outputLayer->operator[](currentConnectionGene->outputId);
		}
		else{
			// vestigial connection gene
			break;
		}

		if(currentConnectionGene->enabled){
			connection = new Connection();
			connection->from = inputNode;
			connection->to = outputNode;
			connection->weight = currentConnectionGene->weight;

			inputNode->addOutput(connection);
			outputNode->addInput(connection);
			connections->push_back(connection);
		}
	}

	// TODO: Establish activation order of neurons (currently just using gene order)
	//     note - as long as the order is consistent it shouldn't matter
}

Network::~Network(){
	for(int i = 0; i < connections->size(); i++){
		delete connections->operator[](i);
		connections->operator[](i) = NULL;
	}
	delete connections;

	for(int i = 0; i < inputLayerKeys->size(); i++){
		delete inputLayer->operator[](inputLayerKeys->operator[](i));
		inputLayer->operator[](inputLayerKeys->operator[](i)) = NULL;
	}
	delete inputLayer;
	delete inputLayerKeys;

	for(int i = 0; i < hiddenLayerKeys->size(); i++){
		delete hiddenLayer->operator[](hiddenLayerKeys->operator[](i));
		hiddenLayer->operator[](hiddenLayerKeys->operator[](i)) = NULL;
	}
	delete hiddenLayer;
	delete hiddenLayerKeys;

	for(int i = 0; i < outputLayerKeys->size(); i++){
		delete outputLayer->operator[](outputLayerKeys->operator[](i));
		outputLayer->operator[](outputLayerKeys->operator[](i)) = NULL;
	}
	delete outputLayer;
	delete outputLayerKeys;

	//for(int i = 0; i < connections->size(); i++){
	//	delete connections->operator[](i);
	//}
	//delete connections;
}

std::vector<int>* Network::getInputLayerKeys(){
	return inputLayerKeys;
}

std::vector<int>* Network::getHiddenLayerKeys(){
	return hiddenLayerKeys;
}

std::vector<int>* Network::getOutputLayerKeys(){
	return outputLayerKeys;
}

std::map<int, Neuron*>* Network::getInputLayer(){
	return inputLayer;
}

std::map<int, Neuron*>* Network::getHiddenLayer(){
	return hiddenLayer;
}

std::map<int, Neuron*>* Network::getOutputLayer(){
	return outputLayer;
}

std::vector<Connection*>* Network::getConnections(){
	return connections;
}

void Network::activate(std::vector<double>* inputs){
	for(int i = 0; i < inputLayerKeys->size() - 1; i++){
		inputLayer->operator[](inputLayerKeys->operator[](i))->activateDirect(inputs->operator[](i));
	}
	// Activate bias node
	inputLayer->operator[](inputLayerKeys->operator[](inputLayerKeys->size() - 1))->activateDirect(1.0);

	for(int i = 0; i < hiddenLayerKeys->size(); i++){
		hiddenLayer->operator[](hiddenLayerKeys->operator[](i))->activate();
	}
	for(int i = 0; i < outputLayerKeys->size(); i++){
		outputLayer->operator[](outputLayerKeys->operator[](i))->activate();
	}
}

void Network::compileOutputs(std::vector<double>* outputs){
	// Num output nodes is not mutable at runtime so macro is fine
	for(int i = 0; i < GENOME_NUM_OUTPUT_NODES; i++){
		outputs->push_back(outputLayer->operator[](outputLayerKeys->operator[](i))->getActivation());
	}
}

void Network::resetActivations(){
	for(int i = 0; i < inputLayerKeys->size(); i++){
		inputLayer->operator[](inputLayerKeys->operator[](i))->resetActivation();
	}
	for(int i = 0; i < hiddenLayerKeys->size(); i++){
		hiddenLayer->operator[](hiddenLayerKeys->operator[](i))->resetActivation();
	}
	for(int i = 0; i < outputLayerKeys->size(); i++){
		outputLayer->operator[](outputLayerKeys->operator[](i))->resetActivation();
	}
}
