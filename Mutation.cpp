#include "Mutation.h"
#include "Population.h"

void Mutation::mutate(Genome* genome, Population* population){
	double r;
	for(int i = 0; i <= End; i++){
		r = ((double)rand() / (double)RAND_MAX);

		switch(i){
		case(AddNode):
			if(r < MUTATION_RATE_ADD_NODE){
				Mutation::addNode(genome, population);
			}
			break;
		case(AddConnection):
			if(r < MUTATION_RATE_ADD_CONNECTION){
				Mutation::addConnection(genome, population);
			}
			break;
		case(PerturbBias):
			if(r < MUTATION_RATE_PERTURB_BIAS){
				Mutation::perturbBias(genome);
			}
			break;
		case(PerturbWeight):
			if(r < MUTATION_RATE_PERTURB_WEIGHT){
				Mutation::perturbWeight(genome);
			}
			break;
		case(ToggleNode):
			if(r < MUTATION_RATE_TOGGLE_NODE){
				Mutation::toggleNode(genome);
			}
			break;
		case(ToggleConnection):
			if(r < MUTATION_RATE_TOGGLE_CONNECTION){
				Mutation::toggleConnection(genome);
			}
			break;
		}
	}
}

void Mutation::addNode(Genome* genome, Population* population){
	// Get random connection key
	// Add new node
	// Add two new connections where previous connection was
	// Disable connection
	if(genome->getConnectionKeys()->size() == 0){
		return;
	}

	int index = rand() % genome->getConnectionKeys()->size();
	ConnectionGene* currentConnectionGene = genome->getConnectionGenes()->operator[](genome->getConnectionKeys()->operator[](index));

	int nodeGeneInnovation = population->updateInnovations(AddNode, GeneTypeNode, currentConnectionGene->inputId, currentConnectionGene->outputId);
	NodeGene* newNodeGene = new NodeGene();
	newNodeGene->innovation = nodeGeneInnovation;
	newNodeGene->bias = 0.0;
	newNodeGene->type = Hidden;
	newNodeGene->enabled = true;

	genome->getHiddenNodeKeys()->push_back(nodeGeneInnovation);
	genome->getNodeKeys()->push_back(nodeGeneInnovation);
	genome->getNodeGenes()->insert(std::pair<int, NodeGene*>(nodeGeneInnovation, newNodeGene));

	ConnectionGene* inputConnectionGene = new ConnectionGene();
	inputConnectionGene->innovation = population->updateInnovations(AddNode, GeneTypeConnection, currentConnectionGene->inputId, nodeGeneInnovation);
	inputConnectionGene->inputId = currentConnectionGene->inputId;
	inputConnectionGene->outputId = nodeGeneInnovation;
	inputConnectionGene->weight = 1.0;
	inputConnectionGene->enabled = true;

	ConnectionGene* outputConnectionGene = new ConnectionGene();
	outputConnectionGene->innovation = population->updateInnovations(AddNode, GeneTypeConnection, nodeGeneInnovation, currentConnectionGene->outputId);
	outputConnectionGene->inputId = nodeGeneInnovation;
	outputConnectionGene->outputId = currentConnectionGene->outputId;
	outputConnectionGene->weight = currentConnectionGene->weight;
	outputConnectionGene->enabled = true;

	genome->getConnectionKeys()->push_back(inputConnectionGene->innovation);
	genome->getConnectionGenes()->insert(std::pair<int, ConnectionGene*>(inputConnectionGene->innovation, inputConnectionGene));
	genome->getConnectionKeys()->push_back(outputConnectionGene->innovation);
	genome->getConnectionGenes()->insert(std::pair<int, ConnectionGene*>(outputConnectionGene->innovation, outputConnectionGene));

	currentConnectionGene->enabled = false;
}

void Mutation::addConnection(Genome* genome, Population* population){
	// Get random two random nodes (output cannot be of type input)
	// Add new connection between them
	
	
	int inputKey = rand() % genome->getNodeKeys()->size();
	NodeGene* inputNodeGene = genome->getNodeGenes()->operator[](genome->getNodeKeys()->operator[](inputKey));
	
	std::vector<int>* currentOutputKeys = new std::vector<int>();
	for(int i = 0; i < genome->getConnectionKeys()->size(); i++){
		if(genome->getConnectionGenes()->operator[](genome->getConnectionKeys()->operator[](i))->inputId == inputNodeGene->innovation){
			currentOutputKeys->push_back(genome->getConnectionGenes()->operator[](genome->getConnectionKeys()->operator[](i))->outputId);
		}
	}

	int outputKey = rand() % genome->getNodeKeys()->size();
	if((genome->getNodeGenes()->operator[](genome->getNodeKeys()->operator[](outputKey))->type == Input) || (std::find(currentOutputKeys->begin(), currentOutputKeys->end(), outputKey) != currentOutputKeys->end())){
		delete currentOutputKeys;
		return;
	}


	NodeGene* outputNodeGene = genome->getNodeGenes()->operator[](genome->getNodeKeys()->operator[](outputKey));

	ConnectionGene* newConnectionGene = new ConnectionGene();
	newConnectionGene->innovation = population->updateInnovations(AddConnection, GeneTypeConnection, inputKey, outputKey);
	newConnectionGene->inputId = inputKey;
	newConnectionGene->outputId = outputKey;
	newConnectionGene->weight = CONNECTION_GENE_INITIAL_WEIGHT;
	newConnectionGene->enabled = true;

	genome->getConnectionKeys()->push_back(newConnectionGene->innovation);
	genome->getConnectionGenes()->insert(std::pair<int, ConnectionGene*>(newConnectionGene->innovation, newConnectionGene));

	delete currentOutputKeys;
}

void Mutation::perturbBias(Genome* genome){
	// Get random node
	// Perturb bias
	int index = rand() % genome->getNodeKeys()->size();
	double perturbationPercent = (2.0 * ((double)rand() / (RAND_MAX))) - 1.0;
	genome->getNodeGenes()->operator[](genome->getNodeKeys()->operator[](index))->bias += (NODE_GENE_MAX_BIAS_PERTURBATION * perturbationPercent);
}

void Mutation::perturbWeight(Genome* genome){
	// Get random connection
	// Perturb weight
	if(genome->getConnectionKeys()->size() == 0){
		return;
	}

	int index = rand() % genome->getConnectionKeys()->size();
	double perturbationPercent = (2.0 * ((double)rand() / (RAND_MAX))) - 1.0;
	genome->getConnectionGenes()->operator[](genome->getConnectionKeys()->operator[](index))->weight += (CONNECTION_GENE_MAX_WEIGHT_PERTURBATION * perturbationPercent);
}

void Mutation::toggleNode(Genome* genome){
	// Get random hidden node
	// Toggle enabled state
	if(genome->getHiddenNodeKeys()->size() == 0){
		return;
	}

	int index = rand() % genome->getHiddenNodeKeys()->size();
	NodeGene* node = genome->getNodeGenes()->operator[](genome->getHiddenNodeKeys()->operator[](index));
	node->enabled = !node->enabled;
}

void Mutation::toggleConnection(Genome* genome){
	// Get random connection
	// Toggle enabled state
	if(genome->getConnectionKeys()->size() == 0){
		return;
	}

	int index = rand() % genome->getConnectionKeys()->size();
	bool state = genome->getConnectionGenes()->operator[](genome->getConnectionKeys()->operator[](index))->enabled;
	genome->getConnectionGenes()->operator[](genome->getConnectionKeys()->operator[](index))->enabled = !state; 
}