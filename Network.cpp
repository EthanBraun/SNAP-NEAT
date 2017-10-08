#include "Network.h"

Network::Network(Genome* genome){
	inputLayerKeys = new std::vector<int>();
	hiddenLayerKeys = new std::vector<int>();
	outputLayerKeys = new std::vector<int>();
	inputLayer = new std::map<int, Neuron*>();
	hiddenLayer = new std::map<int, Neuron*>();
	outputLayer = new std::map<int, Neuron *>();

	std::vector<int>* nodeKeys = genome->getNodeKeys();
	std::map<int, NodeGene*>* nodeGenes = genome->getNodeGenes();
	NodeGene* currentNodeGene;

	for(int i = 0; i < nodeKeys->size(); i++){
		currentNodeGene = nodeGenes->operator[][nodeKeys->operator[][i]];
		switch(currentNodeGene->type){
		case(NodeType::Input):
			inputLayerKeys->push_back(currentNodeGene->innovation);
			inputLayer->insert(std::pair<int, Neuron*>(currentNodeGene->innovation, new Neuron(currentNodeGene->innovation, currentNodeGene->bias)));
			break;
		case(NodeType::Hidden):
			hiddenLayerKeys->push_back(currentNodeGene->innovation);
			hiddenLayer->insert(std::pair<int, Neuron*>(currentNodeGene->innovation, new Neuron(currentNodeGene->innovation, currentNodeGene->bias)));
			break;
		case(NodeType::Output):
			outputLayerKeys->push_back(currentNodeGene->innovation);
			outputLayer->insert(std::pair<int, Neuron*>(currentNodeGene->innovation, new Neuron(currentNodeGene->innovation, currentNodeGene->bias)));
			break;
		}
	}

	std::vector<int>* connectionKeys = genome->getConnectionKeys();
	std::map<int, ConnectionGene*>* connectionGenes = genome->getConnectionGenes();
	ConnectionGene* currentConnectionGene;
	Neuron* inputNode;
	Neuron* outputNode;
	Connection* connection = NULL;

	for(int i = 0; i < connectionKeys->size(); i++){
		currentConnectionGene = connectionGenes->operator[][connectionKeys->operator[][i]];
		inputNode = NULL;
		outputNode = NULL;

		if(inputLayer->count(currentConnectionGene->inputId) != 0){
			inputNode = inputLayer->operator[][currentConnectionGene->inputId];
		}
		else if(hiddenLayer->count(currentConnectionGene->inputId) != 0){
			inputNode = hiddenLayer->operator[][currentConnectionGene->inputId];
		}
		else{
			inputNode = outputLayer->operator[][currentConnectionGene->inputId];
		}

		if(inputLayer->count(currentConnectionGene->outputId) != 0){
			outputNode = inputLayer->operator[][currentConnectionGene->outputId];
		}
		else if(hiddenLayer->count(currentConnectionGene->outputId) != 0){
			outputNode = hiddenLayer->operator[][currentConnectionGene->outputId];
		}
		else{
			outputNode = outputLayer->operator[][currentConnectionGene->outputId];
		}

		connection = new Connection();
		connection->from = inputNode;
		connection->to = outputNode;
		connection->weight = currentConnectionGene->weight;

		inputNode->addOutput(connection);
		outputNode->addInput(connection);
	}

	// TODO: Establish activation order of neurons	
}

Network::~Network(){
	for(int i = 0; i < inputLayerKeys->size(); i++){
		delete inputLayer->operator[][inputLayerKeys->operator[][i]];
	}
	delete inputLayer;
	delete inputLayerKeys;

	for(int i = 0; i < hiddenLayerKeys->size(); i++){
		delete hiddenLayer->operator[][hiddenLayerKeys->operator[][i]];
	}
	delete hiddenLayer;
	delete hiddenLayerKeys;

	for(int i = 0; i < outputLayerKeys->size(); i++){
		delete outputLayer->operator[][outputLayerKeys->operator[][i]];
	}
	delete outputLayer;
	delete outputLayerKeys;
}